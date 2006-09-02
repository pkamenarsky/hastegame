#ifndef ION_VIDEO_VIDEODEVICE_HH_INCLUDED
#define ION_VIDEO_VIDEODEVICE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"
#include "../base/numericarrays.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"
#include "../math/vectors.hh"
#include "../math/matrix.hh"
#include "../math/rectangle.hh"
#include "caps.hh"
#include "coreenums.hh"
#include "cubemap.hh"
#include "indexformat.hh"
#include "indexstream.hh"
#include "occlusionquery.hh"
#include "program.hh"
#include "texture2d.hh"
#include "vertexformat.hh"
#include "vertexstream.hh"

namespace ion {
namespace video {

	struct ION_API Videomode {
		ion_uint32 m_Width,m_Height,m_Hz;
		Pixelformat m_Pixelformat;
	};

	class ViewportResizingCallback
	{
	public:
		virtual void checkIonVideodeviceResources()=0;
		virtual ~ViewportResizingCallback() {}
	};

	struct ION_API FFLightProperties
	{
		Lighttype m_Lighttype;
		math::Vector3f m_Position,m_Direction;
		base::Float4 m_Color;
		float m_Range,m_Falloff,m_ConstantAttenuation,m_LinearAttenuation,m_QuadraticAttenuation,
			m_InnerConeAngle,m_OuterAngle;
	};

	class ION_API Videodevice:public base::BaseMemObj
	{
	public:
		~Videodevice();

		//*********** INITIALIZATION / ENUMERATION

		virtual const Caps& caps() const=0;
		virtual ion_uint32 numVideoadapters() const=0;
		virtual ion_uint32 numVideomodes(const ion_uint32 adapterindex,const Pixelformat pixelformat) const=0;
		virtual bool fetchVideomode(const ion_uint32 adapter,
			const ion_uint32 modeindex,Videomode& mode,
			const Pixelformat pixelformat) const=0;

		virtual void init(const ion_uint32 width,const ion_uint32 height,
			const base::String& title,const bool fullscreen,void* displayhandle,
			const ion_uint32 adapter,const Pixelformat colorbufferformat,
			const Pixelformat depthstencilformat,const ion_uint32 Hz,const bool vsync)=0;
			
		virtual ion_intptr windowhandle() const=0;

		virtual void windowtitle(const base::String& title)=0;

		virtual void showCursor(const bool show)=0;
		virtual bool cursorShown() const=0;

		virtual bool testVertexProgram(const base::String& program,const base::String& format)=0;
		virtual bool testFragmentProgram(const base::String& program,const base::String& format)=0;

		virtual ion_uint32 displaywidth() const=0;
		virtual ion_uint32 displayheight() const=0;

		virtual Pixelformat colorbufferFormat() const=0;
		virtual Pixelformat depthstencilbufferFormat() const=0;




		//*********** FRAME START / END / UPDATE
		virtual void clear(const ion_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ion_uint32 stencilvalue=0)=0;

		virtual void begin()=0;
		virtual void end()=0;

		virtual void update()=0;

		// Handles window messages. Returns true if messages were processed; the app should not render in this frame.
		virtual bool handleWindow()=0;

		virtual bool quitting() const=0;

		virtual WindowActivationState windowActivationState() const=0;

		virtual void setViewport(const ion_uint32 newx,const ion_uint32 newy,const ion_uint32 newwidth,const ion_uint32 newheight)=0;

		virtual bool resizeFramebuffer(
			const ion_uint32 newwidth,const ion_uint32 newheight,ViewportResizingCallback* callback)=0;



		//*********** RESOURCE GENERATION

		virtual Vertexstream* createVertexstream(const ion_uint32 numVertices,const Vertexformat& format,
			const ion_uint32 flags,const Memorypool pool)=0;
		virtual Indexstream* createIndexstream(const ion_uint32 numIndices,const Indexformat format,
			const ion_uint32 flags,const Memorypool pool)=0;

		virtual Texture2D* create2DTexture(const base::String& identifier,
			const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,
			const ion_uint32 flags,const Pixelformat format,const Memorypool pool)=0;

		virtual Cubemap* createCubemap(const base::String& identifier,
			const ion_uint32 edgelength,const ion_uint32 levels,
			const ion_uint32 flags,const Pixelformat format,const Memorypool pool)=0;

		virtual Occlusionquery* createOcclusionquery()=0;

		virtual Program* createVertexProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format)=0;
		virtual Program* createFragmentProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format)=0;





		//*********** PRIMITIVES DRAWING

		virtual void drawPrimitives(const PrimitivesType type,
			Vertexstream& vertexstream,const ion_uint32 firstElement,
			const ion_uint32 numElements)=0;
		virtual void drawIndexedPrimitives(
			const PrimitivesType type,
			Vertexstream& vertexstream,
			Indexstream& indexstream,
			const ion_uint32 indexOffset,
			const ion_uint32 numElements)=0;



		//*********** STATE SETTING/GETTING

