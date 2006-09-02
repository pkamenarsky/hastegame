#ifndef ION_MATH_MESHCONV_HH_INCLUDED
#define ION_MATH_MESHCONV_HH_INCLUDED

#include <algorithm>
#include <vector>
#include "../base/dll.hh"

namespace ion {
namespace math {

	class Meshconv
	{
	public:

		struct ION_API InTriangle
		{
			unsigned long m_XYZ[3],m_Normals[3],m_Texcoords2D[3],m_Colors[3];

			//float m_Normal[3];
		};

		struct ION_API InVtx
		{
			float m_X,m_Y,m_Z;

			InVtx() {}
			InVtx(const float x,const float y,const float z):m_X(x),m_Y(y),m_Z(z) {}
			InVtx(const InVtx& src) { m_X=src.m_X; m_Y=src.m_Y; m_Z=src.m_Z; }
			InVtx& operator =(const InVtx& src) { m_X=src.m_X; m_Y=src.m_Y; m_Z=src.m_Z; return *this; }
		};

		struct ION_API InNormal
		{
			float m_X,m_Y,m_Z;

			InNormal() {}
			InNormal(const float x,const float y,const float z):m_X(x),m_Y(y),m_Z(z) {}
			InNormal(const InNormal& src) { m_X=src.m_X; m_Y=src.m_Y; m_Z=src.m_Z; }
			InNormal& operator =(const InNormal& src) { m_X=src.m_X; m_Y=src.m_Y; m_Z=src.m_Z; return *this; }
		};

		struct ION_API InTexcoord2D
		{
			float m_U,m_V;

			InTexcoord2D() {}
			InTexcoord2D(const float u,const float v):m_U(u),m_V(v) {}
			InTexcoord2D(const InTexcoord2D& src) { m_U=src.m_U; m_V=src.m_V; }
			InTexcoord2D& operator =(const InTexcoord2D& src) { m_U=src.m_U; m_V=src.m_V; return *this; }
		};

		struct ION_API InColor
		{
			float m_R,m_G,m_B,m_A;

			InColor() {}
			InColor(const float r,const float g,const float b,const float a):m_R(r),m_G(g),m_B(b),m_A(a) {}
			InColor(const InColor& src) { m_R=src.m_R; m_G=src.m_G; m_B=src.m_B; m_A=src.m_A; }
			InColor& operator =(const InColor& src) { m_R=src.m_R; m_G=src.m_G; m_B=src.m_B; m_A=src.m_A; return *this; }
		};

		struct ION_API OutTriangle
		{
			unsigned long m_Indices[3];
		};

		struct ION_API OutVertex
		{
			InVtx m_XYZ;
			InNormal m_Normal;
			InTexcoord2D m_Texcoord;
			InColor m_Color;

			OutVertex() {}
			OutVertex(const OutVertex& src) { clone(src); }
			OutVertex& operator = (const OutVertex& src) { clone(src); return *this; }

		protected:
			void clone(const OutVertex& src);
		};

		ION_API Meshconv(const bool texcoords,const bool colors);
		ION_API ~Meshconv();

		ION_API void addTriangle(const InTriangle& triangle);
		ION_API void addXYZVtx(const InVtx& xyz);
		ION_API void addNormal(const InNormal& normal);
		ION_API void addTexcoord2D(const InTexcoord2D& tex2d);
		ION_API void addColor(const InColor& color);

		ION_API void calculateNormals();
		ION_API void calculate();

		ION_API unsigned long numOutTriangles() const;
		ION_API const OutTriangle &outTriangle(const unsigned long index) const;
		ION_API unsigned long numOutVertices() const;
		ION_API const OutVertex & outVertex(const unsigned long index) const;

	protected:

		std::vector<InVtx> m_Vertices;
		std::vector<InNormal> m_Normals;
		std::vector<InTexcoord2D> m_Texcoords2D;
		std::vector<InColor> m_Colors;
		std::vector<InTriangle> m_InTriangles;
		std::vector<OutTriangle> m_OutTriangles;
		std::vector<OutVertex> m_OutVertices;
		bool m_Texcoords2DUsed,m_ColorsUsed;
	};


}
}

#endif
