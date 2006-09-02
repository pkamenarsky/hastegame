#include "rectangle.hh"

namespace ion {
namespace math {

	FloatRectangle::FloatRectangle()
	{
	}

	FloatRectangle::FloatRectangle(const FloatRectangle& src):m_Left(src.left()),m_Top(src.top()),m_Width(src.width()),m_Height(src.height())
	{
	}

	FloatRectangle::FloatRectangle(const IntRectangle& src):
	m_Left((float)(src.left())),m_Top((float)(src.top())),m_Width((float)(src.width())),m_Height((float)(src.height()))
	{
	}

	FloatRectangle::FloatRectangle(const float l,const float t,const float w,const float h):m_Left(l),m_Top(t),m_Width(w),m_Height(h)
	{
	}

	FloatRectangle::~FloatRectangle()
	{
	}


	IntRectangle::IntRectangle()
	{
	}

	IntRectangle::IntRectangle(const IntRectangle& src):m_Left(src.left()),m_Top(src.top()),m_Width(src.width()),m_Height(src.height())
	{
	}

	IntRectangle::IntRectangle(const FloatRectangle& src):
	m_Left((long)(src.left())),m_Top((long)(src.top())),m_Width((long)(src.width())),m_Height((long)(src.height()))
	{
	}

	IntRectangle::IntRectangle(const long l,const long t,const long w,const long h):m_Left(l),m_Top(t),m_Width(w),m_Height(h)
	{
	}

	IntRectangle::~IntRectangle()
	{
	}

}
}
