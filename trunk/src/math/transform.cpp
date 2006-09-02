#include "transform.hh"

namespace ion {
namespace math {

	Transform::Transform():m_MatrixUpToDate(false)
	{
	}

	Transform::Transform(const Vector3f& position,const Quaternion& rotation):m_MatrixUpToDate(false)
	{
	}

	Transform::Transform(const Transform &src):m_MatrixUpToDate(false)
	{
		clone(src);
	}

	void Transform::clone(const Transform &src)
	{
		rotation(src.rotation());
		position(src.position());
	}

	Transform& Transform::operator =(const Transform& src)
	{
		clone(src);
		return *this;
	}

	void Transform::rotation(const Quaternion& newrotation)
	{
		m_MatrixUpToDate=false;
		m_Rotation=newrotation;
	}

	Quaternion& Transform::rotation()
	{
		m_MatrixUpToDate=false;
		return m_Rotation;
	}

	const Quaternion& Transform::rotation() const
	{
		return m_Rotation;
	}

	void Transform::position(const float x,const float y,const float z)
	{
		m_MatrixUpToDate=false;
		m_Position.x(x);
		m_Position.y(y);
		m_Position.z(z);
	}

	void Transform::position(const Vector3f& newposition)
	{
		m_MatrixUpToDate=false;
		m_Position=newposition;
	}

	Vector3f& Transform::position()
	{
		m_MatrixUpToDate=false;
		return m_Position;
	}

	const Vector3f& Transform::position() const
	{
		return m_Position;
	}

	const Matrix4f& Transform::matrix() const
	{
		if (!m_MatrixUpToDate) {
			m_Matrix.setIdentity();
			m_Matrix.fromQuaternion(m_Rotation);
			m_Matrix(3,0)=m_Position.x();
			m_Matrix(3,1)=m_Position.y();
			m_Matrix(3,2)=m_Position.z();
			m_MatrixUpToDate=true;
		}

		return m_Matrix;
	}

	void Transform::extractFrom(const Matrix4f& m)
	{
		m_MatrixUpToDate=true;
		m_Matrix=m;
		m_Rotation.fromMatrix(m);
		m_Position.x()=m(3,0);
		m_Position.y()=m(3,1);
		m_Position.z()=m(3,2);
	}

	void Transform::lookatDirection(const Vector3f& origin,const Vector3f& out,const Vector3f& worldUp)
	{
		m_MatrixUpToDate=true;
		m_Matrix.lookatDirection(origin,out,worldUp);
		m_Rotation.fromMatrix(m_Matrix);
		m_Position.x()=m_Matrix(3,0);
		m_Position.y()=m_Matrix(3,1);
		m_Position.z()=m_Matrix(3,2);
	}

	void Transform::lookatLocation(const Vector3f& origin,const Vector3f& to,const Vector3f& worldUp)
	{
		m_MatrixUpToDate=true;
		m_Matrix.lookatLocation(origin,to,worldUp);
		m_Rotation.fromMatrix(m_Matrix);
		m_Position.x()=m_Matrix(3,0);
		m_Position.y()=m_Matrix(3,1);
		m_Position.z()=m_Matrix(3,2);
	}

}
}
