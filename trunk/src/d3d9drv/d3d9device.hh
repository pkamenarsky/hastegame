#ifndef ION_D3D9DRV_D3D9DEVICE_HH_INCLUDED
#define ION_D3D9DRV_D3D9DEVICE_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#ifdef D3D9DRV_USE_CG
#include <Cg/cg.h>
#endif

#include "../base/dll.hh"
#include "../video/videodevice.hh"

namespace ion {
namespace d3d9drv {

	class ION_API D3D9Device:public video::Videodevice
	{
	public:
		D3D9Device();
		~D3D9Device();

		bool isValid() const;


		//*********** INITIALIZATION / ENUMERATION

		const video::Caps& caps() const;
		ion_uint32 numVideoadapters() const;
		ion_uint32 numVideomodes(const ion_uint32 adapterindex,const video::Pixelformat pixelformat) const;
		bool fetchVideomode(const ion_uint32 adapter,const ion_uint32 modeindex,
			video::Videomode& mode,const video::Pixelformat pixelformat) const;

		void init(
			const ion_uint32 width,const ion_uint32 height,
			const base::String& title,const bool fullscreen,void* displayhandle,
			const ion_uint32 adapter,const video::Pixelformat colorbufferformat,
			const video::Pixelformat depthstencilformat,const ion_uint32 Hz,const bool vsync);

		ion_intptr windowhandle() const;

		void windowtitle(const base::String& title);

		void showCursor(const bool show);
		bool cursorShown() const;

		bool testVertexProgram(const base::String& program,const base::String& format);
		bool testFragmentProgram(const base::String& program,const base::String& format);

		ion_uint32 displaywidth() const;
		ion_uint32 displayheight() const;

		video::Pixelformat colorbufferFormat() const;
		video::Pixelformat depthstencilbufferFormat() const;





		//*********** FRAME START / END / UPDATE
		void clear(const ion_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ion_uint32 stencilvalue=0);

		void begin();
		void end();

		void update();
		bool handleWindow();

		bool quitting() const;

		video::WindowActivationState windowActivationState() const;

		void setViewport(const ion_uint32 newx,const ion_uint32 newy,const ion_uint32 newwidth,const ion_uint32 newheight);

		bool resizeFramebuffer(
			const ion_uint32 newwidth,const ion_uint32 newheight,video::ViewportResizingCallback* callback);


		inline LPDIRECT3D9 d3d9interface() const { return m_pD3D9; }
		inline LPDIRECT3DDEVICE9 d3d9deviceinterface() const { return m_pD3DDev9; }
		inline const D3DCAPS9& d3d9caps() const { return m_D3DCaps; }




		//*********** RESOURCE GENERATION

		video::Vertexstream* createVertexstream(const ion_uint32 numVertices,const video::Vertexformat& format,
			const ion_uint32 flags,const video::Memorypool pool);
		video::Indexstream* createIndexstream(const ion_uint32 numIndices,const video::Indexformat format,
			const ion_uint32 flags,const video::Memorypool pool);

		video::Texture2D* create2DTexture(const base::String& identifier,
			const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,
			const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);

		video::Cubemap* createCubemap(const base::String& identifier,
			const ion_uint32 edgelength,const ion_uint32 levels,
			const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);

		video::Occlusionquery* createOcclusionquery();

		video::Program* createVertexProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);
		video::Program* createFragmentProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);





		//*********** PRIMITIVES DRAWING

		void drawPrimitives(const video::PrimitivesType type,
			video::Vertexstream& vertexstream,const ion_uint32 firstElement,
			const ion_uint32 numElements);
		void drawIndexedPrimitives(
			const video::PrimitivesType type,
			video::Vertexstream& vertexstream,
			video::Indexstream& indexstream,
			const ion_uint32 indexOffset,
			const ion_uint32 numElements);




		//*********** STATE SETTING

