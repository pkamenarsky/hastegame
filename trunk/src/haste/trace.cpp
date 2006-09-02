#include "common.h"

#define CD_EPSILON	(1.0f/32)

static ion_int32	s_leafbrushes[65536];
static ion_int32	s_num_leafbrushes;

static ion_int32	s_leafcurves[4096];
static ion_int32	s_num_leafcurves;

static Q3BSP		*s_bsp = NULL;

//OMFG why isn't that defined elsewhere??!
#ifndef min 
#define min(x, y)	(x) < (y) ? (x) : (y)
#endif

#ifndef max 
#define max(x, y)	(x) > (y) ? (x) : (y)
#endif

template <class T> static int CheckAABB(Vector3f mins1, Vector3f maxs1, const T mins2, const T maxs2)
{
	ion_int32	i;

#if 1
	for(i = 0; i < 3; i++)
		if(mins1[i] > (float)maxs2[i] || maxs1[i] < (float)mins2[i])
			return false;
#else
	//!!! TEMP HACK !!!
	for(i = 0; i < 2; i++)
		if(mins1[i] > (float)maxs2[i] || maxs1[i] < (float)mins2[i])
			return false;

	if(mins1[2] > (float)mins2[2] || maxs1[2] < (float)maxs2[2])
		return false;
#endif

	return true;
}

void Trace::AddLeafbrushNode(ion_int32 node_num)
{
	//walk recursively the BSP and add all leafbrushes to the global list
	if(node_num < 0)
	{
		const Q3BSP::Leaf	*leaf = leaves - (node_num + 1);
		const ion_int32		*leafface = leaffaces + leaf->m_Leafface;

		ion_int32			i;

		if(CheckAABB<int[3]>(mins, maxs, leaf->m_Mincoord, leaf->m_Maxcoord))
		{
			for(i = 0; i < leaf->m_NumLeafbrushes; i++)
				s_leafbrushes[s_num_leafbrushes++] = leafbrushes[leaf->m_Leafbrush + i];

#ifdef COLDET_PATCHES
			for(i = 0; i < leaf->m_NumLeaffaces; i++, leafface++)
				if(faces[*leafface].m_Type == Q3BSP::Facetype_Patch)
					s_leafcurves[s_num_leafcurves++] = *leafface;
#endif
		}

		return;
	}
	
	if(!CheckAABB<int[3]>(mins, maxs, nodes[node_num].m_Mincoord, nodes[node_num].m_Maxcoord))
		return;

	AddLeafbrushNode(nodes[node_num].m_FrontChild);
	AddLeafbrushNode(nodes[node_num].m_BackChild);
}

void Trace::AddModelbrushes(ion_int32 model_num, bool cull)
{
	ion_int32	i;

	s_num_leafbrushes = 0;
	s_num_leafcurves = 0;

	if(cull)
		if(!CheckAABB<float[3]>(mins, maxs, bsp_models[model_num].m_Mincoord, bsp_models[model_num].m_Maxcoord))
			return;

	for(i = 0; i < bsp_models[model_num].m_NumBrushes; i++)
		s_leafbrushes[s_num_leafbrushes++] = bsp_models[model_num].m_FirstBrush + i;
}

void Trace::AddLeafbrushes(void)
{
	s_num_leafbrushes = 0;
	s_num_leafcurves = 0;

	AddLeafbrushNode(0);
}

void Trace::AddModel(ion_int32 model_num, bool cull)
{
	if(model_num == 0)
		AddLeafbrushes();
	else
		AddModelbrushes(model_num, cull);
}

Vector3f &Trace::GetCollisionPoint(void)
{
	return col;
}

Plane *Trace::GetCollisionPlane(void)
{
	if(col_surface_index != -1)
		return col_plane;
	else
		return NULL;
}

Q3BSP::Surface *Trace::GetCollisionSurface(void)
{
	if(col_surface_index != -1)
		return (Q3BSP::Surface *)&surfaces[col_surface_index];
	else
		return NULL;
}

float Trace::GetFraction(void)
{
	return col_frac;
}

ion_int32 Trace::GetModelnum(void)
{
	return col_model_num;
}

bool Trace::StartSolid(void)
{
	return all_solid;
}

static Plane t_plane;
static Plane t_col_plane;

