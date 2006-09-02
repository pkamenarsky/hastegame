#include <assert.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <list>

#include "../base/log.hh"
#include "d3d9device.hh"
#include "d3d9cubemap.hh"
#include "d3d9fragmentprogram.hh"
#include "d3d9indexstream.hh"
#include "d3d9surface.hh"
#include "d3d9texture2d.hh"
#include "d3d9vertexstream.hh"
#include "d3d9vertexprogram.hh"
#include "tod3d9convs.hh"

#ifdef D3D9DRV_USE_CG
#include <Cg/cgD3D9.h>

#include "cgd3d9fragmentprogram.hh"
#include "cgd3d9vertexprogram.hh"
#endif

namespace ion {
namespace d3d9drv {


#define D3D9DRV_CG_DEBUG_ERRORCALLBACK


	void D3D9Device::deviceState(const ion_intptr state,const ion_intptr data)
	{
	}


	namespace iond3d9deviceWin32 {

		static long refcounter=0;

		void registerClass()
		{
			if (refcounter>0) return;

			WNDCLASSEX windowclass;

			windowclass.cbClsExtra=0;
			windowclass.cbSize=sizeof(windowclass);
			windowclass.cbWndExtra=0;
			windowclass.hbrBackground=GetSysColorBrush(COLOR_MENU);
			windowclass.hCursor=LoadCursor(NULL,IDC_ARROW);
			windowclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
			windowclass.hIconSm=0;
			windowclass.hInstance=GetModuleHandle(0);							
			windowclass.lpfnWndProc=D3D9Device::wndProc;
			windowclass.lpszClassName="ion_Win32_d3d9window_h1546814671";
			windowclass.lpszMenuName=0;
			windowclass.style=0;

			RegisterClassEx(&windowclass);

			++refcounter;
		}

		void unregisterClass()
		{
			if (refcounter<=0) return;
			--refcounter;

			if (refcounter==0) {
				UnregisterClass("ion_Win32_d3d9window_h1546814671",GetModuleHandle(0));
			}
		}
	}



#ifdef D3D9DRV_CG_DEBUG_ERRORCALLBACK
	CGcontext errctx;

	void cgErrorCallback(void)
	{
		CGerror LastError = cgGetError();

		if(LastError)
		{
			const char *Listing = cgGetLastListing(errctx);
			base::logstream() << "---------------------------------------------------\n";
			base::logstream() << cgGetErrorString(LastError) << "\n\n";
			base::logstream() << Listing << "\n";
			base::logstream() << "---------------------------------------------------\n";
		}
	}
#endif







	// Resource lists needed for notification when the device is reset
	struct D3D9Device::Resources
	{
		std::list<D3D9Texture2D* > m_2DTextures;
		std::list<D3D9Cubemap* > m_Cubemaps;
		std::list<D3D9Vertexstream* > m_Vertexstreams;
		std::list<D3D9Indexstream* > m_Indexstreams;
	};

	D3D9Device::D3D9Device():m_pD3D9(0),m_pD3DDev9(0),m_pCurrentVertexProgram(0),
		m_pCurrentFragmentProgram(0),m_pCurrentVertexstream(0),m_pCurrentIndexstream(0),
		m_pRendercolortarget(0),m_pRenderdepthstenciltarget(0),m_pFramebuffer(0),
		m_pDepthstencilbuffer(0),m_Cullmode(D3DCULL_CCW),m_CullingEnabled(true),
		m_IsValid(true),m_WindowOpened(false),m_Quitting(false),m_CursorShown(true),
		m_WindowActivationState(video::WindowActivated)
	{
		m_pResources=new Resources;

		memset(&m_D3D9FFMaterial,0,sizeof(m_D3D9FFMaterial));
		memset(&m_D3D9FFLight,0,sizeof(m_D3D9FFLight));

		m_pD3D9=Direct3DCreate9(D3D_SDK_VERSION);

		if (!m_pD3D9) {
			base::log("D3D9Device::D3D9Device()",base::Error) << "could not create the D3D9 interface\n";

			m_IsValid=false;
		}

		m_Texcoordindexmodes=new DWORD[8];
		m_Texcoordgenmodes=new DWORD[8];
		for (int i=0;i<8;++i) {
			m_Texcoordindexmodes[i]=0;
			m_Texcoordgenmodes[i]=0;
		}
	}

	D3D9Device::~D3D9Device()
	{
		rendercolortarget(0);
		renderdepthstenciltarget(0);

#ifdef D3D9DRV_USE_CG
		cgD3D9SetDevice(NULL);
		cgDestroyContext(m_CgContext);
#endif

		showCursor(true);

		if (m_pFramebuffer) m_pFramebuffer->Release();
		if (m_pDepthstencilbuffer) m_pDepthstencilbuffer->Release();
		if (m_pD3DDev9) m_pD3DDev9->Release();
		if (m_pD3D9) m_pD3D9->Release();

		DestroyWindow(m_hWindow);

		iond3d9deviceWin32::unregisterClass();

		delete[] m_Texcoordgenmodes;
		delete[] m_Texcoordindexmodes;

		delete m_pResources;
	}

	bool D3D9Device::isValid() const
	{
		return m_IsValid;
	}




	LRESULT CALLBACK D3D9Device::wndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
	#ifdef _MSC_VER
	#pragma warning(disable:4312)
	#endif
		// TODO: Verify this
		D3D9Device *pD3D9Device=(D3D9Device*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	#ifdef _MSC_VER
	#pragma warning(default:4312)
	#endif

		// TODO: Check if the pointer really points to a D3D9Device instance
		if (pD3D9Device==0) return DefWindowProc(hWnd,uMsg,wParam,lParam);
		else return pD3D9Device->sysProc(hWnd,uMsg,wParam,lParam);
	}

