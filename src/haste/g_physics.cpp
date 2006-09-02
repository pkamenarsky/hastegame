#include "common.h"

static NewtonWorld		*world;
static NewtonBody		*world_body;

static NewtonBody		*entity_bodies[MAX_ENTITY_BODIES];
static int				num_entity_bodies;

static Q3BSP			*n_bsp;

static int				id;

/*
===================================
MODIFIED APDE ENGINE POLYLIB STUFF
===================================
*/

#define MAX_POINTS_ON_WINDING	64
#define MAX_BRUSH_WINDINGS		32

#define WORLD_MAX_COORDS		65535
#define WORLD_MIN_COORDS		-65535

#define POLY_CROSS				0
#define POLY_FRONT				1
#define POLY_BACK				2
#define POLY_ON					3
#define POLY_NONE				4

#define PLANE_EPSILON			0.01f

#define	BOGUS_RANGE				8000

typedef struct
{
	Plane		plane;

	Vector3f	points[MAX_POINTS_ON_WINDING];
	int			numpoints;
}winding_t;

void CopyWinding(winding_t *src, winding_t *dst)
{
	memcpy (dst, src, sizeof (winding_t));
}

int SplitWindingByPlane (winding_t *winding, Plane *plane, winding_t *front, winding_t *back, float epsilon)
{
	int			i;
	winding_t	f, b;
	int			points_on = 0;

	if (winding->numpoints == 0)
	{
		if(front) front->numpoints = 0;
		if(back) back->numpoints = 0;
		return POLY_NONE;
	}

	f.plane = winding->plane;
	b.plane = winding->plane;

	f.numpoints = 0;
	b.numpoints = 0;

	for (i = 0; i < winding->numpoints; i++)
	{
		Vector3f p1 = winding->points[i];
		Vector3f p2 = winding->points[(i + 1) % winding->numpoints];

		float dot1, dot2;

		dot1 = p1 * plane->normal() - plane->distance();
		dot2 = p2 * plane->normal() - plane->distance();

		if (dot1 < -epsilon)
			b.points[b.numpoints++] = p1;
		else if (dot1 > epsilon)
			f.points[f.numpoints++] = p1;
		else
		{
			points_on++;

			f.points[f.numpoints++] = p1;
			b.points[b.numpoints++] = p1;
		}

		if ((dot1 < -epsilon && dot2 > epsilon) || (dot2 < -epsilon && dot1 > epsilon))
		{
			Vector3f	mid = p1 + dot1 / (dot1 - dot2) * (p2 - p1);

			b.points[b.numpoints++] = mid;
			f.points[f.numpoints++] = mid;
		}
	}

	if (f.numpoints < 3)
		f.numpoints = 0;
	if (b.numpoints < 3)
		b.numpoints = 0;

	if (front)
		CopyWinding (&f, front);
	if (back)
		CopyWinding (&b, back);

	if (points_on == winding->numpoints)
		return POLY_ON;
	if (!f.numpoints)
		return POLY_BACK;
	if (!b.numpoints)
		return POLY_FRONT;

	return POLY_CROSS;
}

//MODIFIED Q2 SOURCE
void MakeWindingFromPlane (Plane *p, winding_t *w)
{
	int			i, x;
	float		max, v;
	Vector3f	org, vright, vup;

// find the major axis

	max = -BOGUS_RANGE;
	x = -1;
	for (i = 0; i < 3; i++)
	{
		v = fabs (p->normal()[i]);
		if (v > max)
		{
			x = i;
			max = v;
		}
	}
	if (x == -1)
		logstream() << "MakeWindingFromPlane: no axis found";

	vup = Vector3f(0, 0, 0);
	switch (x)
	{
	case 0:
	case 1:
		vup[2] = 1;
		break;
	case 2:
		vup[0] = 1;
		break;
	}

	v = vup * p->normal();
	vup += -v * p->normal();
	vup.normalize();

	org = p->normal() * p->distance();

	vright = vup ^ p->normal();

	vup *= 8192;
	vright *= 8192;

// project a really big axis aligned box onto the plane

	w->points[0] = org - vright + vup;
	w->points[1] = org + vright + vup;
	w->points[2] = org + vright - vup;
	w->points[3] = org - vright - vup;

	w->numpoints = 4;
	w->plane = *p;
}

