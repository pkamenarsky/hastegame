#include "../base/log.hh"
#include "../video/coreenums.hh"
#include "car.hh"

Tire::Tire(const ion::base::String& identifier,Car &rCar,NewtonWorld *pWorld,const float radius,const float width,
	const float mass,float suspensionShock,float suspensionSpring,float suspensionLength,
	const ion::math::Vector3f& rotaxis,const int ID):Node(identifier),m_rCar(rCar),m_pNewtonworld(pWorld),
	m_Steering(0),m_Torque(0),m_Width(width),m_Mass(mass),m_Radius(radius),m_SuspensionShock(suspensionShock),
	m_SuspensionSpring(suspensionSpring),m_SuspensionLength(suspensionLength),m_Rotaxis(rotaxis),m_ID(ID)
{
	inheritTransform(false);
}

Tire::~Tire()
{
}

Car& Tire::car()
{
	return m_rCar;
}

NewtonWorld* Tire::world()
{
	return m_pNewtonworld;
}

float Tire::width() const
{
	return m_Width;
}

float Tire::mass() const
{
	return m_Mass;
}

float Tire::radius() const
{
	return m_Radius;
}

float Tire::suspensionShock() const
{
	return m_SuspensionShock;
}

float Tire::suspensionSpring() const
{
	return m_SuspensionSpring;
}

float Tire::suspensionLength() const
{
	return m_SuspensionLength;
}

const ion::math::Vector3f& Tire::rotaxis() const
{
	return m_Rotaxis;
}

int Tire::ID() const
{
	return m_ID;
}

const Car& Tire::car() const
{
	return m_rCar;
}

const NewtonWorld* Tire::world() const
{
	return m_pNewtonworld;
}

const char* Tire::type() const
{
	return "Tire";
}

void Tire::respawn(ion::base::Streamable& source)
{
}

void Tire::serialize(ion::base::Streamable& dest)
{
}

bool Tire::isValid() const
{
	return m_rCar.isValid() && (m_pNewtonworld!=0);
}

void Tire::setTirePhysics(const NewtonJoint* vehicle,void* tireId)
{
	NewtonVehicleSetTireTorque(vehicle,tireId,m_Torque);
	NewtonVehicleSetTireSteerAngle(vehicle,tireId,m_Steering);
}

void Tire::steering(const float s)
{
	m_Steering=s;
}

float Tire::steering() const
{
	return m_Steering;
}

void Tire::torque(const float t)
{
	m_Torque=t;
}

float Tire::torque() const
{
	return m_Torque;
}






Car::Car(const ion::base::String& identifier,NewtonWorld *pWorld,const ion::video::Mesh& srcmesh,const float mass,
		 const float Ixx,const float Iyy,const float Izz):Node(identifier),m_pNewtonChassisCollision(0),m_pBody(0),
		 m_pNewtonworld(pWorld),m_pNewtonJoint(0)
{
	if (pWorld==0) {
		ion::base::log("Car::Car()",ion::base::Error) << "No NewtonWorld pointer given\n";
		return;
	}

	if (!srcmesh.isValid()) {
		ion::base::log("Car::Car()",ion::base::Error) << "Source mesh is invalid\n";
		return;
	}

	if (!srcmesh.vertexstream().isMapped()) {
		ion::base::log("Car::Car()",ion::base::Error) << "source mesh \"" << srcmesh.objIdentifier() << " is not mapped!\n";
		return;
	}


	if ((m_pNewtonChassisCollision!=0) && (m_pNewtonworld!=0))
		NewtonReleaseCollision(m_pNewtonworld,m_pNewtonChassisCollision);

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

	ion::math::Matrix4f c;

	m_pNewtonworld=pWorld;
	m_pNewtonChassisCollision=NewtonCreateConvexHull(m_pNewtonworld,srcmesh.vertexstream().capacity(),pPoints,12,c);
	m_pBody=NewtonCreateBody(m_pNewtonworld,m_pNewtonChassisCollision);
	NewtonBodySetUserData(m_pBody,this);

	ion::math::Vector3f origin,inertia;

	// calculate the moment of inertia and the relative center of mass of the solid
	NewtonConvexCollisionCalculateInertialMatrix (m_pNewtonChassisCollision, &inertia[0], &origin[0]);	

	float ixx = mass * inertia[0];
	float iyy = mass * inertia[1];
	float izz = mass * inertia[2];

	// set the mass matrix
	NewtonBodySetMassMatrix (m_pBody, mass, ixx, iyy, izz);

	origin.y()=-1;
	NewtonBodySetCentreOfMass (m_pBody, &origin[0]);

	NewtonBodySetMatrix(m_pBody,localTransform().matrix());

	NewtonReleaseCollision(m_pNewtonworld,m_pNewtonChassisCollision);

	NewtonBodySetTransformCallback (m_pBody, physicsSetTransform);
	NewtonBodySetForceAndTorqueCallback (m_pBody, physicsApplyForceAndTorque);

	float updir[3]={0,1,0};

	m_pNewtonJoint=NewtonConstraintCreateVehicle(m_pNewtonworld,&updir[0],m_pBody);
	NewtonVehicleSetTireCallback(m_pNewtonJoint,tireUpdate);

	delete [] pPoints;
}

