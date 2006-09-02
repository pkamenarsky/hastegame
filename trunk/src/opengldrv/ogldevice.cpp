#include <string>
#include <sstream>
#include "../base/log.hh"
#include "oglindexstream.hh"
#include "ogldevice.hh"
#include "oglcubemap.hh"
#include "ogl2dtexture.hh"
#include "oglsurface.hh"
#include "oglvertexstream.hh"

#ifdef OPENGLDRV_USE_CG
#include <Cg/cgGL.h>

#include "cgoglvertexprogram.hh"
#include "cgoglfragmentprogram.hh"
#endif

namespace ion {
namespace opengldrv {
	
	bool OGLDevice::extensionSupported(const std::string& extensionName) const
	{
		return glewIsSupported(extensionName.c_str())!=GL_FALSE;
	}

	void OGLDevice::initializeExtensions()
	{
		glewExperimental=GL_TRUE;
		glewInit();
	}

	void OGLDevice::deviceState(const ion_intptr state,const ion_intptr data)
	{
		switch (state) {
			case OGLDevState_Vertexstorage: {
				if (m_StorageMode==(int)data) return;
				std::set < OGLVertexstream* >::iterator itvstream=m_pVertexstreams->begin();
				for (;itvstream!=m_pVertexstreams->end();++itvstream) {
					OGLVertexstream* pVStream=(*itvstream);
					pVStream->switchStorage((int)data);
				}
				std::set < OGLIndexstream* >::iterator itistream=m_pIndexstreams->begin();
				for (;itistream!=m_pIndexstreams->end();++itistream) {
					OGLIndexstream* pIStream=(*itistream);
					pIStream->switchStorage((int)data);
				}
				m_StorageMode=(int)data;
			} break;
			default:break;
		}
	}

	int OGLDevice::vertexStorageMode() const
	{
		return m_StorageMode;
	}

	void OGLDevice::removeVertexstreamFromList(OGLVertexstream& rStream)
	{
		std::set < OGLVertexstream* >::iterator it=m_pVertexstreams->find(&rStream);
		if (it!=m_pVertexstreams->end()) m_pVertexstreams->erase(it);
	}

	void OGLDevice::removeIndexstreamFromList(OGLIndexstream& rStream)
	{
		std::set < OGLIndexstream* >::iterator it=m_pIndexstreams->find(&rStream);
		if (it!=m_pIndexstreams->end()) m_pIndexstreams->erase(it);
	}






	OGLDevice::OGLDevice():m_pCurrentVertexProgram(0),m_pCurrentFragmentProgram(0),
	m_pColorRendertarget(0),m_pDepthRendertarget(0),m_StorageMode(OGLVertexStorage_VBO),
	m_IsValid(true),m_Quitting(false),m_CursorShown(true),
	m_WindowActivationState(video::NoWindowActivationChange),m_pOGLInternaldata(0)
	{
		m_pVertexstreams=new std::set < OGLVertexstream* >;
		m_pIndexstreams=new std::set < OGLIndexstream* >;

		m_SamplerstageparametersInt=0;
		m_SamplerstageparametersFloat=0;
		m_TexstageparametersInt=0;
		m_TexstageparametersFloat=0;
		m_pBoundTextures=0;
	}

	OGLDevice::~OGLDevice()
	{
		rendercolortarget(0);
		renderdepthstenciltarget(0);

#ifdef OPENGLDRV_USE_CG
		cgDestroyContext(m_CgContext);
#endif

		showCursor(true);
		shutdownGL();

		if (m_pBoundTextures) delete [] m_pBoundTextures;
		if (m_SamplerstageparametersInt) delete [] m_SamplerstageparametersInt;
		if (m_SamplerstageparametersFloat) delete [] m_SamplerstageparametersFloat;
		if (m_TexstageparametersInt) delete [] m_TexstageparametersInt;
		if (m_TexstageparametersFloat) delete [] m_TexstageparametersFloat;

		if (m_pVertexstreams) delete m_pVertexstreams;
		if (m_pIndexstreams) delete m_pIndexstreams;
	}

	bool OGLDevice::isValid() const
	{
		return m_IsValid;
	}

	//*********** INITIALIZATION / ENUMERATION

	const video::Caps& OGLDevice::caps() const
	{
		return m_Caps;
	}

	ion_uint32 OGLDevice::numVideoadapters() const
	{
		return 0;
	}

	ion_uint32 OGLDevice::numVideomodes(const ion_uint32 adapterindex,const video::Pixelformat pixelformat) const
	{
		return 0;
	}

	bool OGLDevice::fetchVideomode(const ion_uint32 adapter,const ion_uint32 modeindex,
		video::Videomode& mode,const video::Pixelformat pixelformat) const
	{
		return false;
	}

