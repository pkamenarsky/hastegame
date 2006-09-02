#ifndef ION_MATH_AABBVOLUME_HH_INCLUDED
#define ION_MATH_AABBVOLUME_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"
#include "volume.hh"

namespace ion {
namespace math {

	class SphereVolume;
	class OBBVolume;
	class Frustum;
	class Rectangle;

	class ION_API AABBVolume:public Volume
	{
	public:
		AABBVolume();
		~AABBVolume();

		inline bool intersectsWith(const Volume &volume) const { return volume.intersectsWith(*this); }
		bool intersectsWith(const SphereVolume& sphere) const;
		bool intersectsWith(const AABBVolume& aabb) const;
		bool intersectsWith(const OBBVolume& obb) const;
		bool intersectsWith(const Frustum& frustum) const;

		void projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const;

		inline const Vector3f& center() const { return m_Center; }
		void center(const Vector3f &v);

		inline const Vector3f& extents() const { return m_Extents; }
		void extents(const Vector3f &v);

	/*	Vector3f minCorner() const { return m_Center-m_Extents*0.5f; }
		Vector3f maxCorner() const { return m_Center+m_Extents*0.5f; }*/
		const Vector3f& minCorner() const;
		const Vector3f& maxCorner() const;
	protected:
		Vector3f m_Center,m_Extents;
		mutable Vector3f m_MinCorner,m_MaxCorner;
		mutable bool m_CornersUpdated;
	};

}
}

#endif



