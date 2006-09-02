#include "matrix.hh"
#include "obbvolume.hh"
#include "rectangle.hh"

namespace ion {
namespace math {

void OBBVolume::calcCorners()
{
	Vector3f halfExtents=m_Extents*0.5f;

	m_Corners[0].x()=m_Center.x()-halfExtents.x();
	m_Corners[0].y()=m_Center.y()-halfExtents.y();
	m_Corners[0].z()=m_Center.z()-halfExtents.z();

	m_Corners[1].x()=m_Center.x()+halfExtents.x();
	m_Corners[1].y()=m_Center.y()-halfExtents.y();
	m_Corners[1].z()=m_Center.z()-halfExtents.z();

	m_Corners[2].x()=m_Center.x()-halfExtents.x();
	m_Corners[2].y()=m_Center.y()-halfExtents.y();
	m_Corners[2].z()=m_Center.z()+halfExtents.z();

	m_Corners[3].x()=m_Center.x()+halfExtents.x();
	m_Corners[3].y()=m_Center.y()-halfExtents.y();
	m_Corners[3].z()=m_Center.z()+halfExtents.z();



	m_Corners[4].x()=m_Center.x()-halfExtents.x();
	m_Corners[4].y()=m_Center.y()+halfExtents.y();
	m_Corners[4].z()=m_Center.z()-halfExtents.z();

	m_Corners[5].x()=m_Center.x()+halfExtents.x();
	m_Corners[5].y()=m_Center.y()+halfExtents.y();
	m_Corners[5].z()=m_Center.z()-halfExtents.z();

	m_Corners[6].x()=m_Center.x()-halfExtents.x();
	m_Corners[6].y()=m_Center.y()+halfExtents.y();
	m_Corners[6].z()=m_Center.z()+halfExtents.z();

	m_Corners[7].x()=m_Center.x()+halfExtents.x();
	m_Corners[7].y()=m_Center.y()+halfExtents.y();
	m_Corners[7].z()=m_Center.z()+halfExtents.z();

	m_BoxChanged=false;
}

void OBBVolume::transform(const Matrix4f& m)
{
	if (m_BoxChanged) calcCorners();

	for (int i=0;i<8;++i) {
		const Vector3f &t=m_Corners[i];
		Vector4f v(t.x(),t.y(),t.z(),0);
		m_TransformedCorners[i]=m*v;
	}
}

void OBBVolume::projectToRectangle(const Matrix4f& projectionMatrix,FloatRectangle& rectangle) const
{
	rectangle.left()=rectangle.top()=rectangle.width()=rectangle.height()=0;

	float r=0,b=0;

	for (int i=0;i<8;++i) {
		const Vector3f &t=m_Corners[i];
		Vector4f v(t.x(),t.y(),t.z(),0);
		v=projectionMatrix*v;

		float rhw=1.0f/v.w();
		float x=v.x()*rhw;
		float y=v.y()*rhw;

		if (i==0) {
			rectangle.left()=r=x;
			rectangle.top()=b=y;
		} else {
			if (rectangle.left()>x) rectangle.left()=x;
			if (r<x) r=x;

			if (rectangle.top()<y) rectangle.top()=y;
			if (b<y) b=y;
		}
	}

	rectangle.width()=r-rectangle.left();
	rectangle.height()=b-rectangle.top();
}

}
}
