#include "frustum.hh"
#include "aabbvolume.hh"
#include "spherevolume.hh"

namespace ion {
namespace math {

	AABBVolume::AABBVolume():m_CornersUpdated(false)
	{
	}

	AABBVolume::~AABBVolume()
	{
	}

	bool AABBVolume::intersectsWith(const SphereVolume& sphere) const
	{
		return sphere.intersectsWith(*this);
	}

	bool AABBVolume::intersectsWith(const AABBVolume& aabb) const
	{
		return false;
	}

	bool AABBVolume::intersectsWith(const OBBVolume& obb) const
	{
		return false;
	}

	bool AABBVolume::intersectsWith(const Frustum& frustum) const
	{
		Vector3f extentsHalf=m_Extents*0.5f;
		return frustum.checkAABB(m_Center-extentsHalf,m_Center+extentsHalf)!=Frustum::NotInFrustum;
	}

	void AABBVolume::projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const
	{
	}

	void AABBVolume::center(const Vector3f &v) { m_Center=v; m_CornersUpdated=false; }
	void AABBVolume::extents(const Vector3f &v) { m_Extents=v; m_CornersUpdated=false; }

	const Vector3f& AABBVolume::minCorner() const
	{
		if (!m_CornersUpdated) {
			m_MinCorner=m_Center-m_Extents*0.5f;
			m_CornersUpdated=true;
		}
		return m_MinCorner;
	}

	const Vector3f& AABBVolume::maxCorner() const
	{
		if (!m_CornersUpdated) {
			m_MaxCorner=m_Center-m_Extents*0.5f;
			m_CornersUpdated=true;
		}
		return m_MaxCorner;
	}

}
}