/*
=================================
NEWTON GAME DYNAMICS INTEGRATION
=================================
*/

//TEMP???
void *PhysicsAlloc(int sizeInBytes)
{
	return new char[sizeInBytes];
}

void PhysicsFree(void *ptr, int sizeInBytes)
{
	delete ptr;
}

void PH_Init(void)
{
	num_entity_bodies = 0;

	world = NewtonCreate(NULL, NULL);

	id = NewtonMaterialGetDefaultGroupID(world);
	NewtonMaterialSetDefaultCollidable(world, id, id, 1);
}

void PH_Shut(void)
{
	NewtonDestroy(world);
}

void CB(const NewtonBody *body)
{
	dFloat	omega[3];
	omega[0] = omega[1] = omega[2] = 0;
	omega[1] = -1600;

	NewtonBodySetForce(body, omega);
}

void CT(const NewtonBody* body, const dFloat* matrix)
{
	//OMG LOL PLS FIX THAT!!!
	Matrix4f	transform(	matrix[0], matrix[1], matrix[2], matrix[3],
							matrix[4], matrix[5], matrix[6], matrix[7],
							matrix[8], matrix[9], matrix[10], matrix[11],
							matrix[12], matrix[13], matrix[14], matrix[15]);
	RenderableMeshNode	*n;

	n = (RenderableMeshNode *)NewtonBodyGetUserData (body);
	n->extractLocalTransformFrom(transform);
}

void CBE(const NewtonBody *body)
{
	Entity		*ent = (Entity*)NewtonBodyGetUserData (body);

//	logstream() << "NEW: " << ent->GetVelocity()[0] << " " << ent->GetVelocity()[1] << " " << ent->GetVelocity()[2] << "\n";
//	logstream() << "OLD: " << ent->GetOldVelocity()[0] << " " << ent->GetOldVelocity()[1] << " " << ent->GetOldVelocity()[2] << "\n";
//	logstream() << "TIME: " << G_LastFrametime() << "\n";

	if(G_LastFrametime() == 0.0f)
		return;

	Vector3f	force = 2.0f * (ent->GetVelocity() - ent->GetOldVelocity()) / G_LastFrametime();
	NewtonBodySetForce(body, force);
}

void CTE(const NewtonBody* body, const dFloat* matrix)
{
	//OMG LOL PLS FIX THAT!!!
	Matrix4f	transform(	matrix[0], matrix[1], matrix[2], matrix[3],
							matrix[4], matrix[5], matrix[6], matrix[7],
							matrix[8], matrix[9], matrix[10], matrix[11],
							matrix[12], matrix[13], matrix[14], matrix[15]);

	Entity		*ent = (Entity*)NewtonBodyGetUserData (body);

	//calc the relative offset
	transform[12] -= ent->GetCenter()[0];
	transform[13] -= ent->GetCenter()[1];
	transform[14] -= ent->GetCenter()[2];

	ent->SetModelTransform(transform);
}

void PH_InitMap(Q3BSP *bsp)
{
	int						i, j;
	
	const Q3BSP::BSPModel	*model = bsp->BSPModels();
	const Q3BSP::BSPVertex	*vertices = bsp->BSPVertices();
	const Q3BSP::Face		*face = bsp->faces() + model->m_FirstFace;
	const ion_int32			*indices = bsp->meshverts();
	Q3BSP::BSPVertex		tri_verts[3];	
	
	NewtonCollision			*tree;

	float					mins[3] = {-10000, -10000, -10000};
	float					maxs[3] = {10000, 10000, 10000};

	n_bsp = bsp;

	dFloat matrix[16];
	memset(matrix, 0, sizeof(matrix));

	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;

	NewtonSetWorldSize(world, mins, maxs);
	
	NewtonSetSolverModel(world, 0); 
	NewtonSetFrictionModel(world, 0);

	NewtonSetMinimumFrameRate(world, 100);
	
	tree = NewtonCreateTreeCollision(world, NULL);

	NewtonTreeCollisionBeginBuild(tree);

	for(i = 0; i < model->m_NumFaces; i++, face++)
		if(face->m_Type == Q3BSP::Facetype::Facetype_Polygon)
			for(j = 0; j < face->m_NumMeshverts; j += 3)
			{
				tri_verts[0] = vertices[face->m_FirstVertex + indices[face->m_FirstMeshvert + j + 0]];
				tri_verts[1] = vertices[face->m_FirstVertex + indices[face->m_FirstMeshvert + j + 1]];
				tri_verts[2] = vertices[face->m_FirstVertex + indices[face->m_FirstMeshvert + j + 2]];

				NewtonTreeCollisionAddFace(tree, 3, (dFloat*)tri_verts, sizeof(Q3BSP::BSPVertex), i);
			}

	NewtonTreeCollisionEndBuild(tree, 0);

	world_body = NewtonCreateBody(world, tree);
	NewtonReleaseCollision(world, tree);

	NewtonBodySetContinuousCollisionMode(world_body, 1);

//	NewtonBodySetMassMatrix(world_body, 1000.0f, 1.0f, 1.0f, 1.0f);
	NewtonBodySetMatrix(world_body, matrix);
	NewtonBodySetAutoFreeze(world_body, 0);
	NewtonWorldUnfreezeBody(world, world_body);
}

