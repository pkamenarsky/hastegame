#ifndef ION_BASE_FILTERBASE_HH_INCLUDED
#define ION_BASE_FILTERBASE_HH_INCLUDED

namespace ion {
namespace math {

	class FilterBase
	{
	protected:
		double m_Width;

		FilterBase(double newwidth):m_Width(newwidth) {}
	public:
		virtual ~FilterBase() {}

		double width() const			{ return m_Width; }
		void   width(double newwidth)	{ m_Width=newwidth; }

		virtual double filter(const double value) const=0;
	};

}
}

#endif