	LRESULT D3D9Device::sysProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
			case WM_ACTIVATEAPP:m_WindowActivationState=(wParam==FALSE) ?
									video::WindowDeactivated : video::WindowActivated; return 0;
			case WM_ACTIVATE:m_WindowActivationState=(LOWORD(wParam)==WA_INACTIVE) ?
								video::WindowDeactivated : video::WindowActivated; return 0;
			case WM_SYSCOMMAND:
				switch( wParam ) {
					case SC_MOVE:
					case SC_SIZE:
					case SC_MAXIMIZE:
					case SC_KEYMENU:
					case SC_MONITORPOWER:
						if (m_Fullscreen) return 0;
						else break;
					default:break;
				} break;
			case WM_QUIT:
			case WM_CLOSE:
			case WM_DESTROY:m_Quitting=true; return 0;
			default:break;
		}

		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}




	const video::Caps& D3D9Device::caps() const
	{
		return m_Caps;
	}

	ion_uint32 D3D9Device::numVideoadapters() const
	{
		return m_pD3D9->GetAdapterCount();
	}

	ion_uint32 D3D9Device::numVideomodes(const ion_uint32 adapterindex,
		const video::Pixelformat pixelformat) const
	{
		D3DFORMAT fmt=d3dpixelformat(pixelformat);
		return m_pD3D9->GetAdapterModeCount(adapterindex,fmt);
	}

	bool D3D9Device::fetchVideomode(const ion_uint32 adapter,
		const ion_uint32 modeindex,video::Videomode& mode,
		const video::Pixelformat pixelformat) const
	{
		D3DFORMAT fmt=d3dpixelformat(pixelformat);
		D3DDISPLAYMODE d3ddisplaymode;
		HRESULT hr=m_pD3D9->EnumAdapterModes(adapter,fmt,modeindex,&d3ddisplaymode);

		if (hr!=D3D_OK) return false;

		mode.m_Width=d3ddisplaymode.Width;
		mode.m_Height=d3ddisplaymode.Height;
		mode.m_Hz=d3ddisplaymode.RefreshRate;
		mode.m_Pixelformat=pixelformat; // TODO: correct this (use the format from d3ddisplaymode)

		return true;
	}




	void D3D9Device::openWindow(const ion_uint32 width,const ion_uint32 height,
		const base::String& title,const bool fullscreen,void* displayhandle)
	{
		if (m_WindowOpened) {
			base::log("D3D9Device::openWindow()",base::Warning) <<
				"Window already opened\n";
			return;
		}

		iond3d9deviceWin32::registerClass();

		m_Fullscreen=fullscreen;

		unsigned int flags=WS_VISIBLE|(fullscreen ? WS_POPUP : (WS_CAPTION|WS_BORDER|WS_OVERLAPPED|WS_SYSMENU));

		RECT rectangle;
		rectangle.left = 0;
		rectangle.right = width - 1;
		rectangle.top = 0;
		rectangle.bottom = height - 1;
		if (!fullscreen) AdjustWindowRect(&rectangle, flags, false);


		const int windowWidth= rectangle.right - rectangle.left + 1;
		const int windowHeight = rectangle.bottom - rectangle.top + 1;
		const int windowX=fullscreen ? 0 : ((GetSystemMetrics(SM_CXSCREEN)-windowWidth)>>1);
		const int windowY=fullscreen ? 0 : ((GetSystemMetrics(SM_CYSCREEN)-windowHeight)>>1);

		m_hWindow = CreateWindowEx(0,
			"ion_Win32_d3d9window_h1546814671",
			title.cstr(), flags, windowX, windowY, windowWidth, windowHeight,
			0, 0, GetModuleHandle(0), 0);

	#ifdef _MSC_VER
	#pragma warning(disable:4244)
	#endif
		SetWindowLongPtr(m_hWindow,GWL_USERDATA,(LONG_PTR)this);
	#ifdef _MSC_VER
	#pragma warning(default:4244)
	#endif
		
		ShowWindow(m_hWindow,SW_SHOW);

		m_WindowOpened=true;
	}

	void D3D9Device::init(
		const ion_uint32 width,const ion_uint32 height,
		const base::String& title,const bool fullscreen,void* displayhandle,
		const ion_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ion_uint32 Hz,const bool vsync)
	{
		if (!m_IsValid) {
			base::log("D3D9Device::init()",base::Warning) <<
				"Initializing an invalid device\n";
		}

		openWindow(width,height,title,fullscreen,displayhandle);

		bool windowed=!m_Fullscreen;

		D3DFORMAT adapterfmt;
		if (windowed) {
			 D3DDISPLAYMODE d3ddm;
			 m_pD3D9->GetAdapterDisplayMode(adapter,&d3ddm);
			 adapterfmt=d3ddm.Format;
		} else adapterfmt=d3dpixelformat(colorbufferformat);

		if (adapterfmt==D3DFMT_UNKNOWN) {
			base::log("D3D9Device::init()",base::Error) << "Unknown/unsupported color buffer format\n";
			m_IsValid=false; return;
		}

		RECT r;
		GetClientRect(m_hWindow,&r);

		m_D3DPP.Windowed=windowed;
		m_D3DPP.BackBufferWidth=r.right-r.left;
		m_D3DPP.BackBufferHeight=r.bottom-r.top;
		m_D3DPP.BackBufferCount=1;
		m_D3DPP.BackBufferFormat=adapterfmt;
		m_D3DPP.AutoDepthStencilFormat=d3dpixelformat(depthstencilformat);
		m_D3DPP.EnableAutoDepthStencil=TRUE;
		m_D3DPP.Flags=0;
		m_D3DPP.PresentationInterval=vsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
		m_D3DPP.FullScreen_RefreshRateInHz=windowed ? 0 : Hz;
		m_D3DPP.hDeviceWindow=m_hWindow;
		m_D3DPP.MultiSampleQuality=0;
		m_D3DPP.MultiSampleType=D3DMULTISAMPLE_NONE;
		m_D3DPP.SwapEffect=D3DSWAPEFFECT_DISCARD;
		m_D3DPP.Windowed=windowed;

		// Getting caps
		base::log("D3D9Device::init()",base::Message) << "Getting caps\n";

		HRESULT hr=m_pD3D9->GetDeviceCaps(adapter,D3DDEVTYPE_HAL,&m_D3DCaps);
		if (FAILED(hr)) {
			base::log("D3D9Device::init()",base::Error) << "GetDeviceCaps() failed\n";
			m_IsValid=false;
			return;
		}

		// Caps structure is initialized here
		{
			base::log("D3D9Device::init()",base::Message) << "Analyzing caps\n";

			m_Caps.m_NPOTTextures=!((m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_POW2) ||
				(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL));
			m_Caps.m_RectangularTextures=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_SQUAREONLY)==0;
			m_Caps.m_Cubemaps=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP)!=0;
			m_Caps.m_NPOTCubemaps=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP_POW2)==0;
			m_Caps.m_ProjectedTextures=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_PROJECTED)!=0;
			m_Caps.m_DisplacementMaps=false;
			m_Caps.m_CompressedTextures=false;

			m_Caps.m_MaxTextureWidth=m_D3DCaps.MaxTextureWidth;
			m_Caps.m_MaxTextureHeight=m_D3DCaps.MaxTextureHeight;
			m_Caps.m_MaxTextureVolumeDepth=m_D3DCaps.MaxVolumeExtent;
			m_Caps.m_MaxAnisotropy=m_D3DCaps.MaxAnisotropy;
			m_Caps.m_MaxTextureBlendStages=m_D3DCaps.MaxTextureBlendStages;
			m_Caps.m_MaxActiveFFLights=m_D3DCaps.MaxActiveLights;
			m_Caps.m_MaxPointSize=m_D3DCaps.MaxPointSize;
			m_Caps.m_MaxPrimitiveCount=m_D3DCaps.MaxPrimitiveCount;
			m_Caps.m_MaxVertexIndex=m_D3DCaps.MaxVertexIndex;

		}

		// Probing formats
		{

			video::Caps::Texflags **pTexflags,
				*pTexflagarrays[]={m_Caps.m_Valid2DTextureFormats,m_Caps.m_Valid3DTextureFormats,
				m_Caps.m_ValidCubemapFormats,0};

			const D3DRESOURCETYPE rtype[]={D3DRTYPE_TEXTURE,D3DRTYPE_VOLUMETEXTURE,D3DRTYPE_CUBETEXTURE};

			pTexflags=pTexflagarrays;
			unsigned int ii=0;

			base::log("D3D9Device::init()",base::Message) << "Probing texture formats\n";

			while (*pTexflags) {

				video::Caps::Texflags *pTexflagarray=*pTexflags;

				for (unsigned int i=0;i<video::Pixelformat_NumFormats;++i) {
					D3DFORMAT fmt=d3dpixelformat((video::Pixelformat)i);
					hr=0;

					hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,0,rtype[ii],fmt);
					if (FAILED(hr)) { pTexflagarray[i].m_Supported=false; continue; }
					else pTexflagarray[i].m_Supported=true;

					if ((i==video::Pixelformat_RGB_DXT1)
						|| (i==video::Pixelformat_RGBA_DXT1)
						|| (i==video::Pixelformat_RGBA_DXT3)
						|| (i==video::Pixelformat_RGBA_DXT5))
						m_Caps.m_CompressedTextures=true;

					// Volume textures are not supported as render targets
					if (rtype[ii]!=D3DRTYPE_VOLUMETEXTURE) {
						hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_RENDERTARGET,rtype[ii],fmt);
						pTexflagarray[i].m_Rendertarget=(hr==D3D_OK);
						m_Caps.m_Rendertargets=true;
					} else pTexflagarray[i].m_Rendertarget=false;

					hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DYNAMIC,rtype[ii],fmt);
					pTexflagarray[i].m_Dynamic=(hr==D3D_OK);

					// For depth buffers, only depth formats are supported
					// Volume textures are not supported
					if (isDepthformat((video::Pixelformat)i) && (rtype[ii]!=D3DRTYPE_VOLUMETEXTURE)) {
						hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DEPTHSTENCIL,rtype[ii],fmt);
						pTexflagarray[i].m_Depth=(hr==D3D_OK);
					} else pTexflagarray[i].m_Depth=false;

					if ((m_D3DCaps.DevCaps2&D3DDEVCAPS2_DMAPNPATCH)!=0) {
						hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DMAP,rtype[ii],fmt);
						pTexflagarray[i].m_DisplacementMap=(hr==D3D_OK);
					} else pTexflagarray[i].m_DisplacementMap=false;
				}

				++pTexflags;
				++ii;
			}
		}

		base::log("D3D9Device::init()",base::Message) << "Creating D3D device\n";

		hr=m_pD3D9->CreateDevice(adapter,D3DDEVTYPE_HAL,m_hWindow,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_D3DPP,&m_pD3DDev9);

		if (FAILED(hr)) {
			base::log("D3D9Device::init()",base::Error) << "CreateDevice() failed\n";
			m_IsValid=false;
			return;
		}

		D3DVIEWPORT9 viewport;
		viewport.X=0;
		viewport.Y=0;
		viewport.Width=r.right-r.left;
		viewport.Height=r.bottom-r.top;
		viewport.MinZ=0;
		viewport.MaxZ=1;
		m_pD3DDev9->SetViewport(&viewport);

