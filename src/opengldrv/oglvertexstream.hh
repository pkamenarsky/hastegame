#ifndef ION_OPENGLDRV_OGLVERTEXSTREAM_HH_INCLUDED
#define ION_OPENGLDRV_OGLVERTEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/vertexformat.hh"
#include "../video/vertexiterator.hh"
#include "../video/vertexstream.hh"
#include "../video/memvertexstream.hh"
#include "oglverteximmediate.hh"
#include "oglvertexva.hh"
#include "oglvertexvbo.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGLVertexstream:public video::Vertexstream
	{
	public:
		OGLVertexstream(OGLDevice& rOGLDevice,const ion_uint32 numVertices,
			const video::Vertexformat& format,const ion_uint32 flags,video::Vertexstream::Vertexsource *pSource=0);
		~OGLVertexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ion_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		void restoreData();

		void switchStorage(const int type);

		const math::Vector3f& position(const ion_uint32 vtxindex) const;
		const math::Vector3f& normal(const ion_uint32 vtxindex) const;
		ion_uint32 diffuseColor(const ion_uint32 vtxindex) const;
		ion_uint32 specularColor(const ion_uint32 vtxindex) const;
		float texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const;
		const math::Vector2f& texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const;
		const math::Vector3f& texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const;

		void position(const ion_uint32 vtxindex,const float x,const float y,const float z);
		void position(const ion_uint32 vtxindex,const math::Vector3f& newPosition);
		void normal(const ion_uint32 vtxindex,const float x,const float y,const float z);
		void normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal);
		void diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b);
		void diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color);
		void specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b);
		void specularColor(const ion_uint32 vtxindex,const ion_uint32 color);
		void texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D);
		void texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v);
		void texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D);
		void texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w);
		void texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D);

	protected:
		video::MemVertexstream *m_pMemVertexstream;
		video::Vertexstream *m_pCurrentBackend;
		OGLVertexVBO *m_pOGLVertexVBO;
		OGLVertexVA *m_pOGLVertexVA;
		OGLVertexImmediate *m_pOGLVertexImmediate;
		ion_uint32 m_Flags;
		OGLDevice& m_rOGLDevice;

	};

}
}

#endif
