#include "meshconv.hh"




namespace ion {
namespace math {

	struct Tempvtx {
		unsigned long m_XYZ,m_Normal,m_Texcoord2D,m_Color;

		inline void getFrom(const Meshconv::InTriangle& intr,const int num)
		{
			m_XYZ=intr.m_XYZ[num];
			m_Normal=intr.m_Normals[num];
			m_Texcoord2D=intr.m_Texcoords2D[num];
			m_Color=intr.m_Colors[num];
		}
	};

	bool operator == (const Tempvtx &a,const Tempvtx &b)
	{
		return (a.m_XYZ==b.m_XYZ) && (a.m_Normal==b.m_Normal) && (a.m_Texcoord2D==b.m_Texcoord2D) && (a.m_Color==b.m_Color);
	}




	void Meshconv::OutVertex::clone(const OutVertex& src)
	{
		m_XYZ=src.m_XYZ;
		m_Normal=src.m_Normal;
		m_Texcoord=src.m_Texcoord;
		m_Color=src.m_Color;
	}


	Meshconv::Meshconv(const bool texcoords,const bool colors):m_Texcoords2DUsed(texcoords),m_ColorsUsed(colors)
	{
	}

	Meshconv::~Meshconv()
	{
	}


	void Meshconv::addTriangle(const InTriangle& triangle)
	{
		m_InTriangles.push_back(triangle);
	}


	void Meshconv::addXYZVtx(const InVtx& xyz)
	{
		m_Vertices.push_back(xyz);
	}

	void Meshconv::addNormal(const InNormal& normal)
	{
		m_Normals.push_back(normal);
	}

	void Meshconv::addTexcoord2D(const InTexcoord2D& tex2d)
	{
		m_Texcoords2D.push_back(tex2d);
	}

	void Meshconv::addColor(const InColor& color)
	{
		m_Colors.push_back(color);
	}

	void Meshconv::calculateNormals()
	{
	}

	void Meshconv::calculate()
	{
		std::vector <Tempvtx> tempverts;

		std::vector< InTriangle >::iterator intr=m_InTriangles.begin();

		OutTriangle outtr;
		for (;intr!=m_InTriangles.end();++intr) {

			for (int i=0;i<3;++i) {
				Tempvtx vtx;
				vtx.getFrom(*intr,i);
				if (!m_Texcoords2DUsed) vtx.m_Texcoord2D=0;
				if (!m_ColorsUsed) vtx.m_Color=0;

				std::vector< Tempvtx >::iterator intr2=std::find(tempverts.begin(),tempverts.end(),vtx);

				unsigned long newidx;
				if (intr2==tempverts.end()) {
					newidx=(unsigned long)(tempverts.size());
					tempverts.push_back(vtx);
				} else {
					newidx=(unsigned long)(intr2-tempverts.begin());
				}

				outtr.m_Indices[i]=newidx;
			}

			m_OutTriangles.push_back(outtr);
		}

		std::vector< Tempvtx >::iterator intvtx=tempverts.begin();
		for (;intvtx!=tempverts.end();++intvtx) {
			OutVertex vtx;

			if ((*intvtx).m_XYZ >= m_Vertices.size()) continue;
			vtx.m_XYZ=m_Vertices[(*intvtx).m_XYZ];

			if ((*intvtx).m_Texcoord2D < m_Texcoords2D.size())
				vtx.m_Texcoord=m_Texcoords2D[(*intvtx).m_Texcoord2D];

			if ((*intvtx).m_Normal < m_Normals.size())
				vtx.m_Normal=m_Normals[(*intvtx).m_Normal];

			if ((*intvtx).m_Color < m_Colors.size())
				vtx.m_Color=m_Colors[(*intvtx).m_Color];

			m_OutVertices.push_back(vtx);
		}

	}

	unsigned long Meshconv::numOutTriangles() const
	{
		return (unsigned long)(m_OutTriangles.size());
	}


	const Meshconv::OutTriangle& Meshconv::outTriangle(const unsigned long index) const
	{
		return m_OutTriangles[index];
	}


	unsigned long Meshconv::numOutVertices() const
	{
		return (unsigned long)(m_OutVertices.size());
	}


	const Meshconv::OutVertex & Meshconv::outVertex(const unsigned long index) const
	{
		return m_OutVertices[index];
	}
}
}