		// hw blending. default: false
		virtual void blending(const bool state)=0;
		// handles zbuffer (both test & writing). default: true
		virtual void zbuffer(const bool state)=0;
		// z-writing. default: true
		virtual void zwrite(const bool state)=0;
		// handles stencil buffer (both test & writing). default: false
		virtual void stencilbuffer(const bool state)=0;
		// stencil writing mask. default: 0xFFFFFFFF
		virtual void stencilwritemask(const ion_uint32 stencilmask)=0;
		// backface culling. default: true
		virtual void culling(const bool state)=0;
		// culling mode. default: counter clockwise
		virtual void cullmode(const Cullingmode mode)=0;
		// blending function. default: one,zero
		virtual void blendfunc(const BlendFunc srcblend,const BlendFunc destblend)=0;
		// z test function. default: lessequal
		virtual void zfunc(const CompareFunc func)=0;
		// z test function. default: lessequal,0,0
		virtual void stencilfunc(const CompareFunc func,const ion_uint32 ref,const ion_uint32 mask)=0;
		// stencil operations. default: zero,zero,zero
		virtual void stencilop(const StencilOp opfail,const StencilOp opzfail,const StencilOp oppass)=0;
		// geometry drawing mode. default: faces
		virtual void drawingmode(const Drawingmode mode)=0;
		// shading of filled triangles. default: smooth
		virtual void solidshading(const Solidshading type)=0;
		// depth bias. default: 0,0 (disables depth bias)
		virtual void depthbias(const float scaledbias,const float uniformbias)=0;
		// scissor test state. default: false
		virtual void scissortest(const bool state)=0;

		virtual void scissorRectangle(const math::IntRectangle& rectangle)=0;

		virtual void rendercolortarget(const Texture::Texsurface* pTex)=0;
		virtual const Texture::Texsurface* rendercolortarget() const=0;

		virtual void renderdepthstenciltarget(const Texture::Texsurface* pTex)=0;
		virtual const Texture::Texsurface* renderdepthstenciltarget() const=0;



		// ************* Matrices

		const math::Matrix4f& worldmatrix() const;
		void worldmatrix(const math::Matrix4f& newmatrix);

		const math::Matrix4f& viewmatrix() const;
		void viewmatrix(const math::Matrix4f& newmatrix);

		virtual const math::Matrix4f& projmatrix() const;
		virtual void projmatrix(const math::Matrix4f& newmatrix);

		const math::Matrix4f& worldView() const;
		const math::Matrix4f& viewProj() const;
		const math::Matrix4f& worldViewProj() const;
		const math::Vector3f& eyePosition() const;

		// ************* Sample stage parameters

		virtual unsigned long samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const=0;
		virtual float samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const=0;
		virtual void samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const unsigned long value)=0;
		virtual void samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const float value)=0;

		virtual void bindTexture(const ion_uint32 samplerstage,const Texture* texVar)=0;

		// ************* Texture stage parameters

		virtual unsigned long texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam) const=0;
		virtual float texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam) const=0;
		virtual void texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam,const unsigned long value)=0;
		virtual void texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam,const float value)=0;

		// ************* Vertex/fragment program parameters

		virtual video::Program* vertexprogram()=0;
		virtual video::Program* fragmentprogram()=0;

		// ************* Fixed function pipeline support

		virtual void activateFFPipeline(const Fixedfunctionactivationtype pipeline)=0;

		virtual void fixedSetMaterialAmbientColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialDiffuseColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialSpecularColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialEmissiveColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialShininess(const float shininess)=0;
		virtual ion_uint32 fixedMaxTextures()=0;
		virtual ion_uint32 fixedMaxTextureStages()=0;
		virtual void fixedLighting(const bool state)=0;
		virtual void fixedSetLightEnabled(const ion_uint32 lightNr,const bool state)=0;
		virtual void fixedSetLightProperties(const ion_uint32 lightNr,const FFLightProperties& properties)=0;
		virtual void fixedSetTextureMatrix(const ion_uint32 texstage,const base::Float4x4& matrix)=0;
		virtual ion_uint32 fixedMaxLights() const=0;
		virtual void fixedFog(const bool state)=0;
		virtual void fixedSetFogStart(const float start)=0;
		virtual void fixedSetFogEnd(const float end)=0;
		virtual void fixedSetFogColor(const base::Float3& color)=0;
		virtual void fixedUseRangeFog(const bool use)=0;





		// Device-specific data. Use ONLY if you REALLY know what you are doing!
		virtual void deviceState(const ion_intptr state,const ion_intptr data)=0;


	protected:
		// These are protected to prevent direct usage by programs - use worldmatrix/viewmatrix/projmatrix instead
		virtual void fixedSetProjectionMatrix(const base::Float4x4& matrix)=0;
		virtual void fixedSetWorldMatrix(const base::Float4x4& matrix)=0;
		virtual void fixedSetViewMatrix(const base::Float4x4& matrix)=0;

	protected:
		Videodevice();

		mutable math::Vector3f m_Eyepos;
		math::Matrix4f m_Worldmatrix,m_Viewmatrix,m_Projmatrix;
		mutable math::Matrix4f m_Worldview,m_Viewproj,m_Worldviewproj;
		mutable bool m_UpdateEyeposition,m_UpdateWorldview,m_UpdateViewproj,m_UpdateWorldviewproj;

/*		struct Renderdata;
		Renderdata *m_pInternalData;*/
	};

	typedef Videodevice* (*CreateVideodeviceInstanceFunc)();
	typedef const char* (*VideodeviceDescriptionFunc)();

}
}

#endif