float Trace::TraceBox(Vector3f v_start, Vector3f v_end, Vector3f v_mins, Vector3f v_maxs, Q3BSP *bsp, int flags)
{
#ifdef COLDET_PATCHES
	const Q3BSP::BSPVertex	*vertex;
	const Q3BSP::Face		*face;
#endif
	const Q3BSP::Brush		*brush;
	const Q3BSP::Brushside	*brushside;
	const Plane				*plane;

	Matrix4f				transform;
	bool					is_transform;

	Entity					*ent;

	ion_int32				i, j, k, m;
	float					d1, d2, expand, frac, lfrac;

	if(!bsp)
		bsp = s_bsp;	
	
	if(!bsp)
		bsp = BSP;

	if(!bsp)
		return 0.0f;

	start = v_start;
	col = end = v_end;

	col_surface_index = -1;
	col_model_num = -1;

	//calc the AABB of the whole move
	mins = Vector3f(99999.9f, 99999.9f, 99999.9f);
	maxs = Vector3f(-99999.9f, -99999.9f, -99999.9f);

	for(i = 0; i < 3; i++)
	{
		mins[i] = min(mins[i], start[i] + v_mins[i]);
		mins[i] = min(mins[i], end[i] + v_mins[i]);

		maxs[i] = max(maxs[i], start[i] + v_maxs[i]);
		maxs[i] = max(maxs[i], end[i] + v_maxs[i]);
	}

	//get working pointers
	bsp_models = bsp->BSPModels();
	nodes = bsp->nodes();
	leaves = bsp->leaves();
	vertices = bsp->BSPVertices();
	faces = bsp->faces();
	leaffaces = bsp->leaffaces();
	brushes = bsp->brushes();
	brushsides = bsp->brushsides();
	surfaces = bsp->surfaces();
	leafbrushes = bsp->leafbrushes();
	planes = bsp->planes();

	num_bsp_models = bsp->numBSPModels();

	col_frac = 99999.9f;
	for(m = 0; m < num_bsp_models; m++)
	{

		is_transform = FALSE;
		if(ent = G_EntityByModel(m))
		{
			if(ent->GetClassName() == "phys_model")
			{
				is_transform = TRUE;
				transform = ent->GetModelTransform();
			}
		}

		if(is_transform)
			AddModel(m, FALSE);
		else
			AddModel(m, TRUE);

		for(i = 0; i < s_num_leafbrushes; i++)
		{
			brush = brushes + s_leafbrushes[i];
			brushside = brushsides + brush->m_FirstBrushside;

			if(!(surfaces[brush->m_Surfaceindex].m_Contents & flags))
				continue;

			start = v_start;
			end = col;

			start_solid = true;
			all_solid = true;

			frac = -1.0f;
			lfrac = 1.0f;

			for(j = 0; j < brush->m_NumBrushsides; j++, brushside++)
			{
				plane = planes + brushside->m_Planeindex;

				//lol there *must* be an easier way to do this :)
				if(is_transform)
				{
					Vector3f	t_normal = plane->normal();
					Vector3f	t_vec;
					float		t_dist;

					t_plane = *plane;
					plane = &t_plane;

					t_dist = t_plane.normal() * ent->GetCenter() - t_plane.distance();

					//FIXME
					t_plane.normal()[0] = t_normal * Vector3f(transform[0], transform[1], transform[2]);
					t_plane.normal()[1] = t_normal * Vector3f(transform[4], transform[5], transform[6]);
					t_plane.normal()[2] = t_normal * Vector3f(transform[8], transform[9], transform[10]);

					t_vec = ent->GetCenter() - t_plane.normal() * t_dist;

					t_vec += Vector3f(transform[12], transform[13], transform[14]);
					t_plane.distance(t_vec * t_plane.normal());
				}
		
				d1 = plane->distanceFromPlane(start);
				d2 = plane->distanceFromPlane(end);

				//expand the brushplane
				for(k = 0; k < 3; k++)
					p[k] = plane->normal()[k] < 0.0f ? v_maxs[k] : v_mins[k];

				expand = p * plane->normal();
				d1 += expand;
				d2 += expand;

				if(d1 > 0)
					start_solid = false;
				if(d2 > 0)
					all_solid = false;

				//completely outside
				if(d1 > 0.0f && (d2 >= CD_EPSILON || d2 >= d1))
					break;

				//completely inside
				if(d1 <= 0.0f && d2 <= 0.0f)
					continue;

				//normal move
				if(d1 > d2)
				{
					float f = (d1 - CD_EPSILON) / (d1 - d2);

					if(f < 0)f = 0;
					if(f > frac)
					{
						frac = f;
//					start = start + (end - start) * frac;

						t_col_plane = *plane;
						col_plane = &t_col_plane;
						col_surface_index = brushside->m_Surfaceindex;
						col_model_num = m;
					}
				}
				//startsolid
				else
				{
					float f = (d1 + CD_EPSILON) / (d1 - d2);

					if(f > 1) f = 1;
					if(f < lfrac)
						lfrac = f;
					//break;
//					end = start + (end - start) * frac;
				}
			}

			if(j == brush->m_NumBrushsides)
			{
				if(start_solid)
				{
					col = v_start;
					col_plane = NULL;
					col_surface_index = -1;
					col_model_num = -1;

					col = start + (end - start) * lfrac;
					return col_frac = 0.0f;
				}
				
				if(frac < lfrac)
				{
					all_solid = false;

					if(frac > -1 && frac < col_frac)
						if(frac < 0)
							frac = 0;

					if(frac < col_frac)
						col_frac = frac;

				//epsilon issues; this, guys, is the dumb/lazy/both man's way for solving them ;)
					col = start + (end - start) * frac;
//					col = start + (start - end).normalize() * CD_EPSILON * 0;
				}
			}
		}
	}

#ifdef COLDET_PATCHES
	//trace against all the curves
	for(i = 0; i < s_num_leafcurves; i++)
	{
		face = faces + s_leafcurves[i];
		vertex = vertices + face->m_FirstVertex;

//		logstream() << "LF: " << s_leafcurves[i] << "\n";
//		logstream() << "FV: " << face->m_Patchsize[0] << "\n";

		for(j = 0; j < face->m_Patchsize[0] - 1; j++)
			for(k = 0; k < face->m_Patchsize[1] - 1; k++)
			{
				TraceFacet(v_start, col, v_mins, v_maxs, vertex[j + k * face->m_Patchsize[0]].m_Position,
															vertex[j + 1 + k * face->m_Patchsize[0]].m_Position,
															vertex[j + (k + 1) * face->m_Patchsize[0]].m_Position);
				TraceFacet(v_start, col, v_mins, v_maxs, vertex[j + (k + 1) * face->m_Patchsize[0]].m_Position,
															vertex[j + 1 + k * face->m_Patchsize[0]].m_Position,
															vertex[j + 1 + (k + 1) * face->m_Patchsize[0]].m_Position);
			}

	}
#endif

//	col_frac = frac;

//	col_frac = (col - v_start).length() / (v_end - v_start).length();
	if(col_frac > 1.0f - CD_EPSILON)
		col_frac = 1.0f;

	if((v_start - col).length() < CD_EPSILON)
		col_frac = 0.0f;

//	col = start + (end - start) * col_frac;

	return col_frac;
}

