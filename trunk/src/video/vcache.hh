#ifndef ION_VIDEO_VCACHE_HH_INCLUDED
#define ION_VIDEO_VCACHE_HH_INCLUDED

#include "../base/basememobj.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	class Vertexformat;
	class Vertexiterator;
	class Vertexstream;

	class ION_API VCache:public base::BaseMemObj
	{
	public:
		VCache(Videodevice& rVideodevice,const Vertexformat& format,const ion_uint32 numVertices);
		~VCache();

		Videodevice &videodevice() const;

		bool isValid() const;

		void begin(const PrimitivesType type);
		void pushVertex();
		void position(const float x,const float y,const float z);
		void position(const math::Vector3f& v);
		void normal(const float x,const float y,const float z);
		void normal(const math::Vector3f& newNormal);
		void diffuseColor(const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b);
		void diffuseColor(const ion_uint32 color);
		void specularColor(const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b);
		void specularColor(const ion_uint32 color);
		void texcoord1D(const ion_uint32 texstage,const float newTexcoord1D);
		void texcoord2D(const ion_uint32 texstage,const float u,const float v);
		void texcoord2D(const ion_uint32 texstage,const math::Vector2f& newTexcoord2D);
		void texcoord3D(const ion_uint32 texstage,const float u,const float v,const float w);
		void texcoord3D(const ion_uint32 texstage,const math::Vector3f& newTexcoord3D);
		void end();

	protected:
		void flush();

		PrimitivesType m_PrimitivesType;
		ion_uint32 m_NumSentVertices,m_Vertexposition;
		//Vertexiterator *m_pVertexiterator;
		Vertexstream *m_pVertexstream;
		Videodevice &m_rVideodevice;
	};

}
}

#endif