#ifdef D3D9DRV_USE_CG
		base::log("D3D9Device::init()",base::Message) << "Initializing Cg\n";
		{
			m_CgContext=cgCreateContext();
			cgD3D9SetDevice(m_pD3DDev9);

#ifdef D3D9DRV_CG_DEBUG_ERRORCALLBACK
			errctx=m_CgContext;
			cgSetErrorCallback(cgErrorCallback);
#endif

			m_CgVertexProfile=cgD3D9GetLatestVertexProfile();
			m_CgFragmentProfile=cgD3D9GetLatestPixelProfile();
			m_CgOptimalVertexProfileOptions=cgD3D9GetOptimalOptions(m_CgVertexProfile);
			m_CgOptimalFragmentProfileOptions=cgD3D9GetOptimalOptions(m_CgFragmentProfile);
		}
#endif
			
		base::log("D3D9Device::init()",base::Message) << "Probing queries\n";
		{
			IDirect3DQuery9* pQuery=0;
			hr=m_pD3DDev9->CreateQuery(D3DQUERYTYPE_OCCLUSION,&pQuery);
			m_Caps.m_HWOcclusionQueries=(hr==D3D_OK);
			if (pQuery) pQuery->Release();
		}
			
		{
			base::log("D3D9Device::init()",base::Message) << "Probing vertex program support\n";

			// Supported vertex programs
			{
				unsigned int version=HIBYTE(LOWORD(m_D3DCaps.VertexShaderVersion));
				unsigned int subversion=LOBYTE(LOWORD(m_D3DCaps.VertexShaderVersion));

				if (version>=3) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_vs20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_vs30");
				} else if (version>=2) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_vs11");
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_vs20");
				} else if (version>=1) {
					if (subversion>=1) m_Caps.m_SupportedProgramFormats.addString("d3d_vs11");
				}

				// HLSL support

				LPCSTR vsprofile=D3DXGetVertexShaderProfile(m_pD3DDev9);
				base::String hlslvsprofile;
				if (vsprofile) hlslvsprofile=vsprofile;

				if (hlslvsprofile.compare("vs_1_1",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
				} else if (hlslvsprofile.compare("vs_2_0",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
				} else if (hlslvsprofile.compare("vs_2_a",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs2a");
				} else if (hlslvsprofile.compare("vs_3_0",true) || (version>=3)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs2a");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs30");
				}

#ifdef D3D9DRV_USE_CG
				m_Caps.m_SupportedProgramFormats.addString("cg_vprogram");
#endif
			}

			base::log("D3D9Device::init()",base::Message) << "Probing fragment program support\n";

			// Supported fragment programs
			{
				unsigned int version=HIBYTE(LOWORD(m_D3DCaps.PixelShaderVersion));
				unsigned int subversion=LOBYTE(LOWORD(m_D3DCaps.PixelShaderVersion));

				if (version>=3) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps20");
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_ps30");
				} else if (version>=2) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_ps20");
				} else if (version>=1) {
					if (subversion>=1) m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					if (subversion>=2) m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					if (subversion>=3) m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					if (subversion>=4) m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
				}
				

				

				// HLSL support

				LPCSTR vsprofile=D3DXGetPixelShaderProfile(m_pD3DDev9);
				base::String hlslpsprofile;
				if (vsprofile) hlslpsprofile=vsprofile;

				if (hlslpsprofile.compare("ps_1_1",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
				} else if (hlslpsprofile.compare("ps_1_2",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
				} else if (hlslpsprofile.compare("ps_1_3",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
				} else if (hlslpsprofile.compare("ps_1_4",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
				} else if (hlslpsprofile.compare("ps_2_0",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
				} else if (hlslpsprofile.compare("ps_2_a",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps2a");
				} else if (hlslpsprofile.compare("ps_3_0",true) || (version>=3)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps2a");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps30");
				}

#ifdef D3D9DRV_USE_CG
				m_Caps.m_SupportedProgramFormats.addString("cg_fprogram");
#endif
			}

		} // Caps end

		m_Displaywidth=viewport.Width;
		m_Displayheight=viewport.Height;
		m_ColorbufferFormat=colorbufferformat;
		m_DepthstencilbufferFormat=depthstencilformat;

		m_pD3DDev9->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);

		fixedLighting(false);

		base::log("D3D9Device::init()",base::Message) << "Init complete\n";
	}

	ion_intptr D3D9Device::windowhandle() const
	{
		return (ion_intptr)m_hWindow;
	}

	void D3D9Device::windowtitle(const base::String& title)
	{
		SetWindowText(m_hWindow,title.cstr());
	}

	void D3D9Device::showCursor(const bool show)
	{
		if (m_CursorShown==show) return;
		ShowCursor(show);
		m_CursorShown=show;
	}

	bool D3D9Device::cursorShown() const
	{
		return m_CursorShown;
	}

	bool D3D9Device::testVertexProgram(const base::String& program,const base::String& format)
	{
		if (m_Caps.m_SupportedProgramFormats.find(format,true)==0) return false;

		return true;
	}

	bool D3D9Device::testFragmentProgram(const base::String& program,const base::String& format)
	{
		if (m_Caps.m_SupportedProgramFormats.find(format,true)==0) return false;

		return true;
	}

	ion_uint32 D3D9Device::displaywidth() const
	{
		return m_Displaywidth;
	}

	ion_uint32 D3D9Device::displayheight() const
	{
		return m_Displayheight;
	}

	video::Pixelformat D3D9Device::colorbufferFormat() const
	{
		return m_ColorbufferFormat;
	}

	video::Pixelformat D3D9Device::depthstencilbufferFormat() const
	{
		return m_DepthstencilbufferFormat;
	}










	void D3D9Device::clear(const ion_uint32 buffers,const base::Float4& color,const float zvalue,
		const ion_uint32 stencilvalue)
	{
		ion_uint32 clearflags=
			(((buffers&video::Clear_Colorbuffer)!=0) ? D3DCLEAR_TARGET  : 0)|
			(((buffers&video::Clear_Stencilbuffer)!=0) ? D3DCLEAR_STENCIL  : 0)|
			(((buffers&video::Clear_Depthbuffer)!=0) ? D3DCLEAR_ZBUFFER  : 0);

		ion_uint32 col=
			(((ion_uint32)(color[0]*255.0f))<<24)|
			(((ion_uint32)(color[1]*255.0f))<<16)|
			(((ion_uint32)(color[2]*255.0f))<<8)|
			(((ion_uint32)(color[3]*255.0f)));

		m_pD3DDev9->Clear(0,NULL,clearflags,col,zvalue,stencilvalue);
	}

	void D3D9Device::begin()
	{
		m_pD3DDev9->BeginScene();
	}

	void D3D9Device::end()
	{
		m_pD3DDev9->EndScene();
	}

	void D3D9Device::update()
	{
		m_pD3DDev9->Present(0,0,0,0);
	}

	bool D3D9Device::handleWindow()
	{
		m_WindowActivationState=video::NoWindowActivationChange;
		MSG message;
		if (PeekMessage(&message,m_hWindow,0,0,PM_REMOVE)!=0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
			return true;
		} else return false;
	}

	bool D3D9Device::quitting() const
	{
		return m_Quitting;
	}

	video::WindowActivationState D3D9Device::windowActivationState() const
	{
		return m_WindowActivationState;
	}

	void D3D9Device::setViewport(const ion_uint32 newx,const ion_uint32 newy,
		const ion_uint32 newwidth,const ion_uint32 newheight)
	{
		D3DVIEWPORT9 vp;
		vp.X=newx;
		vp.Y=newy;
		vp.Width=newwidth;
		vp.Height=newheight;
		vp.MinZ=0;
		vp.MaxZ=1;
		m_pD3DDev9->SetViewport(&vp);
	}

	bool D3D9Device::resizeFramebuffer(const ion_uint32 newwidth,const ion_uint32 newheight,video::ViewportResizingCallback* callback)
	{
		rendercolortarget(0);
		renderdepthstenciltarget(0);
		if (m_pFramebuffer) m_pFramebuffer->Release(); m_pFramebuffer=0;

		m_D3DPP.BackBufferWidth=m_D3DPP.Windowed ? 0 : newwidth;
		m_D3DPP.BackBufferHeight=m_D3DPP.Windowed ? 0 : newheight;

		// Notify all resources
		{
			std::list<D3D9Texture2D* > m_2DTextures;
			std::list<D3D9Cubemap* > m_Cubemaps;
			std::list<D3D9Vertexstream* > m_Vertexstreams;
			std::list<D3D9Indexstream* > m_Indexstreams;

			std::list<D3D9Texture2D* >::iterator it2dtex=m_pResources->m_2DTextures.begin();
			std::list<D3D9Cubemap* >::iterator itcubemap=m_pResources->m_Cubemaps.begin();
			std::list<D3D9Vertexstream* >::iterator itvstr=m_pResources->m_Vertexstreams.begin();
			std::list<D3D9Indexstream* >::iterator itistr=m_pResources->m_Indexstreams.begin();

			for (;it2dtex!=m_pResources->m_2DTextures.end();++it2dtex) (*it2dtex)->d3d9releaseBeforeReset();
			for (;itcubemap!=m_pResources->m_Cubemaps.end();++itcubemap) (*itcubemap)->d3d9releaseBeforeReset();
			for (;itvstr!=m_pResources->m_Vertexstreams.end();++itvstr) (*itvstr)->d3d9releaseBeforeReset();
			for (;itistr!=m_pResources->m_Indexstreams.end();++itistr) (*itistr)->d3d9releaseBeforeReset();
		}
		
		HRESULT hr=m_pD3DDev9->Reset(&m_D3DPP);
		do {
			if (hr==D3DERR_DRIVERINTERNALERROR) return false;
			hr=m_pD3DDev9->TestCooperativeLevel();

			if (hr==D3DERR_DEVICENOTRESET) hr=m_pD3DDev9->Reset(&m_D3DPP);
		} while (hr!=D3D_OK);

		if (callback) callback->checkIonVideodeviceResources();

		return true;
	}







	video::Vertexstream* D3D9Device::createVertexstream(const ion_uint32 numVertices,const video::Vertexformat& format,
		const ion_uint32 flags,const video::Memorypool pool)
	{
		return new D3D9Vertexstream(*this,format,flags,pool,numVertices,0);
	}

	video::Indexstream* D3D9Device::createIndexstream(const ion_uint32 numIndices,const video::Indexformat format,
		const ion_uint32 flags,const video::Memorypool pool)
	{
		return new D3D9Indexstream(*this,format,flags,pool,numIndices);
	}

	video::Texture2D* D3D9Device::create2DTexture(const base::String& identifier,
		const ion_uint32 width,const ion_uint32 height,
		const ion_uint32 levels,const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new D3D9Texture2D(*this,identifier,width,height,levels,flags,format,pool);
	}

	video::Cubemap* D3D9Device::createCubemap(const base::String& identifier,
		const ion_uint32 edgelength,const ion_uint32 levels,
		const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new D3D9Cubemap(*this,identifier,edgelength,levels,flags,format,pool);
	}

	video::Occlusionquery* D3D9Device::createOcclusionquery()
	{
		return 0;
	}

	video::Program* D3D9Device::createVertexProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
#ifdef D3D9DRV_USE_CG
		if (format=="cg_vprogram")
			return new CGD3D9Vertexprogram(*this,identifier,entrypoint,program);
		else
#endif
			return new D3D9Vertexprogram(*this,identifier,entrypoint,program,format);
	}

	video::Program* D3D9Device::createFragmentProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