	void OGLDevice::init(const ion_uint32 width,const ion_uint32 height,
			const base::String& title,const bool fullscreen,void* displayhandle,
			const ion_uint32 adapter,const video::Pixelformat colorbufferformat,
			const video::Pixelformat depthstencilformat,const ion_uint32 Hz,const bool vsync)
	{
		initGL(width,height,title,fullscreen,displayhandle,adapter,colorbufferformat,depthstencilformat,Hz,vsync);

		initializeExtensions();

		glViewport(0,0,(GLint)width,(GLint)height);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glFogi(GL_FOG_MODE, GL_LINEAR);

		glFrontFace(GL_CCW);

#ifdef OPENGLDRV_USE_CG
		base::log("OGLDevice::init()",base::Message) << "Initializing Cg\n";
		{
			m_CgContext=cgCreateContext();

			m_CgVertexProfile=cgGLGetLatestProfile(CG_GL_VERTEX);
			m_CgFragmentProfile=cgGLGetLatestProfile(CG_GL_FRAGMENT);
			cgGLSetOptimalOptions(m_CgVertexProfile);
			cgGLSetOptimalOptions(m_CgFragmentProfile);

			base::log("OGLDevice::init()",base::Message) << "Used CG profiles:\n";
			base::logstream() << cgGetProfileString(m_CgVertexProfile) << "\n";
			base::logstream() << cgGetProfileString(m_CgFragmentProfile) << "\n";
		}
#endif
		if (extensionSupported("GL_EXT_texture_filter_anisotropic")) {
			GLint maxanisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&maxanisotropy);
			m_Caps.m_MaxAnisotropy=static_cast<ion_uint32>(maxanisotropy);
		}

		m_Caps.m_NPOTTextures=m_Caps.m_RectangularTextures=extensionSupported("GL_ARB_texture_non_power_of_two");

		m_Caps.m_Cubemaps=extensionSupported("GL_ARB_texture_cube_map");
		m_Caps.m_CompressedTextures=extensionSupported("GL_ARB_texture_compression");
		m_Caps.m_HWOcclusionQueries=extensionSupported("GL_ARB_occlusion_query") ||
			extensionSupported("GL_NV_occlusion_query");
		m_Caps.m_Rendertargets=extensionSupported("GL_EXT_framebuffer_object");

		m_Caps.m_MaxActiveFFLights=fixedMaxLights();
		m_Caps.m_MaxTextureBlendStages=fixedMaxTextureStages();

		{
			m_pBoundTextures=new OGLTexture*[m_Caps.m_MaxTextureBlendStages];
			for (ion_uint32 stage=0;stage<m_Caps.m_MaxTextureBlendStages;++stage)
				m_pBoundTextures[stage]=0;
		}

		float supportedGLslangVersion=0;

