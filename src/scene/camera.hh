#ifndef ION_SCENE_CAMERA_HH_INCLUDED
#define ION_SCENE_CAMERA_HH_INCLUDED

#include "../math/frustum.hh"
#include "../math/matrix.hh"
#include "../math/rectangle.hh"
#include "../math/transform.hh"
#include "node.hh"

namespace ion {

namespace video {
	class Texture2D;
}

namespace scene {

	class ION_API Camera:public Node
	{
	public:
		Camera(const base::String &identifier);

		void invalidateGlobalTransform();

		void lookatDirection(const math::Vector3f& origin,const math::Vector3f& out,const math::Vector3f& worldUp);
		void lookatLocation(const math::Vector3f& pos,const math::Vector3f& to,const math::Vector3f& up);

		void rotation(const math::Quaternion& newrotation);
		void position(const float x,const float y,const float z);
		void position(const math::Vector3f& newposition);
		void extractLocalTransformFrom(const math::Matrix4f& m);

		virtual const math::Matrix4f& projectionmatrix() const;
		virtual const math::Matrix4f& viewmatrix() const;

		virtual const math::Frustum& frustum() const;
		
		virtual const math::IntRectangle& viewport() const;
		virtual math::IntRectangle& viewport();
		virtual void viewport(const ion_uint32 left,const ion_uint32 top,const ion_uint32 width,const ion_uint32 height);

		void calculatePerspective(const float fov,const float znear,const float zfar);

		float fov() const;
		float znear() const;
		float zfar() const;
		float aspectRatio() const;

		void colorRendertarget(video::Texture2D *pRendertarget);
		video::Texture2D* colorRendertarget() const;

		void depthRendertarget(video::Texture2D *pRendertarget);
		video::Texture2D* depthRendertarget() const;

		const char *type() const;
		void respawn(base::Streamable& source);
		void serialize(base::Streamable& dest);

		bool isValid() const;
	protected:

		float m_FOV,m_ZNear,m_ZFar,m_AspectRatio;
		mutable math::Frustum m_Frustum;
		math::Matrix4f m_Projection;
		mutable math::Matrix4f m_View;
		mutable bool m_ViewUpToDate,m_FrustumUpToDate;
		math::IntRectangle m_Rectangle;
		video::Texture2D *m_pColorRendertarget,*m_pDepthRendertarget;
	};

}
}

#endif
