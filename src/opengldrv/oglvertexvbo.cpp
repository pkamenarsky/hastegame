#include "bufobject.hh"
#include "ogldevice.hh"
#include "oglvertexvbo.hh"
#include "../video/vertexformat.hh"

namespace ion {
namespace opengldrv {

	OGLVertexVBO::OGLVertexVBO(OGLDevice& rOGLDevice,const ion_uint32 numVertices,
	const video::Vertexformat& format,const ion_uint32 flags,video::Vertexstream::Vertexsource *pSource):
	Vertexstream(format,numVertices,pSource),
	m_pBuffer(new BufferObject(GL_ARRAY_BUFFER_ARB)),m_rOGLDevice(rOGLDevice),m_IsDataOK(true)
	{
		GLenum usage;
		
		if (flags& video::Streamflags_Dynamic) {
			usage=GL_DYNAMIC_DRAW_ARB;
		} else usage=GL_STATIC_DRAW_ARB;

		m_pBuffer->bind();
		m_pBuffer->create(static_cast<GLsizeiptrARB>(m_Format.stride()*numVertices),0,usage);

		m_Pointers.m_CurrentVtx=0;
		m_Pointers.m_pp1DTexcoords=m_pp1DTexcoords;
		m_Pointers.m_pp2DTexcoords=m_pp2DTexcoords;
		m_Pointers.m_pp3DTexcoords=m_pp3DTexcoords;
	}

	OGLVertexVBO::~OGLVertexVBO()
	{
		delete m_pBuffer;
	}

	bool OGLVertexVBO::isValid() const
	{
		return (m_pBuffer!=0);
	}

	bool OGLVertexVBO::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLVertexVBO::dataIsOK() { m_IsDataOK=true; }

	void *OGLVertexVBO::mappedPointer()
	{
		return m_pBuffer->mappedPointer();
	}

	bool OGLVertexVBO::isMapped() const
	{
		return m_pBuffer->mappedPointer()!=0;
	}

