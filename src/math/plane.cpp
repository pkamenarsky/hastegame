#include "plane.hh"

namespace ion {
namespace math {

	void Plane::from3Points(const Vector3f& p1,const Vector3f& c,const Vector3f& p2)
	{
		Vector3f vec1=p1-c; vec1.normalize();
		Vector3f vec2=p2-c; vec2.normalize();
		m_Normal=vec1^vec2;

		m_Distance=m_Normal*c;
	}

}
}
