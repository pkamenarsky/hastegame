#ifndef MINIRACER_NEWTONNODE_HH
#define MINIRACER_NEWTONNODE_HH

#include "Newton.h"
#include "../scene/node.hh"
#include "../video/mesh.hh"

class Newtonnode:public ion::scene::Node
{
public:
	Newtonnode(const ion::base::String& identifier);
	~Newtonnode();

	void initCube(NewtonWorld *pWorld,const float xlength,const float ylength,const float zlength);
	// NOTE: initConvexHull() expects the source mesh' vertexstream to be mapped!
	void initConvexHull(NewtonWorld *pWorld,const ion::video::Mesh& srcmesh,const float mass,
		const float Ixx,const float Iyy,const float Izz);

	NewtonCollision* collisionGeometry();
	NewtonBody* body();
	NewtonWorld* world();

	const NewtonCollision* collisionGeometry() const;
	const NewtonBody* body() const;
	const NewtonWorld* world() const;

	void bodyActivationCallback(NewtonBodyActivationState callback);
	void bodyForceAndTorqueCallback(NewtonApplyForceAndTorque callback);
	void bodyDestructorCallback(NewtonBodyDestructor callback);

	void rotation(const ion::math::Quaternion& newrotation);
	void position(const float x,const float y,const float z);
	void position(const ion::math::Vector3f& newposition);
	void extractLocalTransformFrom(const ion::math::Matrix4f& m);

	const char *type() const;

	virtual void respawn(ion::base::Streamable& source);
	virtual void serialize(ion::base::Streamable& dest);

	bool isValid() const;

protected:
	static void physicsSetTransform(const NewtonBody* body, const dFloat* matrix);
	static void physicsApplyForceAndTorque(const NewtonBody* body);

	NewtonCollision *m_pNewtonCollision;
	NewtonBody *m_pBody;
	NewtonWorld *m_pNewtonworld;
};

#endif