		if (extensionSupported("GL_ARB_vertex_program")) m_Caps.m_SupportedProgramFormats.addString("ogl_arbvp");
		if (extensionSupported("GL_ARB_fragment_program")) m_Caps.m_SupportedProgramFormats.addString("ogl_arbfp");
		if (extensionSupported("GL_ARB_shading_language_100")) {
			glGetError(); // in case the error flag is set, this resets it
			const char *pV=(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
			GLenum err=glGetError();
			if ((pV==0) && (err==GL_INVALID_ENUM)) pV="1.00";
			base::log("OGLDevice::init()",base::Message) << "Supported GLslang version: " << pV << "\n";

			{
				std::stringstream sstrv(pV);
				sstrv >> supportedGLslangVersion;
			}

			if (extensionSupported("GL_ARB_vertex_shader")) {
				if (supportedGLslangVersion>=1.00f) m_Caps.m_SupportedProgramFormats.addString("ogl_slang_vs_100");
			}
			if (extensionSupported("GL_ARB_fragment_shader")) {
				if (supportedGLslangVersion>=1.00f) m_Caps.m_SupportedProgramFormats.addString("ogl_slang_fs_100");
			}
		}

#ifdef OPENGLDRV_USE_CG
		m_Caps.m_SupportedProgramFormats.addString("cg_vprogram");
		m_Caps.m_SupportedProgramFormats.addString("cg_fprogram");
#endif

		m_SamplerstageparametersInt=new SamplerstageIntarray[m_Caps.m_MaxTextureBlendStages];
		m_SamplerstageparametersFloat=new SamplerstageFloatarray[m_Caps.m_MaxTextureBlendStages];
		m_TexstageparametersInt=new TexstageIntarray[m_Caps.m_MaxTextureBlendStages];
		m_TexstageparametersFloat=new TexstageFloatarray[m_Caps.m_MaxTextureBlendStages];

		for (ion_uint32 stage=0;stage<m_Caps.m_MaxTextureBlendStages;++stage) {
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_Minfilter]=video::Texfilter_Point;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_Mipfilter]=video::Texfilter_None;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_Magfilter]=video::Texfilter_Point;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_UWrap]=video::Texwrap_Repeat;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_VWrap]=video::Texwrap_Repeat;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_WWrap]=video::Texwrap_Repeat;
			m_SamplerstageparametersInt[stage][video::Samplerstageparameter_MaxAnisotropy]=0;

			m_TexstageparametersInt[stage][video::Texstageparameter_Colorop]=(stage==0) ? video::Texstageoperation_Modulate1x : video::Texstageoperation_Disable;
			m_TexstageparametersInt[stage][video::Texstageparameter_Colorarg1]=video::Texstageargument_Texture;
			m_TexstageparametersInt[stage][video::Texstageparameter_Colorarg2]=video::Texstageargument_Current;
			m_TexstageparametersInt[stage][video::Texstageparameter_Alphaop]=(stage==0) ? video::Texstageoperation_Selectarg1 : video::Texstageoperation_Disable;
			m_TexstageparametersInt[stage][video::Texstageparameter_Alphaarg1]=video::Texstageargument_Texture;
			m_TexstageparametersInt[stage][video::Texstageparameter_Alphaarg2]=video::Texstageargument_Current;
			m_TexstageparametersInt[stage][video::Texstageparameter_Texcoordindex]=stage;
			m_TexstageparametersInt[stage][video::Texstageparameter_Textransformations]=video::Texcoordtransform_None;
			m_TexstageparametersInt[stage][video::Texstageparameter_Constant]=0;
			m_TexstageparametersInt[stage][video::Texstageparameter_Texcoordgeneration]=video::Texcoordgeneration_None;
		}

		m_Displaywidth=width;
		m_Displayheight=height;
		m_ColorbufferFormat=colorbufferformat;
		m_DepthstencilbufferFormat=depthstencilformat;

	}

	bool OGLDevice::testVertexProgram(const base::String& program,const base::String& format)
	{
		return false;
	}

	bool OGLDevice::testFragmentProgram(const base::String& program,const base::String& format)
	{
		return false;
	}

	ion_uint32 OGLDevice::displaywidth() const
	{
		return m_Displaywidth;
	}

	ion_uint32 OGLDevice::displayheight() const
	{
		return m_Displayheight;
	}

	video::Pixelformat OGLDevice::colorbufferFormat() const
	{
		return m_ColorbufferFormat;
	}

	video::Pixelformat OGLDevice::depthstencilbufferFormat() const
	{
		return m_DepthstencilbufferFormat;
	}





		//*********** FRAME START / END / UPDATE
	void OGLDevice::clear(const ion_uint32 buffers,const base::Float4& color,const float zvalue,
		const ion_uint32 stencilvalue)
	{
		GLbitfield mask=0;

		if (buffers & video::Clear_Colorbuffer) {
			glClearColor(color[1],color[2],color[3],color[0]);
			mask|=GL_COLOR_BUFFER_BIT;
		}
		if (buffers & video::Clear_Depthbuffer) {
			glClearDepth(zvalue);
			mask|=GL_DEPTH_BUFFER_BIT;

			zwrite(true);
		}
		if (buffers & video::Clear_Stencilbuffer) {
			glClearStencil((GLint)stencilvalue);
			mask|=GL_STENCIL_BUFFER_BIT;
		}

		glClear(mask);
	}

	void OGLDevice::begin()
	{
	}

	void OGLDevice::end()
	{
	}

	void OGLDevice::update()
	{
		m_WindowActivationState=video::NoWindowActivationChange;
		updateGL();
	}

	bool OGLDevice::quitting() const
	{
		return m_Quitting;
	}

	video::WindowActivationState OGLDevice::windowActivationState() const
	{
		return m_WindowActivationState;
	}

	void OGLDevice::setViewport(const ion_uint32 newx,const ion_uint32 newy,
		const ion_uint32 newwidth,const ion_uint32 newheight)
	{
		glViewport(newx,newy,newwidth,newheight);
	}

	bool OGLDevice::resizeFramebuffer(const ion_uint32 newwidth,const ion_uint32 newheight,video::ViewportResizingCallback* callback)
	{
		return true;
	}





	//*********** RESOURCE GENERATION

	video::Vertexstream* OGLDevice::createVertexstream(const ion_uint32 numVertices,const video::Vertexformat& format,
		const ion_uint32 flags,const video::Memorypool pool)
	{
		OGLVertexstream *pStream=new OGLVertexstream(*this,numVertices,format,flags);
		m_pVertexstreams->insert(pStream);
		return pStream;
	}

	video::Indexstream* OGLDevice::createIndexstream(const ion_uint32 numIndices,const video::Indexformat format,
		const ion_uint32 flags,const video::Memorypool pool)
	{
		OGLIndexstream *pStream=new OGLIndexstream(*this,numIndices,format,flags);
		m_pIndexstreams->insert(pStream);
		return pStream;
	}

	video::Texture2D* OGLDevice::create2DTexture(const base::String& identifier,
		const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,
		const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new OGL2DTexture(*this,identifier,width,height,levels,flags,format,pool);
	}

	video::Cubemap* OGLDevice::createCubemap(const base::String& identifier,
		const ion_uint32 edgelength,const ion_uint32 levels,
		const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new OGLCubemap(*this,identifier,edgelength,levels,flags,format,pool);
	}

	video::Occlusionquery* OGLDevice::createOcclusionquery()
	{
		return 0;
	}

	video::Program* OGLDevice::createVertexProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
#ifdef OPENGLDRV_USE_CG
		if (format=="cg_vprogram")
			return new CGOGLVertexprogram(*this,identifier,entrypoint,program);
		else 
#endif
			return 0;
	}

	video::Program* OGLDevice::createFragmentProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
#ifdef OPENGLDRV_USE_CG
		if (format=="cg_fprogram")
			return new CGOGLFragmentprogram(*this,identifier,entrypoint,program);
		else 
