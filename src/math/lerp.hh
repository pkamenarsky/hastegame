#ifndef ION_MATH_LERP_HH_INCLUDED
#define ION_MATH_LERP_HH_INCLUDED

#include "interpolator.hh"

namespace ion {
namespace math {

	class ION_API LinearInterpolator:public Interpolator
	{
	public:
		LinearInterpolator();
		~LinearInterpolator();

		void calculateInterpolations(const float position);
	};

}
}

#endif
