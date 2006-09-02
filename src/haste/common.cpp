#include "common.h"

void VecSwizzle(Vector3f& v)
{
	float temp=v.y();
	v.y()=v.z();
	v.z()=-temp;
	v.x()*=-1;
}

void AngleVectors(Vector3f &angles, Vector3f &front, Vector3f &right, Vector3f &up)
{
	Quaternion	q = (Quaternion::generateFromEulerAngles(angles[0] / 100,0,0) * Quaternion::generateFromEulerAngles(0,angles[1] / 100,0) * Quaternion::generateFromEulerAngles(0,0,angles[2] / 100)).inverted();

	front = q.rotateVector(Vector3f(0, 0, 1));
	right = q.rotateVector(Vector3f(1, 0, 0));
	up = q.rotateVector(Vector3f(0, 1, 0));
}

void AngleVectorsQ(Vector3f &angles, Quaternion &q)
{
	q = (Quaternion::generateFromEulerAngles(angles[2] / 100,0,0) * Quaternion::generateFromEulerAngles(0,-angles[1] / 100,0) * Quaternion::generateFromEulerAngles(0,0,angles[0] / 100)).inverted();
}

void AngleVectorsQ2(Vector3f &angles, Quaternion &q)
{
	q = (Quaternion::generateFromEulerAngles(angles[2] / 100,0,0) * Quaternion::generateFromEulerAngles(0,-angles[1] / 100,0) * Quaternion::generateFromEulerAngles(0,0,angles[0] / 100)).inverted();
}