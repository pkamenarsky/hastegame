#ifndef ION_MATH_RECTANGLE_HH_INCLUDED
#define ION_MATH_RECTANGLE_HH_INCLUDED

#include "../base/dll.hh"

namespace ion {
namespace math {

	class FloatRectangle;
	class IntRectangle;

	class ION_API FloatRectangle
	{
	public:
		FloatRectangle();
		FloatRectangle(const FloatRectangle& src);
		FloatRectangle(const IntRectangle& src);
		FloatRectangle(const float l,const float t,const float w,const float h);
		virtual ~FloatRectangle();

		inline float left() const { return m_Left; }
		inline float& left() { return m_Left; }
		inline void left(const float f) { m_Left=f; }

		inline float top() const { return m_Top; }
		inline float& top() { return m_Top; }
		inline void top(const float f) { m_Top=f; }

		inline float width() const { return m_Width; }
		inline float& width() { return m_Width; }
		inline void width(const float f) { m_Width=f; }

		inline float height() const { return m_Height; }
		inline float& height() { return m_Height; }
		inline void height(const float f) { m_Height=f; }
	protected:
		float m_Left,m_Top,m_Width,m_Height;
	};

	class ION_API IntRectangle
	{
	public:
		IntRectangle();
		IntRectangle(const IntRectangle& src);
		IntRectangle(const FloatRectangle& src);
		IntRectangle(const long l,const long t,const long w,const long h);
		virtual ~IntRectangle();

		inline long left() const { return m_Left; }
		inline long& left() { return m_Left; }
		inline void left(const long f) { m_Left=f; }

		inline long top() const { return m_Top; }
		inline long& top() { return m_Top; }
		inline void top(const long f) { m_Top=f; }

		inline long width() const { return m_Width; }
		inline long& width() { return m_Width; }
		inline void width(const long f) { m_Width=f; }

		inline long height() const { return m_Height; }
		inline long& height() { return m_Height; }
		inline void height(const long f) { m_Height=f; }
	protected:
		long m_Left,m_Top,m_Width,m_Height;
	};

}
}

#endif