void PH_DestroyMap(void)
{
	NewtonDestroyAllBodies(world);
	num_entity_bodies = 0;
}

void PH_Frame(void)
{
	static float	frametime = 0;
	
	frametime += G_Frametime();
	if(frametime < 1.0f / 120.0f)
		return;

	while((frametime -= 1.0f / 120.0f) > 0)
		NewtonUpdate(world, 1.0f / 120.0f);
}

void PH_AddTestBox(Vector3f pos)
{
	RenderableMeshNode	*node = new RenderableMeshNode("bla", *R_CachedModel("bla", *m_pRenderer));
	n_bsp->addRenderable(*node/*, false*/);

	NewtonCollision			*tree;
	NewtonBody				*box;

	// Set the transformation matrix
	dFloat matrix[16];
	memset(matrix, 0, sizeof(matrix));

	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];

	tree = NewtonCreateBox(world, 30, 30, 30, NULL);
	box = NewtonCreateBody(world, tree);
	NewtonReleaseCollision(world, tree);
	
	NewtonBodySetUserData (box, node);
	
	NewtonBodySetForceAndTorqueCallback(box, CB);
	NewtonBodySetTransformCallback(box, CT);

	NewtonBodySetContinuousCollisionMode(box, 1);

	Vector3f size(30.0f, 30.0f,30.0f); 
    
	float mass = 2.0f ;    
	float Ixx = mass * (size[1] * size[1] + size[2] * size[1]) / 12.0f; 
	float Iyy = mass * (size[0] * size[0] + size[2] * size[2]) / 12.0f; 
	float Izz = mass * (size[0] * size[0] + size[1] * size[1]) / 12.0f;
	
	NewtonBodySetMassMatrix(box, mass, Ixx, Iyy, Izz);
	NewtonBodySetMatrix(box, matrix);

	float omega[3];
	omega[0] = omega[1] = omega[2] = 1;
//	NewtonBodySetOmega(box, omega);	
	CT(box, matrix);

	NewtonBodySetAutoFreeze(box, 0);
	NewtonWorldUnfreezeBody(world, box);
}

NewtonBody *PH_CreateBox(Vector3f pos, Vector3f dim)
{
	NewtonCollision			*tree;
	NewtonBody				*box;

	// Set the transformation matrix
	dFloat matrix[16];
	memset(matrix, 0, sizeof(matrix));

	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];

	tree = NewtonCreateBox(world, dim[0], dim[1], dim[2], NULL);
	box = NewtonCreateBody(world, tree);
	NewtonReleaseCollision(world, tree);
	
	NewtonBodySetContinuousCollisionMode(box, 1);

	Vector3f size(30.0f, 40.0f,30.0f); 
    
	float mass = 400.0f ;    
	float Ixx = mass * (size[1] * size[1] + size[2] * size[1]) / 12.0f; 
	float Iyy = mass * (size[0] * size[0] + size[2] * size[2]) / 12.0f; 
	float Izz = mass * (size[0] * size[0] + size[1] * size[1]) / 12.0f;
	
	NewtonBodySetMassMatrix(box, mass, Ixx, Iyy, Izz);
	NewtonBodySetMatrix(box, matrix);

	NewtonBodySetAutoFreeze(box, 0);
	NewtonWorldUnfreezeBody(world, box);

	return box;
}

