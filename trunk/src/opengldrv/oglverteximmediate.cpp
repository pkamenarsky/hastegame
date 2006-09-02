#include "ogldevice.hh"
#include "OGLVertexImmediate.hh"
#include "../video/vertexformat.hh"
#include "../video/coreenums.hh"

namespace ion {
namespace opengldrv {

	OGLVertexImmediate::OGLVertexImmediate(OGLDevice& rOGLDevice,const ion_uint32 numVertices,
	const video::Vertexformat& format,const ion_uint32 flags,video::Vertexstream::Vertexsource *pSource):
	Vertexstream(format,numVertices,pSource),m_rOGLDevice(rOGLDevice),m_IsDataOK(true)
	{
		m_pVertices=new ion_uint8[m_Format.stride()*numVertices];

		m_Pointers.m_CurrentVtx=0;
		m_Pointers.m_pp1DTexcoords=m_pp1DTexcoords;
		m_Pointers.m_pp2DTexcoords=m_pp2DTexcoords;
		m_Pointers.m_pp3DTexcoords=m_pp3DTexcoords;
	}

	OGLVertexImmediate::~OGLVertexImmediate()
	{
		delete m_pVertices;
	}

	bool OGLVertexImmediate::isValid() const
	{
		return (m_pVertices!=0);
	}

	bool OGLVertexImmediate::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLVertexImmediate::dataIsOK() { m_IsDataOK=true; }

	void *OGLVertexImmediate::mappedPointer()
	{
		return m_pVertices;
	}

	bool OGLVertexImmediate::isMapped() const
	{
		return m_pVertices!=0;
	}

	void OGLVertexImmediate::bind()
	{
		/*ion_uint8* offset=m_pVertices;
		int numtexcoord=0;
		int texsize=0;

		for (ion_uint32 entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry vfentry=m_Format.entry(entrynr);

			texsize=0;
			switch (vfentry) {
				case video::VertexFormatEntry_Position:
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3,GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Normal:
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Diffuse:
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4,GL_UNSIGNED_BYTE,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Specular:continue;
					break;
				case video::VertexFormatEntry_Texcoord1D:
					texsize=1;
				case video::VertexFormatEntry_Texcoord2D:
					if (texsize==0) texsize=2;
				case video::VertexFormatEntry_Texcoord3D:
					if (texsize==0) texsize=3;
					glActiveTextureARB(GL_TEXTURE0_ARB+numtexcoord);
					glClientActiveTextureARB(GL_TEXTURE0_ARB+numtexcoord);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texsize,GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					++numtexcoord;
					break;
				case video::VertexFormatEntry_Boneweight:break;
			}
			offset+=video::vertexFormatEntrySizeLookup(vfentry);
		}*/
	}

	void OGLVertexImmediate::map(const ion_uint32 flags)
	{
		calculatePointers(m_Pointers);
	}

	void OGLVertexImmediate::unmap()
	{
	}

	void OGLVertexImmediate::calculatePointers(video::VertexiteratorPointers& vitp)
	{
		if ((vitp.m_CurrentVtx>=capacity()) || !isMapped()) return;

		ion_uint8* pPtr=m_pVertices+m_Format.stride()*vitp.m_CurrentVtx;
		int numtexcoords[3]={0,0,0};
		for (unsigned long entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry entry=m_Format.entry(entrynr);
			switch (entry) {
				case video::VertexFormatEntry_Position:vitp.m_pPosition=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Normal:vitp.m_pNormal=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Texcoord1D:vitp.m_pp1DTexcoords[numtexcoords[0]++]=(float*)pPtr; break;
				case video::VertexFormatEntry_Texcoord2D:vitp.m_pp2DTexcoords[numtexcoords[1]++]=(math::Vector2f*)pPtr; break;
				case video::VertexFormatEntry_Texcoord3D:vitp.m_pp3DTexcoords[numtexcoords[2]++]=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Diffuse:vitp.m_pDiffuseColor=(ion_uint32*)pPtr;
				case video::VertexFormatEntry_Specular:vitp.m_pSpecularColor=(ion_uint32*)pPtr;
				default:break;
			}
			pPtr+=video::vertexFormatEntrySizeLookup(entry);
		}
	}

	const math::Vector3f& OGLVertexImmediate::position(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	const math::Vector3f& OGLVertexImmediate::normal(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	ion_uint32 OGLVertexImmediate::diffuseColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	ion_uint32 OGLVertexImmediate::specularColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	float OGLVertexImmediate::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((float*)pPtr);
	}

	const math::Vector2f& OGLVertexImmediate::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector2f*)pPtr);
	}

	const math::Vector3f& OGLVertexImmediate::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	void OGLVertexImmediate::position(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void OGLVertexImmediate::position(const ion_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newPosition.x();
		((math::Vector3f*)pPtr)->y()=newPosition.y();
		((math::Vector3f*)pPtr)->z()=newPosition.z();
	}

	void OGLVertexImmediate::normal(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void OGLVertexImmediate::normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newNormal.x();
		((math::Vector3f*)pPtr)->y()=newNormal.y();
		((math::Vector3f*)pPtr)->z()=newNormal.z();
	}

	void OGLVertexImmediate::diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)b)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)r);
	}

	void OGLVertexImmediate::diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ion_uint32*)pPtr)=color;
	}

	void OGLVertexImmediate::specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)b)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)r);
	}

	void OGLVertexImmediate::specularColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ion_uint32*)pPtr)=color;
	}

	void OGLVertexImmediate::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		*((float*)pPtr)=newTexcoord1D;
	}

	void OGLVertexImmediate::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
	}

	void OGLVertexImmediate::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord2D.x();
		((float*)pPtr)[1]=newTexcoord2D.y();
	}

	void OGLVertexImmediate::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
		((float*)pPtr)[2]=w;
	}

	void OGLVertexImmediate::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord3D.x();
		((float*)pPtr)[1]=newTexcoord3D.y();
		((float*)pPtr)[2]=newTexcoord3D.z();
	}

}
}
