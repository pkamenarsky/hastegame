#ifndef ION_OPENGLDRV_OGLVERTEXVBO_HH_INCLUDED
#define ION_OPENGLDRV_OGLVERTEXVBO_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/vertexformat.hh"
#include "../video/vertexiterator.hh"
#include "../video/vertexstream.hh"

namespace ion {
namespace opengldrv {

	class BufferObject;
	class OGLDevice;

	class ION_API OGLVertexVBO:public video::Vertexstream
	{
	public:
		OGLVertexVBO(OGLDevice& rOGLDevice,const ion_uint32 numVertices,
			const video::Vertexformat& format,const ion_uint32 flags,video::Vertexstream::Vertexsource *pSource=0);
		~OGLVertexVBO();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void unbind();
		void map(const ion_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

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
		void calculatePointers(video::VertexiteratorPointers& vitp);

		BufferObject *m_pBuffer;
		OGLDevice& m_rOGLDevice;
		bool m_IsDataOK;

		video::VertexiteratorPointers m_Pointers;
	};

}
}

#endif
