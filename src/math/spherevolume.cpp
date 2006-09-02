#include "frustum.hh"
#include "aabbvolume.hh"
#include "spherevolume.hh"

namespace ion {
namespace math {

	SphereVolume::SphereVolume()
	{
	}

	SphereVolume::~SphereVolume()
	{
	}

	bool SphereVolume::intersectsWith(const SphereVolume& sphere) const
	{
		float squaredRadSum=radius()+sphere.radius();
		squaredRadSum*=squaredRadSum;

		float squaredDist=(sphere.center()-center()).lengthSquared();

		return (squaredDist<squaredRadSum);
	}

	bool SphereVolume::intersectsWith(const Vector3f& aabbmin,const Vector3f& aabbmax) const
	{
		// From Gamasutra article "Simple Intersection Tests For Games", October 18, 1999
		float s,d=0;

		for (long i=0;i<3;++i) {
			if (m_Center[i]<aabbmin[i]) {
				s=m_Center[i]-aabbmin[i];
				d+=s*s;
			} else if (m_Center[i]>aabbmax[i]) {
				s=m_Center[i]-aabbmax[i];
				d+=s*s;
			}
		}

		return d<=(m_Radius*m_Radius);
	}

	bool SphereVolume::intersectsWith(const AABBVolume& aabb) const
	{
		// From Gamasutra article "Simple Intersection Tests For Games", October 18, 1999
		float s,d=0;

		const Vector3f &minB=aabb.minCorner();
		const Vector3f &maxB=aabb.maxCorner();

		for (long i=0;i<3;++i) {
			if (m_Center[i]<minB[i]) {
				s=m_Center[i]-minB[i];
				d+=s*s;
			} else if (m_Center[i]>maxB[i]) {
				s=m_Center[i]-maxB[i];
				d+=s*s;
			}
		}

		return d<=(m_Radius*m_Radius);
	}

	bool SphereVolume::intersectsWith(const OBBVolume& obb) const
	{
		return false;
	}

	bool SphereVolume::intersectsWith(const Frustum& frustum) const
	{
		return frustum.checkSphere(center(),radius())!=Frustum::NotInFrustum;
	}

	void SphereVolume::projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const
	{
	}

}
}