Car::~Car()
{
	if ((m_pNewtonChassisCollision!=0) && (m_pNewtonworld!=0))
		NewtonReleaseCollision(m_pNewtonworld,m_pNewtonChassisCollision);
}

void Car::addTire(Tire &rTire)
{
	rTire.rotation(ion::math::Quaternion::generateFromRotAxisAndAngle(ion::math::Vector3f(0,-1,0),3.1415926535f*0.5f));

	NewtonVehicleAddTire(m_pNewtonJoint,rTire.localTransform().matrix(),rTire.rotaxis(),rTire.mass(),rTire.width(),
		rTire.radius(),rTire.suspensionShock(),rTire.suspensionSpring(),rTire.suspensionLength(),&rTire,rTire.ID());
	addChild(rTire);
}

NewtonCollision* Car::chassisCollisionGeometry()
{
	return m_pNewtonChassisCollision;
}

NewtonBody* Car::body()
{
	return m_pBody;
}

NewtonWorld* Car::world()
{
	return m_pNewtonworld;
}

const NewtonCollision* Car::chassisCollisionGeometry() const
{
	return m_pNewtonChassisCollision;
}

const NewtonBody* Car::body() const
{
	return m_pBody;
}

const NewtonWorld* Car::world() const
{
	return m_pNewtonworld;
}

NewtonJoint* Car::vehicleJoint()
{
	return m_pNewtonJoint;
}

void Car::rotation(const ion::math::Quaternion& newrotation)
{
	Node::rotation(newrotation);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Car::position(const float x,const float y,const float z)
{
	Node::position(x,y,z);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Car::position(const ion::math::Vector3f& newposition)
{
	Node::position(newposition);
	NewtonBodySetMatrix(m_pBody,localTransform().matrix());
}

void Car::extractLocalTransformFrom(const ion::math::Matrix4f& m,const bool updatePhysics)
{
	Node::extractLocalTransformFrom(m);
	if (updatePhysics) NewtonBodySetMatrix(m_pBody,m);
}

const char* Car::type() const
{
	return "Car";
}

void Car::respawn(ion::base::Streamable& source)
{
}

void Car::serialize(ion::base::Streamable& dest)
{
}

bool Car::isValid() const
{
	return (m_pNewtonChassisCollision!=0) && (m_pNewtonworld!=0);
}

void Car::physicsApplyForceAndTorque(const NewtonBody* body)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	float force[3]={0.0f, -mass * 9.87f, 0.0f};
	NewtonBodySetForce(body, force);
}

void Car::physicsSetTransform(const NewtonBody* body, const dFloat* matrix)
{
	Car* pCar;

	pCar=(Car*) NewtonBodyGetUserData(body);

	ion::math::Matrix4f m;
	for (int i=0;i<16;++i) m[i]=matrix[i];

	pCar->extractLocalTransformFrom(m,false);

	//Set the global matrix for each tire
	for (void* tyreId = NewtonVehicleGetFirstTireID (pCar->vehicleJoint()); tyreId; tyreId = NewtonVehicleGetNextTireID (pCar->vehicleJoint(), tyreId)) {

		// get the graphic object and set the transformation matrix 
		Tire *tire = (Tire*) NewtonVehicleGetTireUserData (pCar->vehicleJoint(), tyreId);
		NewtonVehicleGetTireMatrix (pCar->vehicleJoint(), tyreId,m);
		tire->extractLocalTransformFrom(m);
	}
}

void Car::tireUpdate(const NewtonJoint* vehicle)
{
	void* tyreId;
	Tire* tire;

	// iterate trough each tire applying the tire dynamics
	for (tyreId = NewtonVehicleGetFirstTireID (vehicle); tyreId; tyreId = NewtonVehicleGetNextTireID (vehicle, tyreId)) {
		// get the graphic object and set the transformation matrix 
		tire = (Tire*) NewtonVehicleGetTireUserData(vehicle, tyreId);
		tire->setTirePhysics(vehicle,tyreId);
	}
}
