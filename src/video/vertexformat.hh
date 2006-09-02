#ifndef ION_VIDEO_VERTEXFORMAT_HH_INCLUDED
#define ION_VIDEO_VERTEXFORMAT_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include <vector>

namespace ion {
namespace video {

	/*
	NOTE: The vertex format entry order in the Vertexformat class isn't strictly followed.
	For example, a normal vector can actually be located behind the texture coords, even if it is before
	these in the Vertexformat class.
	*/

	enum VertexFormatEntry {
		VertexFormatEntry_Position=0,
		VertexFormatEntry_Normal=1,
		VertexFormatEntry_Diffuse=2,
		VertexFormatEntry_Specular=3,
		VertexFormatEntry_Texcoord1D=4,
		VertexFormatEntry_Texcoord2D=5,
		VertexFormatEntry_Texcoord3D=6,
		VertexFormatEntry_Boneweight=7
	};

	enum VertexFormatSemantic {
		VertexFormatSemantic_None=0,
		VertexFormatSemantic_Position=1,
		VertexFormatSemantic_Tangent=2,
		VertexFormatSemantic_Binormal=3,
		VertexFormatSemantic_Normal=4,
		VertexFormatSemantic_Texcoord=5,
		VertexFormatSemantic_SHCoefficients=6
	};

	ION_API unsigned long vertexFormatEntrySizeLookup(const VertexFormatEntry entry);

	class ION_API Vertexformat
	{
	public:
		void addEntry(const VertexFormatEntry entry,const ion_uint32 semantic);
		VertexFormatEntry entry(unsigned long index) const;
		ion_uint32 entrySemantic(unsigned long index) const;
		unsigned long numEntries() const;

		Vertexformat();
		Vertexformat(const Vertexformat& src);
		virtual ~Vertexformat();

		void operator =(const Vertexformat& src);
		void clone(const Vertexformat& src);

		bool contains(const VertexFormatEntry entry,const unsigned long searchStart,const unsigned long index) const;
		bool contains(const VertexFormatEntry entry,const unsigned long searchStart,const ion_uint32 semantic,
			const unsigned long index) const;
		ion_uint32 find(const VertexFormatEntry entry,const unsigned long searchStart,const ion_uint32 semantic,
			const unsigned long index) const;

		unsigned long stride() const;

	private:
		struct Entry {
			VertexFormatEntry m_Type;
			ion_uint32 m_Semantic;
		};

		mutable unsigned long m_Stride;
		::std::vector <Entry> *m_pVertexFormat;
	};

	bool operator ==(const Vertexformat& v1,const Vertexformat& v2);

}
}

#endif
