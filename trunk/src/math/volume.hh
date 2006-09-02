#ifndef ION_MATH_VOLUME_HH_INCLUDED
#define ION_MATH_VOLUME_HH_INCLUDED

#include "../base/dll.hh"

namespace ion {
namespace math {

	class SphereVolume;
	class AABBVolume;
	class OBBVolume;
	class Frustum;
	class FloatRectangle;
	class Matrix4f;

	class ION_API Volume
	{
	public:
		virtual bool intersectsWith(const SphereVolume& sphere) const=0;
		virtual bool intersectsWith(const AABBVolume& aabb) const=0;
		virtual bool intersectsWith(const OBBVolume& obb) const=0;
		virtual bool intersectsWith(const Frustum& frustum) const=0;

		virtual void projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const=0;

		virtual ~Volume() {}
	protected:
		Volume() {}
	};

}
}

#endif
