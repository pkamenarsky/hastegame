#include "camera.hh"
#include "../video/texture2d.hh"

namespace ion {
namespace scene {

	Camera::Camera(const base::String &identifier):Node(identifier),m_ViewUpToDate(false),m_FrustumUpToDate(false),
		m_pColorRendertarget(0),m_pDepthRendertarget(0)
	{
	}

	void Camera::invalidateGlobalTransform()
	{
		m_ViewUpToDate=false;
		m_FrustumUpToDate=false;
		Node::invalidateGlobalTransform();
	}

	void Camera::lookatDirection(const math::Vector3f& origin,const math::Vector3f& out,const math::Vector3f& worldUp)
	{
		math::Matrix4f m;
		m.lookatDirection(origin,out,worldUp);
		extractLocalTransformFrom(m);
	}

	void Camera::lookatLocation(const math::Vector3f& pos,const math::Vector3f& to,const math::Vector3f& up)
	{
		math::Matrix4f m;
		m.lookatLocation(pos,to,up);
		extractLocalTransformFrom(m);
	}

	void Camera::rotation(const math::Quaternion& newrotation)
	{
		Node::rotation(newrotation);
		m_ViewUpToDate=false;
		m_FrustumUpToDate=false;
	}

	void Camera::position(const float x,const float y,const float z)
	{
		Node::position(x,y,z);
		m_ViewUpToDate=false;
		m_FrustumUpToDate=false;
	}

	void Camera::position(const math::Vector3f& newposition)
	{
		Node::position(newposition);
		m_ViewUpToDate=false;
		m_FrustumUpToDate=false;
	}

	void Camera::extractLocalTransformFrom(const math::Matrix4f& m)
	{
		Node::extractLocalTransformFrom(m);
		m_ViewUpToDate=false;
		m_FrustumUpToDate=false;
	}

	const math::Matrix4f& Camera::projectionmatrix() const
	{
		return m_Projection;
	}

	const math::Matrix4f& Camera::viewmatrix() const
	{
		if (!m_ViewUpToDate) {
			m_ViewUpToDate=true;
			m_View=transform().matrix().invert();
		}

		return m_View;
	}

	const math::Frustum& Camera::frustum() const
	{
		if (!m_FrustumUpToDate) {
			m_FrustumUpToDate=true;
			m_Frustum.extract(projectionmatrix(),viewmatrix());
		}

		return m_Frustum;
	}

	const math::IntRectangle& Camera::viewport() const
	{
		return m_Rectangle;
	}

	math::IntRectangle& Camera::viewport()
	{
		return m_Rectangle;
	}

	void Camera::viewport(const ion_uint32 left,const ion_uint32 top,const ion_uint32 width,const ion_uint32 height)
	{
		m_Rectangle.left()=(long)left;
		m_Rectangle.top()=(long)top;
		m_Rectangle.width()=(long)width;
		m_Rectangle.height()=(long)height;
		m_AspectRatio=((float)m_Rectangle.width())/((float)m_Rectangle.height());
	}

	void Camera::calculatePerspective(const float fov,const float znear,const float zfar)
	{
		m_FOV=fov;
		m_ZNear=znear;
		m_ZFar=zfar;
		m_FrustumUpToDate=false;
		m_Projection.perspective(
			fov,
			m_AspectRatio,
			znear,
			zfar);
	}

	float Camera::fov() const
	{
		return m_FOV;
	}

	float Camera::znear() const
	{
		return m_ZNear;
	}

	float Camera::zfar() const
	{
		return m_ZFar;
	}

	float Camera::aspectRatio() const
	{
		return m_AspectRatio;
	}

	void Camera::colorRendertarget(video::Texture2D *pRendertarget)
	{
		if (pRendertarget) {
			if (pRendertarget->isValid())
				m_pColorRendertarget=pRendertarget;
		} else m_pColorRendertarget=0;
	}

	video::Texture2D* Camera::colorRendertarget() const
	{
		return m_pColorRendertarget;
	}

	void Camera::depthRendertarget(video::Texture2D *pRendertarget)
	{
		if (pRendertarget) {
			if (pRendertarget->isValid())
				m_pDepthRendertarget=pRendertarget;
		} else m_pDepthRendertarget=0;
	}

	video::Texture2D* Camera::depthRendertarget() const
	{
		return m_pDepthRendertarget;
	}

	const char* Camera::type() const
	{
		return "Camera";
	}

	void Camera::respawn(base::Streamable& source)
	{
	}

	void Camera::serialize(base::Streamable& dest)
	{
	}

	bool Camera::isValid() const
	{
		return true;
	}

}
}