	void OGLVertexVBO::bind()
	{
		// IMPORTANT: FIRST bind the vbo, THEN set the pointers!
		m_pBuffer->bind();

		ion_uint8* offset=0;
		int numtexcoord=0;
		int texsize=0;

		bool isNormalEnabled=false,isColorEnabled=false;

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
					isNormalEnabled=true;
					break;
				case video::VertexFormatEntry_Diffuse:
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4,GL_UNSIGNED_BYTE,m_Format.stride(),(const GLvoid*)offset);
					isColorEnabled=true;
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
				case video::VertexFormatEntry_Boneweight:break; // TODO: Support for this
			}
			offset+=video::vertexFormatEntrySizeLookup(vfentry);
		}

		if (glIsEnabled(GL_NORMAL_ARRAY) && !isNormalEnabled)
			glDisableClientState(GL_NORMAL_ARRAY);

		if (glIsEnabled(GL_COLOR_ARRAY) && !isColorEnabled) {
			glDisableClientState(GL_COLOR_ARRAY);
			glColor3f(1,1,1);
		}

		for (int stagenr=numtexcoord;stagenr!=m_rOGLDevice.caps().m_MaxTextureBlendStages;++stagenr) {
			glActiveTextureARB(GL_TEXTURE0_ARB+stagenr);
			glClientActiveTextureARB(GL_TEXTURE0_ARB+stagenr);

			if (glIsEnabled(GL_TEXTURE_COORD_ARRAY)) {
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}

	}

	void OGLVertexVBO::unbind()
	{
		int numtexcoord=0;
		int texsize=0;

		for (ion_uint32 entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry vfentry=m_Format.entry(entrynr);

			texsize=0;
			switch (vfentry) {
				case video::VertexFormatEntry_Position:
					glDisableClientState(GL_VERTEX_ARRAY);
					break;
				case video::VertexFormatEntry_Normal:
					glDisableClientState(GL_NORMAL_ARRAY);
					break;
				case video::VertexFormatEntry_Diffuse:
					glDisableClientState(GL_COLOR_ARRAY);
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
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					++numtexcoord;
					break;
				case video::VertexFormatEntry_Boneweight:break; // TODO: Support for this
			}
		}

		for (int stagenr=numtexcoord;stagenr!=m_rOGLDevice.caps().m_MaxTextureBlendStages;++stagenr) {
			glActiveTextureARB(GL_TEXTURE0_ARB+stagenr);
			glClientActiveTextureARB(GL_TEXTURE0_ARB+stagenr);

			if (glIsEnabled(GL_TEXTURE_COORD_ARRAY)) {
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}

		m_pBuffer->disableVBO(GL_ARRAY_BUFFER_ARB);
	}

	void OGLVertexVBO::map(const ion_uint32 flags)
	{
		GLenum access=GL_READ_WRITE_ARB;

	/*	enum Mapflags {
			Map_Writeonly=1,
			Map_Readonly=2,
			Map_Discard=4,
			Map_Nooverwrite=8
		};
		
			@param access Access mode. Valid values:\n
		\n
		<b>GL_READ_ONLY_ARB</b>\n
		Read-only access.\n
		\n
		<b>GL_WRITE_ONLY_ARB</b>\n
		Write-only access.\n
		\n
		<b>GL_READ_WRITE_ARB</b>\n
		Read-write access.\n*/

		if (flags&video::Map_Readonly) {
			access=GL_READ_ONLY_ARB;
		} else if (flags&video::Map_Writeonly) {
			access=GL_WRITE_ONLY_ARB;
		}

		m_pBuffer->bind();
		m_pBuffer->map(access);

		calculatePointers(m_Pointers);
	}

	void OGLVertexVBO::unmap()
	{
		m_pBuffer->bind();
		m_pBuffer->unmap();
	}

	void OGLVertexVBO::calculatePointers(video::VertexiteratorPointers& vitp)
	{
		if ((vitp.m_CurrentVtx>=capacity()) || !isMapped()) return;

		ion_uint8* pPtr=((ion_uint8*)m_pBuffer->mappedPointer())+m_Format.stride()*vitp.m_CurrentVtx;
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

	const math::Vector3f& OGLVertexVBO::position(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	const math::Vector3f& OGLVertexVBO::normal(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	ion_uint32 OGLVertexVBO::diffuseColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	ion_uint32 OGLVertexVBO::specularColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	float OGLVertexVBO::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((float*)pPtr);
	}

	const math::Vector2f& OGLVertexVBO::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector2f*)pPtr);
	}

	const math::Vector3f& OGLVertexVBO::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	void OGLVertexVBO::position(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		/*((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;*/
		((float*)pPtr)[0]=x;
		((float*)pPtr)[1]=y;
		((float*)pPtr)[2]=z;
	}

	void OGLVertexVBO::position(const ion_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		/*((math::Vector3f*)pPtr)->x()=newPosition.x();
		((math::Vector3f*)pPtr)->y()=newPosition.y();
		((math::Vector3f*)pPtr)->z()=newPosition.z();*/
		((float*)pPtr)[0]=newPosition.x();
		((float*)pPtr)[1]=newPosition.y();
		((float*)pPtr)[2]=newPosition.z();
	}

	void OGLVertexVBO::normal(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		/*((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;*/
		((float*)pPtr)[0]=x;
		((float*)pPtr)[1]=y;
		((float*)pPtr)[2]=z;
	}

	void OGLVertexVBO::normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		/*((math::Vector3f*)pPtr)->x()=newNormal.x();
		((math::Vector3f*)pPtr)->y()=newNormal.y();
		((math::Vector3f*)pPtr)->z()=newNormal.z();*/
		((float*)pPtr)[0]=newNormal.x();
		((float*)pPtr)[1]=newNormal.y();
		((float*)pPtr)[2]=newNormal.z();
	}

	void OGLVertexVBO::diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)b)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)r);
	}

	void OGLVertexVBO::diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ion_uint32*)pPtr)=color;
	}

	void OGLVertexVBO::specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)b)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)r);
	}

	void OGLVertexVBO::specularColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ion_uint32*)pPtr)=color;
	}

	void OGLVertexVBO::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		*((float*)pPtr)=newTexcoord1D;
	}

	void OGLVertexVBO::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
	}

	void OGLVertexVBO::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord2D.x();
		((float*)pPtr)[1]=newTexcoord2D.y();
	}

	void OGLVertexVBO::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
		((float*)pPtr)[2]=w;
	}

	void OGLVertexVBO::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord3D.x();
		((float*)pPtr)[1]=newTexcoord3D.y();
		((float*)pPtr)[2]=newTexcoord3D.z();
	}

}
}
