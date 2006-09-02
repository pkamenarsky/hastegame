#include "bufobject.hh"
#include "ogldevice.hh"
#include "oglindexvbo.hh"

namespace ion {
namespace opengldrv {

	// BUG: 16 bit indices don't work on some systems - either a bug in the code or an OpenGL issue#
	// as a workaround, 32 bits are used always, no matter what is specified as format

	OGLIndexVBO::OGLIndexVBO(OGLDevice& rOGLDevice,const ion_uint32 numIndices,
	const video::Indexformat format,const ion_uint32 flags):
	video::Indexstream(video::Indexformat_32bit,numIndices),
	m_pBuffer(new BufferObject(GL_ELEMENT_ARRAY_BUFFER_ARB)),
	m_rOGLDevice(rOGLDevice),m_IsDataOK(true)
	{
		GLenum usage;
		
		if (flags&video::Streamflags_Dynamic) {
			usage=GL_DYNAMIC_DRAW_ARB;
		} else usage=GL_STATIC_DRAW_ARB;

		m_pBuffer->bind();
		m_pBuffer->create(static_cast<GLsizeiptrARB>(indexformatSizeLookup(video::Indexformat_32bit)*numIndices),0,usage);
	}

	OGLIndexVBO::~OGLIndexVBO()
	{
		delete m_pBuffer;
	}

	bool OGLIndexVBO::isValid() const
	{
		return (m_pBuffer!=0);
	}

	bool OGLIndexVBO::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLIndexVBO::dataIsOK() { m_IsDataOK=true; }

	void *OGLIndexVBO::mappedPointer()
	{
		return m_pBuffer->mappedPointer();
	}

	bool OGLIndexVBO::isMapped() const
	{
		return m_pBuffer->mappedPointer()!=0;
	}

	void OGLIndexVBO::bind()
	{
		m_pBuffer->bind();
	}

	void OGLIndexVBO::map(const ion_uint32 flags)
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
	}

	void OGLIndexVBO::unmap()
	{
		m_pBuffer->bind();
		m_pBuffer->unmap();
	}

	ion_uint32 OGLIndexVBO::index(const ion_uint32 position)
	{
		if (position<capacity()) {
			switch (m_Format) {
				case video::Indexformat_16bit:return (ion_uint32)( ((ion_uint16*)(m_pBuffer->mappedPointer()))[position] );
				case video::Indexformat_32bit:return ((ion_uint32*)(m_pBuffer->mappedPointer()))[position];
			}
		}

		return 0xffffffff;
	}

	void OGLIndexVBO::index(const ion_uint32 position,const ion_uint32 newindex)
	{
		if (position<capacity()) {
			switch (m_Format) {
				case video::Indexformat_16bit:((ion_uint16*)(m_pBuffer->mappedPointer()))[position]=(ion_uint16)(newindex&0xffff);
				case video::Indexformat_32bit:((ion_uint32*)(m_pBuffer->mappedPointer()))[position]=newindex;
			}
		}
	}

}
}