#ifdef D3D9DRV_USE_CG
		if (format=="cg_fprogram")
			return new CGD3D9Fragmentprogram(*this,identifier,entrypoint,program);
		else
#endif
			return new D3D9Fragmentprogram(*this,identifier,entrypoint,program,format);
	}








	void D3D9Device::drawPrimitives(const video::PrimitivesType type,
		video::Vertexstream& vertexstream,const ion_uint32 firstElement,
		const ion_uint32 numElements)
	{
		vertexstream.bind();
		m_pD3DDev9->DrawPrimitive(d3dprimitivetype(type),firstElement,numElements);
	}

	void D3D9Device::drawIndexedPrimitives(
		const video::PrimitivesType type,
		video::Vertexstream& vertexstream,
		video::Indexstream& indexstream,
		const ion_uint32 indexOffset,
		const ion_uint32 numElements)
	{
		vertexstream.bind();
		indexstream.bind();
		HRESULT hr=m_pD3DDev9->DrawIndexedPrimitive(
			d3dprimitivetype(type),0,0,vertexstream.capacity(),indexOffset,numElements);

		if (FAILED(hr)) {
			base::log("D3D9Device::drawIndexedPrimitives()",base::Error);
			base::logstream() << DXGetErrorString9(hr) << " " << DXGetErrorDescription9(hr) << "\n";
			base::logstream() << "Index stream capacity: " << indexstream.capacity() << "\n";
			base::logstream() << "Vertex stream capacity: " << vertexstream.capacity() << "\n";
			base::logstream() << "Index offset: " << indexOffset << "\n";
			base::logstream() << "Num elements: " << numElements << "\n";
		}
	}





	void D3D9Device::blending(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ALPHABLENDENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::zbuffer(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZENABLE,state ? D3DZB_TRUE : D3DZB_FALSE);
	}

	void D3D9Device::zwrite(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZWRITEENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::stencilbuffer(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::stencilwritemask(const ion_uint32 stencilmask)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILMASK,stencilmask);
	}

	void D3D9Device::culling(const bool state)
	{
		m_CullingEnabled=state;
		m_pD3DDev9->SetRenderState(D3DRS_CULLMODE,state ? m_Cullmode : D3DCULL_NONE);
	}

	void D3D9Device::cullmode(const video::Cullingmode mode)
	{
		switch (mode) {
		case video::Cullingmode_Clockwise:m_Cullmode=D3DCULL_CW; break;
		case video::Cullingmode_Counterclockwise:m_Cullmode=D3DCULL_CCW; break;
		}

		if (m_CullingEnabled)
			m_pD3DDev9->SetRenderState(D3DRS_CULLMODE,m_Cullmode);
	}

	void D3D9Device::blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend)
	{
		m_pD3DDev9->SetRenderState(D3DRS_SRCBLEND,d3dblendparameter(srcblend));
		m_pD3DDev9->SetRenderState(D3DRS_DESTBLEND,d3dblendparameter(destblend));
	}

	void D3D9Device::zfunc(const video::CompareFunc func)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZFUNC,d3dcomparefunc(func));
	}

	void D3D9Device::stencilfunc(const video::CompareFunc func,const ion_uint32 ref,const ion_uint32 writemask)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILFUNC,d3dcomparefunc(func));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILREF,ref);
		m_pD3DDev9->SetRenderState(D3DRS_STENCILMASK,writemask);
	}

	void D3D9Device::stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILFAIL,d3dstenciloperation(opfail));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILZFAIL,d3dstenciloperation(opzfail));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILPASS,d3dstenciloperation(oppass));
	}

	void D3D9Device::drawingmode(const video::Drawingmode mode)
	{
		switch (mode) {
			case video::Drawingmode_Points:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT); break;
			case video::Drawingmode_Lines:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME); break;
			case video::Drawingmode_Faces:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID); break;
			default:break;
		}
	}

	void D3D9Device::solidshading(const video::Solidshading type)
	{
		switch (type) {
			case video::Solidshading_Flat:m_pD3DDev9->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT); break;
			case video::Solidshading_Smooth:m_pD3DDev9->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD); break;
			default:break;
		}
	}

	void D3D9Device::depthbias(const float scaledbias,const float uniformbias)
	{
		m_pD3DDev9->SetRenderState(D3DRS_DEPTHBIAS,*((DWORD*)(&uniformbias)) );
		m_pD3DDev9->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,*((DWORD*)(&scaledbias)) );
	}

	void D3D9Device::scissortest(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_SCISSORTESTENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::scissorRectangle(const math::IntRectangle& rectangle)
	{
		long l=rectangle.left();
		long t=rectangle.top();
		long r=rectangle.left()+rectangle.width()-1;
		long b=rectangle.top()+rectangle.height()-1;

		l=(l<0) ? 0 : l;
		t=(t<0) ? 0 : t;

		r=(r>=(long)m_Displaywidth) ? (long)(m_Displaywidth-1) : r;
		b=(b>=(long)m_Displayheight) ? (long)(m_Displayheight-1) : b;

		RECT rect;
		rect.left=l;
		rect.top=t;
		rect.right=r;
		rect.bottom=b;

		m_pD3DDev9->SetScissorRect(&rect);
	}

	void D3D9Device::rendercolortarget(const video::Texture::Texsurface* pRendertarget)
	{
		if (m_pRendercolortarget==pRendertarget) return; // texture surface already set as rendertarget -> do nothing
	
		if (pRendertarget) {
			if (video::isColorformat(pRendertarget->m_pTexture->actualpixelformat())) {
				LPDIRECT3DSURFACE9 pSurf=((D3D9TextureSurface*)pRendertarget)->m_pD3D9Surface;

				if (m_pFramebuffer==0)
					m_pD3DDev9->GetRenderTarget(0,&m_pFramebuffer);
				m_pD3DDev9->SetRenderTarget(0,pSurf);
				m_pRendertargetSurface=pSurf;
			}
		} else {
			m_pD3DDev9->SetRenderTarget(0,m_pFramebuffer);
			m_pRendertargetSurface=0;
		}

		m_pRendercolortarget=pRendertarget;
	}
	
	const video::Texture::Texsurface* D3D9Device::rendercolortarget() const
	{
		return m_pRendercolortarget;
	}

	void D3D9Device::renderdepthstenciltarget(const video::Texture::Texsurface* pRendertarget)
	{
		if (m_pRenderdepthstenciltarget==pRendertarget) return; // texture surface already set as depthstenciltarget -> do nothing
	
		if (pRendertarget) {
			if (video::isDepthformat(pRendertarget->m_pTexture->actualpixelformat())) {
				LPDIRECT3DSURFACE9 pSurf=((D3D9TextureSurface*)pRendertarget)->m_pD3D9Surface;

				if (m_pDepthstencilbuffer==0)
					m_pD3DDev9->GetDepthStencilSurface(&m_pDepthstencilbuffer);
				m_pD3DDev9->SetDepthStencilSurface(pSurf);
				m_pDepthstenciltargetSurface=pSurf;
			}
		} else {
			m_pD3DDev9->SetDepthStencilSurface(m_pDepthstencilbuffer);
			m_pDepthstenciltargetSurface=0;
		}

		m_pRenderdepthstenciltarget=pRendertarget;
	}
	
	const video::Texture::Texsurface* D3D9Device::renderdepthstenciltarget() const
	{
		return m_pRenderdepthstenciltarget;
	}

	unsigned long D3D9Device::samplerstageParameterInt(const ion_uint32 texstage,const video::Samplerstageparameter texparam) const
	{
		return 0;
	}

	float D3D9Device::samplerstageParameterFloat(const ion_uint32 texstage,const video::Samplerstageparameter texparam) const
	{
		return 0;
	}

	void D3D9Device::samplerstageParameterInt(const ion_uint32 texstage,const video::Samplerstageparameter texparam,const unsigned long value)
	{
		const char *where="D3D9Device::samplerstageParameterInt()";

		switch (texparam) {

			case video::Samplerstageparameter_MaxAnisotropy:
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MAXANISOTROPY,value); break;

			case video::Samplerstageparameter_Minfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture min filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MINFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_Magfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture mag filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MAGFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_Mipfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture mip filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MIPFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_UWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture U address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSU,d3dadr);
			} break;

			case video::Samplerstageparameter_VWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture V address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSV,d3dadr); break;
			} break;

			case video::Samplerstageparameter_WWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture W address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSW,d3dadr); break;
			} break;

		}
	}

	void D3D9Device::bindTexture(const ion_uint32 samplerstage,const video::Texture* texVar)
	{
		m_pD3DDev9->SetTexture(samplerstage,texVar ? (LPDIRECT3DTEXTURE9)(texVar->texHandle()) : 0);
	}

	void D3D9Device::samplerstageParameterFloat(const ion_uint32 texstage,const video::Samplerstageparameter texparam,const float value)
	{
	}


	unsigned long D3D9Device::texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	float D3D9Device::texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	void D3D9Device::texstageParameterInt(const ion_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value)
	{
		switch (texparam) {

			case video::Texstageparameter_Colorop: {
				D3DTEXTUREOP d3dtexop=d3dtextureop((video::Texstageoperation)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLOROP,d3dtexop);
			} break;

			case video::Texstageparameter_Colorarg1: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLORARG1,d3dtexarg);
			} break;

			case video::Texstageparameter_Colorarg2: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLORARG2,d3dtexarg);
			} break;

			case video::Texstageparameter_Alphaop: {
				D3DTEXTUREOP d3dtexop=d3dtextureop((video::Texstageoperation)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAOP,d3dtexop);
			} break;

			case video::Texstageparameter_Alphaarg1: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAARG1,d3dtexarg);
			} break;

			case video::Texstageparameter_Alphaarg2: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAARG2,d3dtexarg);
			} break;

			case video::Texstageparameter_Texcoordindex: {
				m_Texcoordindexmodes[texstage]=value;
				DWORD dw=m_Texcoordindexmodes[texstage]|m_Texcoordgenmodes[texstage];
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_TEXCOORDINDEX,dw);
			} break;

			case video::Texstageparameter_Textransformations: {
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_TEXTURETRANSFORMFLAGS,value);
			} break;

			case video::Texstageparameter_Constant: {
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_CONSTANT,value);
			} break;

			case video::Texstageparameter_Texcoordgeneration: {
				m_Texcoordgenmodes[texstage]=d3dtexcoordgen((video::Texcoordgeneration)value);
				DWORD dw=m_Texcoordindexmodes[texstage]|m_Texcoordgenmodes[texstage];
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_TEXCOORDINDEX,dw);
			} break;

		}
	}

	void D3D9Device::texstageParameterFloat(const ion_uint32 texstage,const video::Texstageparameter texparam,const float value)
	{
	}


	video::Program* D3D9Device::vertexprogram()
	{
		return m_pCurrentVertexProgram;
	}

	video::Program* D3D9Device::fragmentprogram()
	{
		return m_pCurrentFragmentProgram;
	}

	void D3D9Device::vertexprogram(video::Program* pVertexprogram)
	{
		if (m_pCurrentVertexProgram==pVertexprogram) return;
		m_pCurrentVertexProgram=pVertexprogram;
	}

	void D3D9Device::fragmentprogram(video::Program* pFragmentprogram)
	{
		if (m_pCurrentFragmentProgram==pFragmentprogram) return;
		m_pCurrentFragmentProgram=pFragmentprogram;
	}

	video::Vertexstream* D3D9Device::vertexstream()
	{
		return m_pCurrentVertexstream;
	}

	video::Indexstream* D3D9Device::indexstream()
	{
		return m_pCurrentIndexstream;
	}

	void D3D9Device::vertexstream(video::Vertexstream* pStream)
	{
		m_pCurrentVertexstream=pStream;
	}

	void D3D9Device::indexstream(video::Indexstream* pStream)
	{
		m_pCurrentIndexstream=pStream;
	}

	void D3D9Device::activateFFPipeline(const video::Fixedfunctionactivationtype pipeline)
	{
		if (pipeline==video::FixedfunctionVertex) {
			if (m_pCurrentVertexProgram!=0) {
				m_pCurrentVertexProgram=0;
				m_pD3DDev9->SetVertexShader(0);
			}
		} else {
			if (m_pCurrentFragmentProgram!=0) {
				m_pCurrentFragmentProgram=0;
				m_pD3DDev9->SetPixelShader(0);
			}
		}
	}

	void D3D9Device::fixedSetMaterialAmbientColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Ambient.a=color[0];
		m_D3D9FFMaterial.Ambient.r=color[1];
		m_D3D9FFMaterial.Ambient.g=color[2];
		m_D3D9FFMaterial.Ambient.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialDiffuseColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Diffuse.a=color[0];
		m_D3D9FFMaterial.Diffuse.r=color[1];
		m_D3D9FFMaterial.Diffuse.g=color[2];
		m_D3D9FFMaterial.Diffuse.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialSpecularColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Specular.a=color[0];
		m_D3D9FFMaterial.Specular.r=color[1];
		m_D3D9FFMaterial.Specular.g=color[2];
		m_D3D9FFMaterial.Specular.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialEmissiveColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Emissive.a=color[0];
		m_D3D9FFMaterial.Emissive.r=color[1];
		m_D3D9FFMaterial.Emissive.g=color[2];
		m_D3D9FFMaterial.Emissive.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialShininess(const float shininess)
	{
		m_D3D9FFMaterial.Power=shininess;
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	ion_uint32 D3D9Device::fixedMaxTextures()
	{
		return 0;
	}

	ion_uint32 D3D9Device::fixedMaxTextureStages()
	{
		return m_Caps.m_MaxTextureBlendStages;
	}

	void D3D9Device::fixedLighting(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_LIGHTING,state ? TRUE : FALSE);
	}

	void D3D9Device::fixedSetLightEnabled(const ion_uint32 lightNr,const bool state)
	{
		m_pD3DDev9->LightEnable(lightNr,state ? TRUE : FALSE);
	}

	void D3D9Device::fixedSetLightProperties(const ion_uint32 lightNr,const video::FFLightProperties& properties)
	{
		D3DLIGHT9 d3dlight9;
		memset(&d3dlight9,0,sizeof(d3dlight9));
		switch (properties.m_Lighttype) {
			case video::Lighttype_Directional:d3dlight9.Type=D3DLIGHT_DIRECTIONAL; break;
			case video::Lighttype_Point:d3dlight9.Type=D3DLIGHT_POINT; break;
			case video::Lighttype_Spot:d3dlight9.Type=D3DLIGHT_SPOT; break;
		}
		d3dlight9.Position.x=properties.m_Position.x();
		d3dlight9.Position.y=properties.m_Position.y();
		d3dlight9.Position.z=properties.m_Position.z();
		d3dlight9.Direction.x=properties.m_Direction.x();
		d3dlight9.Direction.y=properties.m_Direction.y();
		d3dlight9.Direction.z=properties.m_Direction.z();
		d3dlight9.Diffuse.a=properties.m_Color[0];
		d3dlight9.Diffuse.r=properties.m_Color[1];
		d3dlight9.Diffuse.g=properties.m_Color[2];
		d3dlight9.Diffuse.b=properties.m_Color[3];
		d3dlight9.Range=properties.m_Range;
		d3dlight9.Falloff=properties.m_Falloff;
		d3dlight9.Attenuation0=properties.m_ConstantAttenuation;
		d3dlight9.Attenuation1=properties.m_LinearAttenuation;
		d3dlight9.Attenuation2=properties.m_QuadraticAttenuation;
		d3dlight9.Theta=properties.m_InnerConeAngle;
		d3dlight9.Phi=properties.m_OuterAngle;

		m_pD3DDev9->SetLight(lightNr,&d3dlight9);
	}

	void D3D9Device::fixedSetProjectionMatrix(const ion::base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_PROJECTION,&m);
	}

	void D3D9Device::fixedSetWorldMatrix(const ion::base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_WORLD,&m);
	}

	void D3D9Device::fixedSetViewMatrix(const ion::base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_VIEW,&m);
	}

	void D3D9Device::fixedSetTextureMatrix(const ion_uint32 texstage,const base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);

		m_pD3DDev9->SetTransform(
			(D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0+((texstage<=7) ? texstage : 7)),&m);
	}

	ion_uint32 D3D9Device::fixedMaxLights() const
	{
		return m_D3DCaps.MaxActiveLights;
	}

	void D3D9Device::fixedFog(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_FOGENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::fixedSetFogStart(const float start)
	{
		m_pD3DDev9->SetRenderState(D3DRS_FOGSTART,*((DWORD*)(&start)));
	}

	void D3D9Device::fixedSetFogEnd(const float end)
	{
		m_pD3DDev9->SetRenderState(D3DRS_FOGEND,*((DWORD*)(&end)));
	}

	void D3D9Device::fixedSetFogColor(const base::Float3& color)
	{
		DWORD dw=0;
		dw|=(((DWORD)(color[0]*255.0f))&0xff)<<16;
		dw|=(((DWORD)(color[1]*255.0f))&0xff)<<8;
		dw|=((DWORD)(color[2]*255.0f))&0xff;
		m_pD3DDev9->SetRenderState(D3DRS_FOGCOLOR,dw);
	}

	void D3D9Device::fixedUseRangeFog(const bool use)
	{
		m_pD3DDev9->SetRenderState(D3DRS_RANGEFOGENABLE,use ? TRUE : FALSE);
	}

#ifdef D3D9DRV_USE_CG
	CGcontext D3D9Device::cgContext()
	{
		return m_CgContext;
	}

	CGprofile D3D9Device::cgVertexProfile()
	{
		return m_CgVertexProfile;
	}

	CGprofile D3D9Device::cgFragmentProfile()
	{
		return m_CgFragmentProfile;
	}

	const char ** D3D9Device::cgOptimalVertexProfileOptions()
	{
		return m_CgOptimalVertexProfileOptions;
	}

	const char ** D3D9Device::cgOptimalFragmentProfileOptions()
	{
		return m_CgOptimalFragmentProfileOptions;
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
	return new ion::d3d9drv::D3D9Device;
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* videodeviceDescription()
{
	return "Direct3D 9 videodevice v0.9 build 2006-03-24 21:08";
}