#endif
		return 0;
	}





	//*********** PRIMITIVES DRAWING

	void OGLDevice::drawPrimitives(const video::PrimitivesType type,
		video::Vertexstream& vertexstream,const ion_uint32 firstElement,
		const ion_uint32 numElements)
	{
		GLenum ptype=GL_POINTS;
		ion_uint32 vtxfactor=1;
		switch (type) {
			case video::Primitives_Points: vtxfactor=1; ptype=GL_POINTS; break;
			case video::Primitives_Linestrip: vtxfactor=1; ptype=GL_LINE_STRIP; break;
			case video::Primitives_Lines: vtxfactor=1; ptype=GL_LINES; break;
			case video::Primitives_Trianglestrip: vtxfactor=1; ptype=GL_TRIANGLE_STRIP; break;
			case video::Primitives_Trianglefan: vtxfactor=1; ptype=GL_TRIANGLE_FAN; break;
			case video::Primitives_Triangles: vtxfactor=3; ptype=GL_TRIANGLES; break;
		}

		vertexstream.bind();
		//glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);

		//glDrawArrays(ptype,firstElement,numElements*vtxfactor);

		switch (m_StorageMode) {
			case OGLVertexStorage_VBO:
				glDrawArrays(ptype,firstElement,numElements*vtxfactor);
				break;
			case OGLVertexStorage_VA:
				glDrawArrays(ptype,firstElement,numElements*vtxfactor);
				break;
			case OGLVertexStorage_Immediate: {

				glBegin(ptype);
				for (ion_uint32 vtxnr=0;vtxnr<vtxfactor*numElements;++vtxnr) {
					ion_uint32 idx=vtxnr+firstElement;

					const video::Vertexformat &vf=vertexstream.vertexformat();
					ion_uint32 stage1d=0,stage2d=0,stage3d=0,texstage=0;
					for (unsigned long vfentry=0;vfentry<vf.numEntries();++vfentry) {

						switch (vf.entry(vfentry)) {
							case video::VertexFormatEntry_Normal: {
								const math::Vector3f& n=vertexstream.normal(idx);
								glNormal3fv(n);
							} break;
							case video::VertexFormatEntry_Diffuse: {
								ion_uint32 color=vertexstream.diffuseColor(idx);
								float a=(float)(color>>24)/255.0f;
								float b=(float)((color>>16)&255)/255.0f;
								float g=(float)((color>>8)&255)/255.0f;
								float r=(float)(color&255)/255.0f;
								glColor4f(r,g,b,a);
							} break;
							case video::VertexFormatEntry_Texcoord1D:
								glMultiTexCoord1f(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord1D(idx,stage1d));
								++stage1d;
								++texstage;
								break;
							case video::VertexFormatEntry_Texcoord2D:
								glMultiTexCoord2fv(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord2D(idx,stage2d));
								++stage2d;
								++texstage;
								break;
							case video::VertexFormatEntry_Texcoord3D:
								glMultiTexCoord3fv(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord3D(idx,stage3d));
								++stage3d;
								++texstage;
								break;
						}
					}

					glVertex3f(
						vertexstream.position(idx).x(),
						vertexstream.position(idx).y(),
						vertexstream.position(idx).z());
				}
				glEnd();
			} break;
			default:break;
		}
	}

	void OGLDevice::drawIndexedPrimitives(
		const video::PrimitivesType type,
		video::Vertexstream& vertexstream,
		video::Indexstream& indexstream,
		const ion_uint32 indexOffset,
		const ion_uint32 numElements)
	{
		GLenum ptype=GL_POINTS;
		ion_uint32 vtxfactor=1;
		switch (type) {
			case video::Primitives_Points: vtxfactor=1; ptype=GL_POINTS; break;
			case video::Primitives_Linestrip: vtxfactor=1; ptype=GL_LINE_STRIP; break;
			case video::Primitives_Lines: vtxfactor=1; ptype=GL_LINES; break;
			case video::Primitives_Trianglestrip: vtxfactor=1; ptype=GL_TRIANGLE_STRIP; break;
			case video::Primitives_Trianglefan: vtxfactor=1; ptype=GL_TRIANGLE_FAN; break;
			case video::Primitives_Triangles: vtxfactor=3; ptype=GL_TRIANGLES; break;
		}

		vertexstream.bind();
		indexstream.bind();

		switch (m_StorageMode) {
			case OGLVertexStorage_VBO:
				glDrawElements(ptype,numElements*vtxfactor,
					(indexstream.indexformat()==video::Indexformat_16bit) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
#ifdef _MSC_VER
#pragma warning(disable:4312)
#endif
					(const GLvoid*)(indexformatSizeLookup(indexstream.indexformat())*indexOffset));
#ifdef _MSC_VER
#pragma warning(default:4312)
#endif
				break;
			case OGLVertexStorage_VA:
				glDrawElements(ptype,numElements*vtxfactor,
					GL_UNSIGNED_INT,
					(const GLuint*)(indexstream.mappedPointer())+indexOffset);
				break;
			case OGLVertexStorage_Immediate: {

				glBegin(ptype);
				for (ion_uint32 vtxnr=0;vtxnr<vtxfactor*numElements;++vtxnr) {
					ion_uint32 idx=indexstream.index(vtxnr+indexOffset);

					const video::Vertexformat &vf=vertexstream.vertexformat();
					ion_uint32 stage1d=0,stage2d=0,stage3d=0,texstage=0;
					for (unsigned long vfentry=0;vfentry<vf.numEntries();++vfentry) {

						switch (vf.entry(vfentry)) {
							case video::VertexFormatEntry_Normal: {
								const math::Vector3f& n=vertexstream.normal(idx);
								glNormal3fv(n);
							} break;
							case video::VertexFormatEntry_Diffuse: {
								ion_uint32 color=vertexstream.diffuseColor(idx);
								float a=(float)(color>>24)/255.0f;
								float b=(float)((color>>16)&255)/255.0f;
								float g=(float)((color>>8)&255)/255.0f;
								float r=(float)(color&255)/255.0f;
								glColor4f(r,g,b,a);
							} break;
							case video::VertexFormatEntry_Texcoord1D:
								glMultiTexCoord1f(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord1D(idx,stage1d));
								++stage1d;
								++texstage;
								break;
							case video::VertexFormatEntry_Texcoord2D:
								glMultiTexCoord2fv(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord2D(idx,stage2d));
								++stage2d;
								++texstage;
								break;
							case video::VertexFormatEntry_Texcoord3D:
								glMultiTexCoord3fv(GL_TEXTURE0_ARB+texstage,vertexstream.texcoord3D(idx,stage3d));
								++stage3d;
								++texstage;
								break;
						}
					}

					glVertex3f(
						vertexstream.position(idx).x(),
						vertexstream.position(idx).y(),
						vertexstream.position(idx).z());
				}
				glEnd();
			} break;
			default:break;
		}
	}




	//*********** STATE SETTING

	void OGLDevice::enableCap(const GLenum cap,const bool state)
	{
		if (state) glEnable(cap); else glDisable(cap);
	}

	// hw blending. default: false
	void OGLDevice::blending(const bool state)
	{
		enableCap(GL_BLEND,state);
	}

	// handles zbuffer (both test & writing). default: true
	void OGLDevice::zbuffer(const bool state)
	{
		enableCap(GL_DEPTH_TEST,state);
	}

	// z-writing. default: true
	void OGLDevice::zwrite(const bool state)
	{
		glDepthMask(state ? GL_TRUE : GL_FALSE);
	}

	// handles stencil buffer (both test & writing). default: false
	void OGLDevice::stencilbuffer(const bool state)
	{
		enableCap(GL_STENCIL_TEST,state);
	}

	// stencil writing. default: 0xFFFFFFFF
	void OGLDevice::stencilwritemask(const ion_uint32 stencilmask)
	{
		glStencilMask(stencilmask);
	}

	// backface culling. default: true
	void OGLDevice::culling(const bool state)
	{
		enableCap(GL_CULL_FACE,state);
	}

	void OGLDevice::cullmode(const video::Cullingmode mode)
	{
		switch (mode) {
			case video::Cullingmode_Clockwise:glCullFace(GL_BACK); break;
			case video::Cullingmode_Counterclockwise:glCullFace(GL_FRONT); break;
		}
	}

	// blending function. default: one,zero
	void OGLDevice::blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend)
	{
		ion_uint32 blendfactors[2]={srcblend,destblend};
		GLenum factor[2];

		for (int i=0;i<2;++i) {
			switch (blendfactors[i]) {
				case video::BlendFunc_Zero:factor[i]=GL_ZERO; break;
				case video::BlendFunc_One:factor[i]=GL_ONE; break;
				case video::BlendFunc_Src_Color:factor[i]=GL_SRC_COLOR; break;
				case video::BlendFunc_One_Minus_Src_Color:factor[i]=GL_ONE_MINUS_SRC_COLOR; break;
				case video::BlendFunc_Dest_Color:factor[i]=GL_DST_COLOR; break;
				case video::BlendFunc_One_Minus_Dest_Color:factor[i]=GL_ONE_MINUS_DST_COLOR; break;
				case video::BlendFunc_Src_Alpha:factor[i]=GL_SRC_ALPHA; break;
				case video::BlendFunc_One_Minus_Src_Alpha:factor[i]=GL_ONE_MINUS_SRC_ALPHA; break;
				case video::BlendFunc_Dest_Alpha:factor[i]=GL_DST_ALPHA; break;
				case video::BlendFunc_One_Minus_Dest_Alpha:factor[i]=GL_ONE_MINUS_DST_ALPHA; break;
			}
		}

		glBlendFunc(factor[0],factor[1]);
	}

	// z test function. default: lessequal
	void OGLDevice::zfunc(const video::CompareFunc func)
	{
		GLenum glfunc=GL_ALWAYS;

		switch (func) {
			case video::CompareFunc_Never:glfunc=GL_NEVER; break;
			case video::CompareFunc_Less:glfunc=GL_LESS; break;
			case video::CompareFunc_Lessequal:glfunc=GL_LEQUAL; break;
			case video::CompareFunc_Equal:glfunc=GL_EQUAL; break;
			case video::CompareFunc_Greaterequal:glfunc=GL_GEQUAL; break;
			case video::CompareFunc_Greater:glfunc=GL_GREATER; break;
			case video::CompareFunc_Notequal:glfunc=GL_NOTEQUAL; break;
			case video::CompareFunc_Always:glfunc=GL_ALWAYS; break;
		}

		glDepthFunc(glfunc);
	}

	// z test function. default: lessequal,0,0
	void OGLDevice::stencilfunc(const video::CompareFunc func,const ion_uint32 ref,const ion_uint32 writemask)
	{
		GLenum glfunc=GL_NEVER;

		switch (func) {
			case video::CompareFunc_Never:glfunc=GL_NEVER; break;
			case video::CompareFunc_Less:glfunc=GL_LESS; break;
			case video::CompareFunc_Lessequal:glfunc=GL_LEQUAL; break;
			case video::CompareFunc_Equal:glfunc=GL_EQUAL; break;
			case video::CompareFunc_Greaterequal:glfunc=GL_GEQUAL; break;
			case video::CompareFunc_Greater:glfunc=GL_GREATER; break;
			case video::CompareFunc_Notequal:glfunc=GL_NOTEQUAL; break;
			case video::CompareFunc_Always:glfunc=GL_ALWAYS; break;
		}

		glStencilFunc(glfunc,ref,writemask);
	}

	// stencil operations. default: zero,zero,zero
	void OGLDevice::stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass)
	{
		GLenum glfunc[3];
		ion_uint32 funcs[3]={opfail,opzfail,oppass};

		for (int i=0;i<3;++i) {
			switch (funcs[i]) {
				case video::StencilOp_Keep:glfunc[i]=GL_KEEP; break;
				case video::StencilOp_Zero:glfunc[i]=GL_ZERO; break;
				case video::StencilOp_Set_To_Reference:glfunc[i]=GL_REPLACE; break;
				case video::StencilOp_Incr:glfunc[i]=GL_INCR; break;
				case video::StencilOp_Decr:glfunc[i]=GL_DECR; break;
				case video::StencilOp_Invert:glfunc[i]=GL_INVERT; break;
			}
		}

		glStencilOp(glfunc[0],glfunc[1],glfunc[2]);
	}

	// geometry drawing mode. default: faces
	void OGLDevice::drawingmode(const video::Drawingmode mode)
	{
		switch (mode) {
			case video::Drawingmode_Points:glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); break;
			case video::Drawingmode_Lines:glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
			case video::Drawingmode_Faces:glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
			default:break;
		}
	}

	// shading of filled triangles. default: smooth
	void OGLDevice::solidshading(const video::Solidshading type)
	{
		switch (type) {
			case video::Solidshading_Flat:glShadeModel(GL_FLAT); break;
			case video::Solidshading_Smooth:glShadeModel(GL_SMOOTH); break;
		default:break;
		}
	}

	// depth bias. default: 0,0 (disables depth bias)
	void OGLDevice::depthbias(const float scaledbias,const float uniformbias)
	{
		if ((scaledbias==0) && (uniformbias==0)) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisable(GL_POLYGON_OFFSET_LINE);
			glDisable(GL_POLYGON_OFFSET_POINT);
		} else {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_POLYGON_OFFSET_POINT);

			glPolygonOffset(scaledbias,uniformbias);
		}
	}

	// scissor test state. default: false
	void OGLDevice::scissortest(const bool state)
	{
		if (state)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);			
	}

	void OGLDevice::scissorRectangle(const math::IntRectangle& rectangle)
	{
		glScissor(
			rectangle.left(),
			((long)m_Displayheight-1)-rectangle.top()-rectangle.height(),
			rectangle.width(),
			rectangle.height());

	}
	
	void OGLDevice::rendercolortarget(const video::Texture::Texsurface* pTex)
	{
		if (m_pColorRendertarget==pTex) return; // texture already set as rendertarget -> do nothing

		if (m_pColorRendertarget) { m_pColorRendertarget->unbind(); m_pColorRendertarget=0; }
		//else m_Projmatrix(1,1)*=-1;

		if (pTex) {
			if (video::isColorformat(pTex->m_pTexture->actualpixelformat())) {
				m_pColorRendertarget=((OGLSurface*)pTex);
				m_pColorRendertarget->bind();
			}
		}// else m_Projmatrix(1,1)*=-1;
	}
	
	const video::Texture::Texsurface* OGLDevice::rendercolortarget() const
	{
		return m_pColorRendertarget;
	}

	void OGLDevice::renderdepthstenciltarget(const video::Texture::Texsurface* pTex)
	{
		if (m_pDepthRendertarget==pTex) return; // texture already set as rendertarget -> do nothing

		if (m_pDepthRendertarget) { m_pDepthRendertarget->unbind(); m_pDepthRendertarget=0; }

		if (pTex) {
			if (video::isDepthformat(pTex->m_pTexture->actualpixelformat())) {
				m_pDepthRendertarget=((OGLSurface*)pTex);
				m_pDepthRendertarget->bind();
			}
		}
	}
	
	const video::Texture::Texsurface* OGLDevice::renderdepthstenciltarget() const
	{
		return m_pDepthRendertarget;
	}


	const math::Matrix4f& OGLDevice::projmatrix() const
	{
		return Videodevice::projmatrix();
	}

	void OGLDevice::projmatrix(const math::Matrix4f& newmatrix)
	{
		if (m_pColorRendertarget!=0) {
			math::Matrix4f m;
			m.scale(1,-1,1);
			Videodevice::projmatrix(m*newmatrix);
		} else Videodevice::projmatrix(newmatrix);
	}


	// ************* Sample stage parameters

	unsigned long OGLDevice::samplerstageParameterInt(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam) const
	{
		return m_SamplerstageparametersInt[samplerstage][sampparam];
	}

	float OGLDevice::samplerstageParameterFloat(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam) const
	{
		return m_SamplerstageparametersFloat[samplerstage][sampparam];
	}

	void OGLDevice::samplerstageParameterInt(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam,const unsigned long value)
	{
		m_SamplerstageparametersInt[samplerstage][sampparam]=value;
		if (m_pBoundTextures[samplerstage]!=0)
			m_pBoundTextures[samplerstage]->samplerstageParameterInt(sampparam,value);
	}

	void OGLDevice::samplerstageParameterFloat(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam,const float value)
	{
		m_SamplerstageparametersFloat[samplerstage][sampparam]=value;
		if (m_pBoundTextures[samplerstage]!=0)
			m_pBoundTextures[samplerstage]->samplerstageParameterFloat(sampparam,value);
	}

	void OGLDevice::bindTexture(const ion_uint32 samplerstage,const video::Texture* texVar)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+samplerstage);

		if (!texVar) {
			if (m_pBoundTextures[samplerstage]) {
				m_pBoundTextures[samplerstage]->unbind();
				glDisable(m_pBoundTextures[samplerstage]->target());
				m_pBoundTextures[samplerstage]=0;
			}
		} else {
			OGLTexture *pNewtex=(OGLTexture*)(texVar->texHandle());

			if (m_pBoundTextures[samplerstage]!=pNewtex) {

				if (m_pBoundTextures[samplerstage]!=0) {
					m_pBoundTextures[samplerstage]->unbind();
					if (pNewtex->target()!=m_pBoundTextures[samplerstage]->target())
						glDisable(m_pBoundTextures[samplerstage]->target());
				}

				m_pBoundTextures[samplerstage]=pNewtex;
				glEnable(m_pBoundTextures[samplerstage]->target());
				m_pBoundTextures[samplerstage]->bind();
				m_pBoundTextures[samplerstage]->copyStates(*this,samplerstage);
			}
		}
	}

	// ************* Texture stage parameters

	unsigned long OGLDevice::texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return m_TexstageparametersInt[texstage][texparam];
	}

	float OGLDevice::texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return m_TexstageparametersFloat[texstage][texparam];
	}

	void OGLDevice::texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value)
	{
		m_TexstageparametersInt[texstage][texparam]=value;
	}

	void OGLDevice::texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam,const float value)
	{
		m_TexstageparametersFloat[texstage][texparam]=value;
	}

	// ************* Vertex/fragment program parameters

	video::Program* OGLDevice::vertexprogram()
	{
		return m_pCurrentVertexProgram;
	}

	video::Program* OGLDevice::fragmentprogram()
	{
		return m_pCurrentFragmentProgram;
	}

	void OGLDevice::vertexprogram(video::Program* pVertexprogram)
	{
		if (m_pCurrentVertexProgram==pVertexprogram) return;

#ifdef OPENGLDRV_USE_CG
		bool oldVPIsCG=(m_pCurrentVertexProgram!=0) ? (m_pCurrentVertexProgram->format()=="cg_vprogram") : false;
		bool newVPIsCG=(pVertexprogram!=0) ? (pVertexprogram->format()=="cg_vprogram") : false;

		if (oldVPIsCG && !newVPIsCG) {
			cgGLUnbindProgram(m_CgVertexProfile);
			cgGLDisableProfile(m_CgVertexProfile);
		} else if (!oldVPIsCG && newVPIsCG) {
			cgGLEnableProfile(m_CgVertexProfile);
		}
#endif
		if (pVertexprogram==0) {
			fixedSetWorldMatrix(worldmatrix());
			fixedSetViewMatrix(viewmatrix());
			fixedSetProjectionMatrix(projmatrix());
		}

		m_pCurrentVertexProgram=pVertexprogram;
	}

	void OGLDevice::fragmentprogram(video::Program* pFragmentprogram)
	{
		if (m_pCurrentFragmentProgram==pFragmentprogram) return;

#ifdef OPENGLDRV_USE_CG
		bool oldFPIsCG=(m_pCurrentFragmentProgram!=0) ? (m_pCurrentFragmentProgram->format()=="cg_fprogram") : 0;
		bool newFPIsCG=(pFragmentprogram!=0) ? (pFragmentprogram->format()=="cg_fprogram") : 0;

		if (oldFPIsCG && !newFPIsCG) {
			cgGLUnbindProgram(m_CgFragmentProfile);
			cgGLDisableProfile(m_CgFragmentProfile);
		} else if (!oldFPIsCG && newFPIsCG) {
			cgGLEnableProfile(m_CgFragmentProfile);
		}
#endif

		m_pCurrentFragmentProgram=pFragmentprogram;
	}

	// ************* Fixed function pipeline support

	void OGLDevice::activateFFPipeline(const video::Fixedfunctionactivationtype pipeline)
	{
		switch (pipeline) {
			case video::FixedfunctionVertex:vertexprogram(0); break;
			case video::FixedfunctionFragment:fragmentprogram(0); break;
		}
	}

	void OGLDevice::fixedSetMaterialAmbientColor(const base::Float4& color)
	{
		float f[4]={color[1],color[2],color[3],color[0]};
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,f);
	}

	void OGLDevice::fixedSetMaterialDiffuseColor(const base::Float4& color)
	{
		float f[4]={color[1],color[2],color[3],color[0]};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,f);
	}

	void OGLDevice::fixedSetMaterialSpecularColor(const base::Float4& color)
	{
		float f[4]={color[1],color[2],color[3],color[0]};
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,f);
	}

	void OGLDevice::fixedSetMaterialEmissiveColor(const base::Float4& color)
	{
		float f[4]={color[1],color[2],color[3],color[0]};
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,f);
	}

	void OGLDevice::fixedSetMaterialShininess(const float shininess)
	{
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	}

	ion_uint32 OGLDevice::fixedMaxTextures()
	{
		return 0;
	}

	ion_uint32 OGLDevice::fixedMaxTextureStages()
	{
		GLint maxtexturestages;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxtexturestages);
		return static_cast<ion_uint32>(maxtexturestages);
	}

	void OGLDevice::fixedLighting(const bool state)
	{
		enableCap(GL_LIGHTING,state);
	}

	void OGLDevice::fixedSetLightEnabled(const ion_uint32 lightNr,const bool state)
	{
		enableCap(GL_LIGHT0+lightNr,state);
	}

	void OGLDevice::fixedSetLightProperties(const ion_uint32 lightNr,const video::FFLightProperties& properties)
	{
		// TODO: Directional & spot light support
		{
			float f[4]={properties.m_Position.x(),properties.m_Position.y(),properties.m_Position.z(),1};
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadMatrixf(static_cast<const float*>(m_View));
			glLightfv(GL_LIGHT0+lightNr,GL_POSITION,f);
			glPopMatrix();
		}

		{
			float f[4]={properties.m_Direction.x(),properties.m_Direction.y(),properties.m_Direction.z(),1};
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadMatrixf(static_cast<const float*>(m_View));
			glLightfv(GL_LIGHT0+lightNr,GL_SPOT_DIRECTION,f);
			glPopMatrix();
		}

		{
			float f[4]={properties.m_Color[1],properties.m_Color[2],properties.m_Color[3],properties.m_Color[0]};
			glLightfv(GL_LIGHT0+lightNr,GL_DIFFUSE,f);
		}

		glLightf(GL_LIGHT0+lightNr,GL_CONSTANT_ATTENUATION,properties.m_ConstantAttenuation);
		glLightf(GL_LIGHT0+lightNr,GL_LINEAR_ATTENUATION,properties.m_LinearAttenuation);
		glLightf(GL_LIGHT0+lightNr,GL_QUADRATIC_ATTENUATION,properties.m_QuadraticAttenuation);
	}

	void OGLDevice::fixedSetProjectionMatrix(const base::Float4x4& matrix)
	{
		math::Matrix4f m;
		m=matrix;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(static_cast<const float*>(m));
	}

	void OGLDevice::fixedSetWorldMatrix(const base::Float4x4& matrix)
	{
		m_World=matrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(static_cast<const float*>((m_World*m_View)));
	}

	void OGLDevice::fixedSetViewMatrix(const base::Float4x4& matrix)
	{
		m_View=matrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(static_cast<const float*>((m_World*m_View)));
	}

	void OGLDevice::fixedSetTextureMatrix(const ion_uint32 texstage,const base::Float4x4& matrix)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+texstage);
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(static_cast<const float*>(matrix));
	}

	ion_uint32 OGLDevice::fixedMaxLights() const
	{
		GLint maxlights;
		glGetIntegerv(GL_MAX_LIGHTS,&maxlights);
		return static_cast<ion_uint32>(maxlights);
	}

	void OGLDevice::fixedFog(const bool state)
	{
		if (state)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
	}

	void OGLDevice::fixedSetFogStart(const float start)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(static_cast<const float*>(m_View));
		glFogf(GL_FOG_START,start);
		glPopMatrix();
	}

	void OGLDevice::fixedSetFogEnd(const float end)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(static_cast<const float*>(m_View));
		glFogf(GL_FOG_END,end);
		glPopMatrix();
	}

	void OGLDevice::fixedSetFogColor(const base::Float3& color)
	{
		float f[4]={color[0],color[1],color[2],1};
		glFogfv(GL_FOG_COLOR,f);
	}

	void OGLDevice::fixedUseRangeFog(const bool use)
	{
		// TODO
	}

#ifdef OPENGLDRV_USE_CG
	CGcontext OGLDevice::cgContext()
	{
		return m_CgContext;
	}

	CGprofile OGLDevice::cgVertexProfile()
	{
		return m_CgVertexProfile;
	}

	CGprofile OGLDevice::cgFragmentProfile()
	{
		return m_CgFragmentProfile;
	}
#endif

}
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ion::video::Videodevice* createVideodeviceInstance()
{
	return new ion::opengldrv::OGLDevice;
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* videodeviceDescription()
{
	return "OpenGL videodevice v0.8 build 2006-03-24 21:08";
}