void PH_AddBspModel(Entity *ent)
{
	const Q3BSP::BSPModel	*model;
	const Q3BSP::Brush		*brush;
	const Q3BSP::Brushside	*brushside;
	Plane					*plane;	

	Vector3f				*v;

	winding_t				windings[64], *w;
	float					vertex_cloud[1024][3], inertia[3], centre_of_mass[3];
	int						vertex_cloud_size;

	int						i, j, k, l;		// ;)

	NewtonCollision			**collisions, *compound;
	NewtonBody				*body;

	int						collision_num = 0;
	dFloat					matrix[16];

	if(ent->GetModelNum() == -1)
		return;

	if(num_entity_bodies >= MAX_ENTITY_BODIES)
	{
		logstream() << "PH_AddBspModel: MAX_ENTITY_BODIES exceeded\n";
		return;
	}

	model = n_bsp->BSPModels() + ent->GetModelNum();

	collisions = new NewtonCollision*[model->m_NumBrushes];

	for(i = 0, brush = n_bsp->brushes() + model->m_FirstBrush; i < model->m_NumBrushes; i++, brush++)
	{
		vertex_cloud_size = 0;

		memset(vertex_cloud, 99999, sizeof(vertex_cloud));

		for(j = 0, brushside = n_bsp->brushsides() + brush->m_FirstBrushside; j < brush->m_NumBrushsides; j++, brushside++)
		{
			plane = (Plane*)n_bsp->planes() + brushside->m_Planeindex;

			MakeWindingFromPlane(plane, windings + j);

			if(j)
				for(k = j - 1; k >= 0; k--)
				{
					SplitWindingByPlane(windings + j, &windings[k].plane, NULL, windings + j, PLANE_EPSILON);
					SplitWindingByPlane(windings + k, &windings[j].plane, NULL, windings + k, PLANE_EPSILON);
				}
		}

		for(j = 0, w = windings; j < brush->m_NumBrushsides; j++, w++)
			for(k = 0, v = w->points; k < w->numpoints; k++, v++)
			{
				//place the object relative to the center
				*v -= ent->GetCenter();

				for(l = 0; l < vertex_cloud_size; l++)
					if((*v - Vector3f(vertex_cloud[l][0], vertex_cloud[l][1], vertex_cloud[l][2])).length() < 0.1f)
						break;

					if(l == vertex_cloud_size)
					{
						vertex_cloud[vertex_cloud_size][0] = (*v)[0];
						vertex_cloud[vertex_cloud_size][1] = (*v)[1];
						vertex_cloud[vertex_cloud_size++][2] = (*v)[2];
					}
			}

		memset(matrix, 0, sizeof(matrix));

		matrix[0] = 1;
		matrix[5] = 1;
		matrix[10] = 1;
		matrix[15] = 1;
		matrix[12] = ent->GetCenter()[0];
		matrix[13] = ent->GetCenter()[1];
		matrix[14] = ent->GetCenter()[2];

		collisions[collision_num++] =  NewtonCreateConvexHull(world, vertex_cloud_size, (dFloat*)vertex_cloud, sizeof(float) * 3, NULL);
	}

	if(collision_num == 1)
		compound = collisions[0];
	else
		compound =  NewtonCreateCompoundCollision(world, collision_num, collisions);

	body = NewtonCreateBody(world, compound);
	NewtonConvexCollisionCalculateInertialMatrix(compound, inertia, centre_of_mass);
	NewtonReleaseCollision(world, compound);

	NewtonBodySetUserData(body, ent);
	
	NewtonBodySetForceAndTorqueCallback(body, CBE);
	NewtonBodySetTransformCallback(body, CTE);

	NewtonBodySetContinuousCollisionMode(body, 1);

	inertia[0] = inertia[1] = inertia[2] = 140;

	NewtonBodySetCentreOfMass(body, centre_of_mass);
	NewtonBodySetMassMatrix(body, 2.0f, inertia[0] * 2, inertia[1] * 2, inertia[2] * 2);
	NewtonBodySetMatrix(body, matrix);

	NewtonBodySetAutoFreeze(body, 0);
	NewtonWorldUnfreezeBody(world, body);

	entity_bodies[num_entity_bodies++] = body;

	delete collisions;
}

void PH_DestroyBspModels(void)
{
	int		i;

	for(i = 0; i < num_entity_bodies; i++)
		NewtonDestroyBody(world, entity_bodies[i]);

	num_entity_bodies = 0;
}