#ifndef MINIRACER_CAR_HH
#define MINIRACER_CAR_HH

#include "Newton.h"
#include "../scene/node.hh"
#include "../video/mesh.hh"
#include "../scene/renderablemeshnode.hh"

class Car;

class Tire:public ion::scene::Node
{
friend class Car;
public:
	Tire(const ion::base::String& identifier,Car &rCar,NewtonWorld *pWorld,const float radius,const float width,
		const float mass,float suspensionShock,float suspensionSpring,float suspensionLength,
		const ion::math::Vector3f& rotaxis,const int ID);

	~Tire();

	Car& car();
	NewtonWorld* world();
	float width() const;
	float mass() const;
	float radius() const;
	float suspensionShock() const;
	float suspensionSpring() const;
	float suspensionLength() const;
	const ion::math::Vector3f& rotaxis() const;
	int ID() const;

	const Car& car() const;
	const NewtonWorld* world() const;

	const char *type() const;

	virtual void respawn(ion::base::Streamable& source);
	virtual void serialize(ion::base::Streamable& dest);

	bool isValid() const;

	void setTirePhysics(const NewtonJoint* vehicle,void* tireId);

	void steering(const float s);
	float steering() const;

	void torque(const float t);
	float torque() const;
protected:

	Car &m_rCar;
	NewtonWorld *m_pNewtonworld;
	float m_Steering,m_Torque,m_Width,m_Mass,m_Radius,m_SuspensionShock,m_SuspensionSpring,m_SuspensionLength;
	ion::math::Vector3f m_Rotaxis;
	int m_ID;
};

class Car:public ion::scene::Node
{
public:
	Car(const ion::base::String& identifier,NewtonWorld *pWorld,const ion::video::Mesh& srcmesh,const float mass,
		const float Ixx,const float Iyy,const float Izz);
	~Car();

	void addTire(Tire &rTire);

	NewtonCollision* chassisCollisionGeometry();
	NewtonBody* body();
	NewtonWorld* world();

	const NewtonCollision* chassisCollisionGeometry() const;
	const NewtonBody* body() const;
	const NewtonWorld* world() const;
	NewtonJoint* vehicleJoint();

	void rotation(const ion::math::Quaternion& newrotation);
	void position(const float x,const float y,const float z);
	void position(const ion::math::Vector3f& newposition);
	void extractLocalTransformFrom(const ion::math::Matrix4f& m,const bool updatePhysics=true);

	const char *type() const;

	virtual void respawn(ion::base::Streamable& source);
	virtual void serialize(ion::base::Streamable& dest);

	bool isValid() const;

protected:
	static void physicsSetTransform(const NewtonBody* body,const dFloat* matrix);
	static void physicsApplyForceAndTorque(const NewtonBody* body);
	static void tireUpdate(const NewtonJoint* vehicle);

	NewtonCollision *m_pNewtonChassisCollision;
	NewtonBody *m_pBody;
	NewtonWorld *m_pNewtonworld;
	NewtonJoint* m_pNewtonJoint;
};

#endif
