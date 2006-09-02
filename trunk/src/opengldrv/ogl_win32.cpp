#include "iongl.hh"

#include <windows.h>
#include <GL/gl.h>
#include <vector>

#include "../base/log.hh"
#include "ogldevice.hh"

namespace ion {
namespace opengldrv {




	struct OGLDevice::Sysprocdata
	{
		HWND m_hWnd;
		UINT m_Msg;
		WPARAM m_wParam;
		LPARAM m_lParam;
	};

	ion_intptr OGLDevice::sysproc(Sysprocdata &data)
	{
		switch(data.m_Msg)
		{
			case WM_ACTIVATEAPP:m_WindowActivationState=(data.m_wParam==FALSE) ?
									video::WindowDeactivated : video::WindowActivated;break;
			case WM_ACTIVATE:m_WindowActivationState=(LOWORD(data.m_wParam)==WA_INACTIVE) ?
								video::WindowDeactivated : video::WindowActivated;break;
			case WM_DESTROY:m_Quitting=true;break;
			default:break;
		}

		return DefWindowProc(data.m_hWnd,data.m_Msg,data.m_wParam,data.m_lParam);
	}


	namespace ionopengldeviceWin32 {



		LRESULT CALLBACK ionopenglWndproc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
	#ifdef _MSC_VER
	#pragma warning(disable:4312)
	#endif
			// TODO: Verify this
			OGLDevice *pOGLDevice=(OGLDevice*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	#ifdef _MSC_VER
	#pragma warning(disable:4312)
	#endif

			// TODO: Check if the pointer really points to an OGLDevice instance
			if (pOGLDevice==0) return DefWindowProc(hWnd,uMsg,wParam,lParam);
			else {
				OGLDevice::Sysprocdata sysprocdata;
				sysprocdata.m_hWnd=hWnd;
				sysprocdata.m_Msg=uMsg;
				sysprocdata.m_wParam=wParam;
				sysprocdata.m_lParam=lParam;
				return pOGLDevice->sysproc(sysprocdata);
			}
		}



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
			windowclass.lpfnWndProc=ionopenglWndproc;
			windowclass.lpszClassName="ion_Win32_openglwindow_5bhjubxhg2";
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
				UnregisterClass("ion_Win32_openglwindow_5bhjubxhg2",GetModuleHandle(0));
			}
		}
	}





















	struct OGLDevice::OGLInternaldata
	{
		HWND m_hWindow;
		HDC m_hDC;
		HGLRC m_hGLRC;
		bool m_Fullscreen;

		struct GDIEnum {
			ion_uint32 m_Adapterindex,m_Modecount;
		};

		std::vector<GDIEnum> m_Adaptermodescount;
	};

	ion_intptr OGLDevice::windowhandle() const
	{
		return (ion_intptr)(m_pOGLInternaldata->m_hWindow);
	}

	void OGLDevice::windowtitle(const base::String& title)
	{
		SetWindowText(m_pOGLInternaldata->m_hWindow,title.cstr());
	}

	void OGLDevice::showCursor(const bool show)
	{
		if (m_CursorShown==show) return;
		ShowCursor(show);
		m_CursorShown=show;
	}

	bool OGLDevice::cursorShown() const
	{
		return m_CursorShown;
	}

	void OGLDevice::initGL(
		const ion_uint32 width,const ion_uint32 height,
		const base::String& title,const bool fullscreen,void* displayhandle,
		const ion_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ion_uint32 Hz,const bool vsync)
	{
		if (m_pOGLInternaldata) return;
		m_pOGLInternaldata=new OGLInternaldata;
		m_pOGLInternaldata->m_Fullscreen=fullscreen;




		unsigned int bpp=0,alpha=0,bitsperpel=0,depth=0,stencil=0;

		switch (colorbufferformat) {
			case video::Pixelformat_XRGB1555:
			case video::Pixelformat_RGB565:
				alpha=0; bpp=bitsperpel=16; break;
			case video::Pixelformat_ARGB1555:
				alpha=1; bpp=bitsperpel=16; break;
			case video::Pixelformat_ARGB4444:
				alpha=4; bpp=bitsperpel=16; break;
			case video::Pixelformat_RGB888:
				alpha=0; bpp=bitsperpel=24; break;
			
			case video::Pixelformat_XRGB8888:
			case video::Pixelformat_XBGR8888:
				alpha=0; bpp=24; bitsperpel=32; break;

			case video::Pixelformat_ARGB8888:
			case video::Pixelformat_ABGR8888:
				alpha=8; bpp=24; bitsperpel=32; break;
			default:
				base::log("OGLDevice::initGL()",base::FatalError) << "Invalid pixelformat for the colorbuffer\n";
				return;
		}

		switch (depthstencilformat) {
			case video::Pixelformat_D16:depth=16; stencil=0; break;
			case video::Pixelformat_D24:depth=24; stencil=0; break;
			case video::Pixelformat_D32:depth=32; stencil=0; break;
			case video::Pixelformat_D15S1:depth=16; stencil=1; break;
			case video::Pixelformat_D24S4:depth=24; stencil=4; break;
			case video::Pixelformat_D24S8:depth=24; stencil=8; break;
			case video::Pixelformat_FD24S8:depth=24; stencil=8; break;
			default:
				base::log("OGLDevice::initGL()",base::FatalError) << "Invalid pixelformat for the depthstencilbuffer\n";
				return;
		}

		PIXELFORMATDESCRIPTOR pfd={
			sizeof(PIXELFORMATDESCRIPTOR),							// Size Of This Pixel Format Descriptor
				1,														// Version Number
				PFD_DRAW_TO_WINDOW |									// Format Must Support Window
				PFD_SUPPORT_OPENGL |									// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,										// Must Support Double Buffering
				PFD_TYPE_RGBA,											// Request An RGBA Format
				bpp,													// Select Our Color Depth
				0, 0, 0, 0, 0, 0,										// Color Bits Ignored
				alpha,														// No Alpha Buffer
				0,														// Shift Bit Ignored
				0,														// No Accumulation Buffer
				0, 0, 0, 0,												// Accumulation Bits Ignored
				depth,														// 16Bit Z-Buffer (Depth Buffer)
				stencil,														// No Stencil Buffer
				0,														// No Auxiliary Buffer
				PFD_MAIN_PLANE,											// Main Drawing Layer
				0,														// Reserved
				0, 0, 0													// Layer Masks Ignored
		};


		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bitsperpel;
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			/*DISPLAY_DEVICE displaydev;
			displaydev.cb=sizeof(displaydev);
			EnumDisplayDevices(0,adapter,&displaydev,0);

			ChangeDisplaySettingsEx(displaydev.DeviceName,&dmScreenSettings,
				(HWND)(m_pOGLInternaldata->m_hWindow),CDS_FULLSCREEN,0);*/
			ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
		}




		ionopengldeviceWin32::registerClass();



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

		m_pOGLInternaldata->m_hWindow = CreateWindowEx(0,
			"ion_Win32_openglwindow_5bhjubxhg2",
			title.cstr(), flags, windowX, windowY, windowWidth, windowHeight,
			0, 0, GetModuleHandle(0), 0);

	#ifdef _MSC_VER
	#pragma warning(disable:4244)
	#endif
		SetWindowLongPtr(m_pOGLInternaldata->m_hWindow,GWL_USERDATA,(LONG_PTR)this);
	#ifdef _MSC_VER
	#pragma warning(default:4244)
	#endif

		ShowWindow(m_pOGLInternaldata->m_hWindow,SW_SHOW);







		m_pOGLInternaldata->m_hDC=GetDC(m_pOGLInternaldata->m_hWindow);
		GLuint PixelFormat=ChoosePixelFormat(m_pOGLInternaldata->m_hDC,&pfd);
		SetPixelFormat(m_pOGLInternaldata->m_hDC,PixelFormat,&pfd);
		m_pOGLInternaldata->m_hGLRC=wglCreateContext(m_pOGLInternaldata->m_hDC);
		wglMakeCurrent(m_pOGLInternaldata->m_hDC,m_pOGLInternaldata->m_hGLRC);

		m_IsValid=true;
	}

	bool OGLDevice::handleWindow()
	{
		m_WindowActivationState=video::NoWindowActivationChange;
		MSG message;
		if( PeekMessage(&message,m_pOGLInternaldata->m_hWindow,0,0,PM_NOREMOVE)) {
			if (!GetMessage(&message,NULL,0,0)) m_Quitting=true;
			else {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			return true;
		} else return false;
	}

	void OGLDevice::updateGL()
	{
		SwapBuffers(m_pOGLInternaldata->m_hDC);
	}

	void OGLDevice::shutdownGL()
	{
		if (!m_pOGLInternaldata) return;

		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(m_pOGLInternaldata->m_hGLRC);
		ReleaseDC(m_pOGLInternaldata->m_hWindow,m_pOGLInternaldata->m_hDC);

		if (m_pOGLInternaldata->m_Fullscreen) ChangeDisplaySettings(NULL,0);

		DestroyWindow(m_pOGLInternaldata->m_hWindow);

		ionopengldeviceWin32::unregisterClass();

		if (m_pOGLInternaldata) delete m_pOGLInternaldata;
		m_pOGLInternaldata=0;
	}

}
}
