#ifndef ION_MATH_INTERPOLATOR_HH_INCLUDED
#define ION_MATH_INTERPOLATOR_HH_INCLUDED

#include <vector>

namespace ion {
namespace math {

	template <class T>
	void subdivideQuadraticBezier(const float start,const float end,const T* controlPoints,
	const int tesselateLevel,const int maxTesselateLevel,typename std::vector < T >& points)
	{
		float range=(end-start);
		float halfrange=range*0.5f;

		float t=start+halfrange;
		float invt=1.0f-t;

		float b[3]={invt*invt,2.0f*t*invt,t*t};

		T point=controlPoints[0]*b[0]+controlPoints[1]*b[1]+controlPoints[2]*b[2];

		if (tesselateLevel<maxTesselateLevel)
			subdivideQuadraticBezier(start,start+halfrange,controlPoints,tesselateLevel+1,maxTesselateLevel,points);

		points.push_back(point);

		if (tesselateLevel<maxTesselateLevel)
			subdivideQuadraticBezier(start+halfrange,end,controlPoints,tesselateLevel+1,maxTesselateLevel,points);
	}

	template <class T>
	typename std::vector < T > calculateQuadraticBezier(const T* controlPoints,const int numControlPoints,const int maxTesselateLevel)
	{
		std::vector < T > points;
		int cnt=(numControlPoints-1)/2;

		for (int i=0;i<cnt;++i) {
			int ofs=i*2;
			points.push_back(controlPoints[ofs]);
			subdivideQuadraticBezier(0,1,&controlPoints[ofs],0,maxTesselateLevel,points);
		}

		points.push_back(controlPoints[numControlPoints-1]);

		return points;
	}

}
}

#endif
