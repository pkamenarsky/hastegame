#include "../base/log.hh"
#include "../video/coreenums.hh"
#include "newtonnode.hh"

Newtonnode::Newtonnode(const ion::base::String& identifier):Node(identifier),m_pNewtonCollision(0),
m_pBody(0),m_pNewtonworld(0)
{
}

Newtonnode::~Newtonnode()
{
	if ((m_pNewtonCollision!=0) && (m_pNewtonworld!=0))
		NewtonReleaseCollision(m_pNewtonworld,m_pNewtonCollision);
}

void Newtonnode::initCube(NewtonWorld *pWorld,const float xlength,const float ylength,const float zlength)
{
	if (pWorld==0) {
		ion::base::log("Newtonnode::initCube()",ion::base::Error) << "No NewtonWorld pointer given\n";
		return;
	}


	if ((m_pNewtonCollision!=0) && (m_pNewtonworld!=0))
		NewtonReleaseCollision(m_pNewtonworld,m_pNewtonCollision);

	m_pNewtonworld=pWorld;
	m_pNewtonCollision=NewtonCreateBox(m_pNewtonworld,xlength,ylength,zlength,0);
	m_pBody=NewtonCreateBody(m_pNewtonworld,m_pNewtonCollision);
	NewtonBodySetUserData(m_pBody,this);
	NewtonBodySetMassMatrix(m_pBody, 1.0f, 1.0f, 1.0f, 1.0f);
	NewtonReleaseCollision(m_pNewtonworld,m_pNewtonCollision);

	NewtonBodySetTransformCallback (m_pBody, physicsSetTransform);
	NewtonBodySetForceAndTorqueCallback (m_pBody, physicsApplyForceAndTorque);
}

void Newtonnode::initConvexHull(NewtonWorld *pWorld,const ion::video::Mesh& srcmesh,const float mass,
								const float Ixx,const float Iyy,const float Izz)
{
	if (pWorld==0) {
		ion::base::log("Newtonnode::initConvexHull()",ion::base::Error) << "No NewtonWorld pointer given\n";
		return;
	}

	if (!srcmesh.isValid()) {
		ion::base::log("Newtonnode::initConvexHull()",ion::base::Error) << "Source mesh is invalid\n";
		return;
	}

	if (!srcmesh.vertexstream().isMapped()) {
		ion::base::log("Newtonnode::initConvexHull()",ion::base::Error) << "source mesh \"" << srcmesh.objIdentifier() << " is not mapped!\n";
		return;
	}




	if ((m_pNewtonCollision!=0) && (m_pNewtonworld!=0))
		NewtonReleaseCollision(m_pNewtonworld,m_pNewtonCollision);

	float *pPoints;
	{
		pPoints=new float[3*srcmesh.vertexstream().capacity()];
		for (ion_uint32 v=0;v<srcmesh.vertexstream().capacity();++v) {
			const ion::math::Vector3f &rV=srcmesh.vertexstream().position(v);
			pPoints[v*3+0]=rV.x();
			pPoints[v*3+1]=rV.y();
			pPoints[v*3+2]=rV.z();
		}
	}

	m_pNewtonworld=pWorld;
	m_pNewtonCollision=NewtonCreateConvexHull(m_pNewtonworld,srcmesh.vertexstream().capacity(),pPoints,12,0);
	m_pBody=NewtonCreateBody(m_pNewtonworld,m_pNewtonCollision);
	NewtonBodySetUserData(m_pBody,this);
	NewtonBodySetMassMatrix(m_pBody,mass,Ixx,Iyy,Izz);
	NewtonReleaseCollision(m_pNewtonworld,m_pNewtonCollision);

	NewtonBodySetTransformCallback (m_pBody, physicsSetTransform);
	NewtonBodySetForceAndTorqueCallback (m_pBody, physicsApplyForceAndTorque);

	delete [] pPoints;
}

NewtonCollision* Newtonnode::collisionGeometry()
{
	return m_pNewtonCollision;
}

NewtonBody* Newtonnode::body()
{
	return m_pBody;
}

NewtonWorld* Newtonnode::world()
{
	return m_pNewtonworld;
}

const NewtonCollision* Newtonnode::collisionGeometry() const
{
	return m_pNewtonCollision;
}

const NewtonBody* Newtonnode::body() const
{
	return m_pBody;
}

const NewtonWorld* Newtonnode::world() const
{
	return m_pNewtonworld;
}

void Newtonnode::bodyActivationCallback(NewtonBodyActivationState callback)
{
	NewtonBodySetAutoactiveCallback (m_pBody,callback);
}

void Newtonnode::bodyForceAndTorqueCallback(NewtonApplyForceAndTorque callback)
{
	NewtonBodySetForceAndTorqueCallback(m_pBody,callback);
}

void Newtonnode::bodyDestructorCallback(NewtonBodyDestructor callback)
{
	NewtonBodySetDestructorCallback(m_pBody,callback);
}

void Newtonnode::rotation(const ion::math::Quaternion& newrotation)
{
	Node::rotation(newrotation);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Newtonnode::position(const float x,const float y,const float z)
{
	Node::position(x,y,z);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Newtonnode::position(const ion::math::Vector3f& newposition)
{
	Node::position(newposition);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Newtonnode::extractLocalTransformFrom(const ion::math::Matrix4f& m)
{
	Node::extractLocalTransformFrom(m);
	NewtonBodySetMatrix(m_pBody,m);
}

const char* Newtonnode::type() const
{
	return "Newtonnode";
}

void Newtonnode::respawn(ion::base::Streamable& source)
{
}

void Newtonnode::serialize(ion::base::Streamable& dest)
{
}

bool Newtonnode::isValid() const
{
	return (m_pNewtonCollision!=0) && (m_pNewtonworld!=0);
}

void Newtonnode::physicsApplyForceAndTorque(const NewtonBody* body)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	float force[3]={0.0f, -mass * 9.87f, 0.0f};
	NewtonBodySetForce(body, force);
}

void Newtonnode::physicsSetTransform(const NewtonBody* body, const dFloat* matrix)
{
	Newtonnode* primitive;

	primitive=(Newtonnode*) NewtonBodyGetUserData(body);

	ion::math::Matrix4f m;
	for (int i=0;i<16;++i) m[i]=matrix[i];

	primitive->extractLocalTransformFrom(m);
}
