#ifndef ION_MATH_PLANE_HH_INCLUDED
#define ION_MATH_PLANE_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"

//#define BLAH_HASTE_CODE

namespace ion {
namespace math {

class ION_API Plane {
protected:

#ifdef BLAH_HASTE_CODE
public:
	Vector3f m_Normal;
	float m_Distance;
	
	long m_Refcount;
#else
	Vector3f m_Normal;
	float m_Distance;
#endif
public:
#ifdef BLAH_HASTE_CODE
	void addRef(void) { m_Refcount++; }
	void release(void) { if(--m_Refcount == 0) delete this; }

	inline void set(float x, float y, float z, float v_dist) { m_Normal.set(x, y, z); m_Distance = v_dist; }
#endif

	Plane():m_Distance(0) {}
	Plane(const Vector3f& normal,const float distance):m_Normal(normal),m_Distance(distance) {}
	Plane(const Plane& srcplane):m_Normal(srcplane.normal()),m_Distance(srcplane.distance()) {}
		
	inline Vector3f normal() const { return m_Normal; }
	inline Vector3f& normal() { return m_Normal; }
	inline void normal(const Vector3f& vec) { m_Normal=vec; }
		
	inline float a() const { return m_Normal.x(); }
	inline void a(const float newA) { m_Normal.x()=newA; }
	
	inline float b() const { return m_Normal.y(); }
	inline void b(const float newB) { m_Normal.y()=newB; }
	
	inline float c() const { return m_Normal.z(); }
	inline void c(const float newC) { m_Normal.z()=newC; }

	inline float d() const { return m_Distance; }
	inline void d(const float dist) { m_Distance=dist; }
	inline float distance() const { return m_Distance; }
	inline void distance(const float dist) { m_Distance=dist; }

	void from3Points(const Vector3f& p1,const Vector3f& c,const Vector3f& p2);
	
	inline float distanceFromPlane(const Vector3f& vec) const { return (a()*vec.x()+b()*vec.y()+c()*vec.z())-m_Distance; }
};

}
}

#endif