		// hw blending. default: false
		void blending(const bool state);
		// handles zbuffer (both test & writing). default: true
		void zbuffer(const bool state);
		// z-writing. default: true
		void zwrite(const bool state);
		// handles stencil buffer (both test & writing). default: false
		void stencilbuffer(const bool state);
		// stencil writing. default: 0xFFFFFFFF
		void stencilwritemask(const ion_uint32 stencilmask);
		// backface culling. default: true
		void culling(const bool state);
		void cullmode(const video::Cullingmode mode);
		// blending function. default: one,zero
		void blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend);
		// z test function. default: lessequal
		void zfunc(const video::CompareFunc func);
		// z test function. default: lessequal,0,0
		void stencilfunc(const video::CompareFunc func,const ion_uint32 ref,const ion_uint32 writemask);
		// stencil operations. default: zero,zero,zero
		void stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass);
		// geometry drawing mode. default: faces
		void drawingmode(const video::Drawingmode mode);
		// shading of filled triangles. default: smooth
		void solidshading(const video::Solidshading type);
		// depth bias. default: 0,0 (disables depth bias)
		void depthbias(const float scaledbias,const float uniformbias);
		// scissor test state. default: false
		void scissortest(const bool state);

		void scissorRectangle(const math::IntRectangle& rectangle);

		void rendercolortarget(const video::Texture::Texsurface* pRendertarget);
		const video::Texture::Texsurface* rendercolortarget() const;

		void renderdepthstenciltarget(const video::Texture::Texsurface* pRendertarget);
		const video::Texture::Texsurface* renderdepthstenciltarget() const;

		// ************* Sample stage parameters

		unsigned long samplerstageParameterInt(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam) const;
		float samplerstageParameterFloat(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam) const;
		void samplerstageParameterInt(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam,const unsigned long value);
		void samplerstageParameterFloat(const ion_uint32 samplerstage,const video::Samplerstageparameter sampparam,const float value);

		void bindTexture(const ion_uint32 samplerstage,const video::Texture* texVar);

		// ************* Texture stage parameters

		unsigned long texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam) const;
		float texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam) const;
		void texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value);
		void texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam,const float value);

		// ************* Vertex/fragment program parameters

		video::Program* vertexprogram();
		video::Program* fragmentprogram();

		void vertexprogram(video::Program* pVertexprogram);
		void fragmentprogram(video::Program* pFragmentprogram);

		// ************* Fixed function pipeline support

		void activateFFPipeline(const video::Fixedfunctionactivationtype pipeline);

		void fixedSetMaterialAmbientColor(const base::Float4& color);
		void fixedSetMaterialDiffuseColor(const base::Float4& color);
		void fixedSetMaterialSpecularColor(const base::Float4& color);
		void fixedSetMaterialEmissiveColor(const base::Float4& color);
		void fixedSetMaterialShininess(const float shininess);
		ion_uint32 fixedMaxTextures();
		ion_uint32 fixedMaxTextureStages();
		void fixedLighting(const bool state);
		void fixedSetLightEnabled(const ion_uint32 lightNr,const bool state);
		void fixedSetLightProperties(const ion_uint32 lightNr,const video::FFLightProperties& properties);
		void fixedSetProjectionMatrix(const base::Float4x4& matrix);
		void fixedSetWorldMatrix(const base::Float4x4& matrix);
		void fixedSetViewMatrix(const base::Float4x4& matrix);
		void fixedSetTextureMatrix(const ion_uint32 texstage,const base::Float4x4& matrix);
		ion_uint32 fixedMaxLights() const;
		void fixedFog(const bool state);
		void fixedSetFogStart(const float start);
		void fixedSetFogEnd(const float end);
		void fixedSetFogColor(const base::Float3& color);
		void fixedUseRangeFog(const bool use);

		video::Vertexstream* vertexstream();
		video::Indexstream* indexstream();

		void vertexstream(video::Vertexstream* pStream);
		void indexstream(video::Indexstream* pStream);

#ifdef D3D9DRV_USE_CG
		CGcontext cgContext();
		CGprofile cgVertexProfile();
		CGprofile cgFragmentProfile();
		const char ** cgOptimalVertexProfileOptions();
		const char ** cgOptimalFragmentProfileOptions();
#endif




		void deviceState(const ion_intptr state,const ion_intptr data);

		LRESULT sysProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		static LRESULT CALLBACK wndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	protected:
		void openWindow(const ion_uint32 width,const ion_uint32 height,
			const base::String& title,const bool fullscreen,void* displayhandle);

		LPDIRECT3D9 m_pD3D9;
		LPDIRECT3DDEVICE9 m_pD3DDev9;
		D3DCAPS9 m_D3DCaps;
		D3DMATERIAL9 m_D3D9FFMaterial;
		D3DLIGHT9 m_D3D9FFLight;
		video::Caps m_Caps;

		video::Program *m_pCurrentVertexProgram;
		video::Program *m_pCurrentFragmentProgram;

		video::Vertexstream *m_pCurrentVertexstream;
		video::Indexstream *m_pCurrentIndexstream;

		const video::Texture::Texsurface *m_pRendercolortarget,*m_pRenderdepthstenciltarget;
		LPDIRECT3DSURFACE9 m_pRendertargetSurface,m_pFramebuffer,m_pDepthstenciltargetSurface,m_pDepthstencilbuffer;

		DWORD m_Cullmode;
		DWORD *m_Texcoordindexmodes,*m_Texcoordgenmodes;
		bool m_CullingEnabled,m_IsValid,m_WindowOpened,m_Fullscreen,m_Quitting,m_CursorShown;

#ifdef D3D9DRV_USE_CG
		CGcontext m_CgContext;
		CGprofile m_CgVertexProfile,m_CgFragmentProfile;
		const char ** m_CgOptimalVertexProfileOptions;
		const char ** m_CgOptimalFragmentProfileOptions;
#endif

		HWND m_hWindow;

		video::Pixelformat m_ColorbufferFormat,m_DepthstencilbufferFormat;
		ion_uint32 m_Displaywidth,m_Displayheight;

		struct Resources;
		Resources *m_pResources;

		video::WindowActivationState m_WindowActivationState;

		D3DPRESENT_PARAMETERS m_D3DPP;
	};

}
}

#endif
