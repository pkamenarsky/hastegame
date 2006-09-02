#ifndef ION_VIDEO_VERTEXSTREAM_HH_INCLUDED
#define ION_VIDEO_VERTEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../math/vectors.hh"
#include "geometrystream.hh"
#include "vertexformat.hh"
#include "vertexiterator.hh"

namespace ion {
namespace video {

	class ION_API Vertexstream:public Geometrystream
	{
	public:
		class Vertexsource {
		public:
			virtual void start(Vertexstream* vertexstream)=0;
			virtual void refill(unsigned long& firstElement,unsigned long& numElements)=0;
			virtual void end()=0;
			virtual bool verticesLeft()=0;
			
			virtual ~Vertexsource() {}
		};

		virtual ~Vertexstream();
		const Vertexformat& vertexformat() const;

		inline const Vertexiterator& begin() const { return *m_pBegin; }
		inline const Vertexiterator& end() const { return *m_pEnd; }

		inline void vertexsource(Vertexsource* pSource) { m_pSource=pSource; }
		inline Vertexsource* vertexsource() { return m_pSource; }
		inline const Vertexsource* vertexsource() const { return m_pSource; }

		inline void transferFrom(const Vertexstream& from) { transferFrom(from,from.capacity()); }
		void transferFrom(const Vertexstream& from,const ion_uint32 numVertices);

		virtual const math::Vector3f& position(const ion_uint32 vtxindex) const=0;
		virtual const math::Vector3f& normal(const ion_uint32 vtxindex) const=0;
		virtual ion_uint32 diffuseColor(const ion_uint32 vtxindex) const=0;
		virtual ion_uint32 specularColor(const ion_uint32 vtxindex) const=0;
		virtual float texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const=0;
		virtual const math::Vector2f& texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const=0;
		virtual const math::Vector3f& texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const=0;

		virtual void position(const ion_uint32 vtxindex,const float x,const float y,const float z)=0;
		virtual void position(const ion_uint32 vtxindex,const math::Vector3f& newPosition)=0;
		virtual void normal(const ion_uint32 vtxindex,const float x,const float y,const float z)=0;
		virtual void normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal)=0;
		virtual void diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)=0;
		virtual void diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color)=0;
		virtual void specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)=0;
		virtual void specularColor(const ion_uint32 vtxindex,const ion_uint32 color)=0;
		virtual void texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D)=0;
		virtual void texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v)=0;
		virtual void texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)=0;
		virtual void texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w)=0;
		virtual void texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)=0;

		ion_uint32 num1DTexcoords() const;
		ion_uint32 num2DTexcoords() const;
		ion_uint32 num3DTexcoords() const;

	protected:
		Vertexstream(const Vertexformat& format,const ion_uint32 _capacity,Vertexsource *pSource);
		//virtual void calculatePointers(VertexiteratorPointers& vitp)=0;

		Vertexiterator *m_pBegin,*m_pEnd;
		Vertexformat m_Format;
		Vertexsource *m_pSource;

		// These are used by derived classes for texcoord pointers

		void initTexcoords();

		float **m_pp1DTexcoords;
		math::Vector2f **m_pp2DTexcoords;
		math::Vector3f **m_pp3DTexcoords;
		ion_uint32 m_Num1DTexcoords,m_Num2DTexcoords,m_Num3DTexcoords;
	};

}
}

#endif
