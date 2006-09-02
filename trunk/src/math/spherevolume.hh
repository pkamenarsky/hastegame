#ifndef ION_MATH_SPHEREVOLUME_HH_INCLUDED
#define ION_MATH_SPHEREVOLUME_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"
#include "volume.hh"

namespace ion {
namespace math {

	class AABBVolume;
	class OBBVolume;
	class Frustum;
	class Rectangle;

	class ION_API SphereVolume:public Volume
	{
	public:
		SphereVolume();
		~SphereVolume();

		inline bool intersectsWith(const Volume &volume) const { return volume.intersectsWith(*this); }
		bool intersectsWith(const SphereVolume& sphere) const;
		bool intersectsWith(const Vector3f& aabbmin,const Vector3f& aabbmax) const;
		bool intersectsWith(const AABBVolume& aabb) const;
		bool intersectsWith(const OBBVolume& obb) const;
		bool intersectsWith(const Frustum& frustum) const;

		void projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const;

		inline const Vector3f& center() const { return m_Center; }
		inline void center(const Vector3f &v) { m_Center=v; }

		inline float radius() const { return m_Radius; }
		inline void radius(const float radius) { m_Radius=radius; }
	protected:
		Vector3f m_Center;
		float m_Radius;
};

}
}

#endif