#define MAKE_PLANE(p, v1, v2, v3)	(p).normal(((v2) - (v1)) ^ ((v3) - (v1))); (p).distance((p).normal() * (v1));

Plane	g_plane;

float Trace::TraceFacet(Vector3f v_start, Vector3f v_end, Vector3f v_mins, Vector3f v_maxs, Vector3f v1, Vector3f v2, Vector3f v3)
{
	Plane		c_planes[5], *plane;
	Vector3f	v;

	ion_int32	i, k;
	float		d1, d2, expand, frac;

	MAKE_PLANE(c_planes[0], v1, v2, v3);

	c_planes[1] = c_planes[0];
	c_planes[1].normal(-c_planes[1].normal());
	c_planes[1].distance(-c_planes[1].distance());

	v = v1 + c_planes[0].normal();

	MAKE_PLANE(c_planes[2], v, v1, v2);

	if(c_planes[2].distanceFromPlane(v3) < 0.0f)
	{
		MAKE_PLANE(c_planes[2], v, v1, v2);
		MAKE_PLANE(c_planes[3], v, v2, v3);
		MAKE_PLANE(c_planes[4], v, v3, v1);
	}
	else
	{
		MAKE_PLANE(c_planes[2], v1, v, v2);
		MAKE_PLANE(c_planes[3], v2, v, v3);
		MAKE_PLANE(c_planes[4], v3, v, v1);
	}

	start = v_start;
	end = col = v_end;

	start_solid = true;
	all_solid = true;

	for(i = 0, plane = c_planes; i < 5; i++, plane++)
	{
		d1 = plane->distanceFromPlane(start);
		d2 = plane->distanceFromPlane(end);

		//expand the brushplane
		for(k = 0; k < 3; k++)
			p[k] = plane->normal()[k] < 0.0f ? v_maxs[k] : v_mins[k];

		expand = p * plane->normal();
		d1 += expand;
		d2 += expand;

		//completely outside
		if(d1 >= 0.0f && d2 >= 0.0f)
		{
			start_solid = false;
			all_solid = false;
			break;
		}

		//completely inside
		if(d1 < 0.0f && d2 < 0.0f)
			continue;

		frac = d1 / (d1 - d2);
		
		//normal move
		if(d1 >= 0.0f)
		{
			start = start + (end - start) * frac;

			start_solid = false;
			g_plane = *plane;
			col_plane = (Plane *)&g_plane;
		}
		//startsolid
		else
			//break;
			end = start + (end - start) * frac;
		if(d2 > 0)
			all_solid = false;
	}

	if(i == 5)
	{
		if(start_solid)
		{
			col = v_start;
			col_plane = NULL;
			col_surface_index = -1;

			return col_frac = 0.0f;
		}
			
		all_solid = false;

		//epsilon issues; this, guys, is the dumb/lazy/both man's way for solving them ;)
		col = start + (start - end).normalize() * 0.01f;
	}

	col_frac = (col - v_start).length() / (v_end - v_start).length();
	if(col_frac > 0.95f)
		col_frac = 1.0f;

	if((v_start - col).length() < 0.1f)
		col_frac = 0.0f;

	return col_frac;
}

void SetColdetBSP(Q3BSP *v_bsp)
{
	s_bsp = v_bsp;
}

Trace::Trace(void)
{
	BSP = NULL;
}

Trace::Trace(Q3BSP *v_bsp)
{
	BSP = v_bsp;
}

void Trace::SetBSP(Q3BSP *v_bsp)
{
	BSP = v_bsp;
}
