#include "matrix.hh"

namespace ion {
namespace math {

	const Matrix4f& Matrix4f::identitymatrix()
	{
		static Matrix4f matrix;
		return matrix;
	}

	void Matrix4f::lookatDirection(const Vector3f& origin,const Vector3f& out,const Vector3f& worldUp)
	{
		Vector3f right(worldUp^out); right.normalize();
		Vector3f up(out^right); up.normalize();

		/*m_11=right.x();	m_21=right.y();	m_31=right.z();	m_14=0.0f;
		m_12=up.x();	m_22=up.y();	m_32=up.z();	m_24=0.0f;
		m_13=out.x();	m_23=out.y();	m_33=out.z();	m_34=0.0f;*/

		m_11=right.x();	m_12=right.y();	m_13=right.z();	m_14=0;
		m_21=up.x();	m_22=up.y();	m_23=up.z();	m_24=0;
		m_31=out.x();	m_32=out.y();	m_33=out.z();	m_34=0;

		m_41=origin.x();
		m_42=origin.y();
		m_43=origin.z();
	}

	void Matrix4f::lookatLocation(const Vector3f& origin,const Vector3f& to,const Vector3f& worldUp)
	{
		Vector3f out(to-origin); out.normalize();
		/*Vector3f right(worldUp^out); right.normalize();
		Vector3f up(out^right); up.normalize();

		m_11=right.x();	m_12=right.y();	m_13=right.z();	m_14=0;
		m_21=up.x();	m_22=up.y();	m_23=up.z();	m_24=0;
		m_31=out.x();	m_32=out.y();	m_33=out.z();	m_34=0;

		m_41=origin.x();
		m_42=origin.y();
		m_43=origin.z();*/

		lookatDirection(origin,out,worldUp);
	}

}
}
