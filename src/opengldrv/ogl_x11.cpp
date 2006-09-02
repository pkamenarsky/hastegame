#include "../base/log.hh"
#include "iongl.hh"
#include "ogldevice.hh"

/* Standard OpenGL/GLX header files */
#include <GL/glx.h>
#include <GL/gl.h>

/* Headers needed for keys used in the program */
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>

namespace ion {
namespace opengldrv {

	struct OGLDevice::Sysprocdata
	{
	};

	ion_intptr OGLDevice::sysproc(Sysprocdata &data)
	{
		return 0;
	}

/* Define our window structure here
 *
 * Description of Structure Members:
 *
 * Display 				*dpy: 	This is the display. In X Windows, a display considers
 *								everything. Monitor, Mouse, Keyboard.
 * int	  				screen:	Which window we are rendering too ?
 * Window 				win:	This is our drawable surrface
 * GLXContext 			ctx:	Stores information about our OpenGL Context
 * XSetWindowAttributes attr:
 * BOOL					fs:		This is the fullscreen flag. Weither or not we have a fullscreen
 *								or not. 
 * XF86VidModeModeInfo 	deskmode:
 * int					x,y:
 * unsigned int			width:	Width of the window
 * unsigned int			height:	Height of the window
 * unsigned int 		bpp:	Bits Per Pixel. With XFree86, highest we can acheive is 24.	
 */

	struct OGLDevice::OGLInternaldata
	{
		Display 			*dpy;
		int				screen;
		Window				win;
		GLXContext			ctx;
		XSetWindowAttributes		attr;
		BOOL				fs;
		XF86VidModeModeInfo		deskMode;
		int				x,y;
		unsigned int			width, height;
		unsigned int			bpp;
		Atom wmDelete;
	};

	ion_intptr OGLDevice::windowhandle() const
	{
		return (ion_intptr)(m_pOGLInternaldata->win);
	}
	
	void OGLDevice::windowtitle(const base::String& title)
	{
		XSetStandardProperties(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, title.cstr(),
			title.cstr(), None, NULL, 0, NULL);
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

		int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4,
							GLX_GREEN_SIZE, 4,
							GLX_BLUE_SIZE, 4,
							GLX_DEPTH_SIZE, 16,
							None};

		int attrListDbl[] = {GLX_RGBA, GLX_DOUBLEBUFFER,
							GLX_RED_SIZE, 4,
							GLX_GREEN_SIZE, 4,
							GLX_BLUE_SIZE, 4,
							GLX_DEPTH_SIZE, 16,
							None};

		XVisualInfo *vi;
		Colormap cmap;
		int dpyWidth, dpyHeight;
		int i;
		int vidModeMajorVersion, vidModeMinorVersion;
		XF86VidModeModeInfo **modes=0;
		int modeNum;
		int bestMode;
		Atom wmDelete;
		Window winDummy;
		unsigned int borderDummy;
	
		m_pOGLInternaldata->fs = fullscreen;
		bestMode = 0;
	
		m_pOGLInternaldata->dpy = XOpenDisplay(0);
		m_pOGLInternaldata->screen = DefaultScreen(m_pOGLInternaldata->dpy);
		XF86VidModeQueryVersion(m_pOGLInternaldata->dpy, &vidModeMajorVersion,
								&vidModeMinorVersion);
	
		XF86VidModeGetAllModeLines(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, &modeNum, &modes);
	
		if (modes!=0) {
			m_pOGLInternaldata->deskMode = *modes[0];
	
			for (i = 0; i < modeNum; i++)
			{
				if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
				{
					bestMode = i;
				}
			}
		}
	
		vi = glXChooseVisual(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, attrListDbl);
		if(NULL == vi)
		{
			vi = glXChooseVisual(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, attrListSgl);
			base::log("OGLDevice::initGL()",base::Message) << "Singlebuffered : true\n";
		}
		else
		{
			base::log("OGLDevice::initGL()",base::Message) << "Doublebuffered : true\n";
		}
	
		m_pOGLInternaldata->ctx = glXCreateContext(m_pOGLInternaldata->dpy, vi, 0, GL_TRUE);
	
		cmap = XCreateColormap(m_pOGLInternaldata->dpy, RootWindow(m_pOGLInternaldata->dpy, vi->screen),vi->visual, AllocNone);
		m_pOGLInternaldata->attr.colormap = cmap;
		m_pOGLInternaldata->attr.border_pixel = 0;
	
		if(m_pOGLInternaldata->fs && (modes!=0))
		{
			XF86VidModeSwitchToMode(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, modes[bestMode]);
			XF86VidModeSetViewPort(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, 0, 0);
			dpyWidth = modes[bestMode]->hdisplay;
			dpyHeight = modes[bestMode]->vdisplay;
			XFree(modes);
			m_pOGLInternaldata->attr.override_redirect = True;
			m_pOGLInternaldata->attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
									StructureNotifyMask;
			m_pOGLInternaldata->win = XCreateWindow(m_pOGLInternaldata->dpy, RootWindow(m_pOGLInternaldata->dpy, vi->screen),
									0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
									CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
									&m_pOGLInternaldata->attr);
			wmDelete = XInternAtom(m_pOGLInternaldata->dpy, "WM_DELETE_WINDOW", True);
			XSetWMProtocols(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, &wmDelete, 1);
			XMapRaised(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win);
			/*
			XWarpPointer(m_pOGLInternaldata->dpy, None, m_pOGLInternaldata->win, 0, 0, 0, 0, 0, 0);
			XGrabKeyboard(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, True, GrabModeAsync,GrabModeAsync, CurrentTime);
			XGrabPointer(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, True, ButtonPressMask,
			GrabModeAsync, GrabModeAsync, m_pOGLInternaldata->win, None, CurrentTime);*/
			XSelectInput(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, StructureNotifyMask);			
			XEvent evtent;
			do { XNextEvent(m_pOGLInternaldata->dpy, &evtent); } while(evtent.type != MapNotify);

		}
		else
		{
			m_pOGLInternaldata->attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
									StructureNotifyMask;
			m_pOGLInternaldata->win = XCreateWindow(m_pOGLInternaldata->dpy, RootWindow(m_pOGLInternaldata->dpy, vi->screen),
									0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
									CWBorderPixel | CWColormap | CWEventMask, &m_pOGLInternaldata->attr);
			wmDelete = XInternAtom(m_pOGLInternaldata->dpy, "WM_DELETE_WINDOW", True);
			XSetWMProtocols(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, &wmDelete, 1);
			XSetStandardProperties(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, title.cstr(),
			title.cstr(), None, NULL, 0, NULL);
			XMapRaised(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win);
			
			XSelectInput(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, StructureNotifyMask);			
			XEvent evtent;
			do 
			{
			  XNextEvent(m_pOGLInternaldata->dpy, &evtent);
			} while(evtent.type != MapNotify);
		}
		
		m_pOGLInternaldata->wmDelete=wmDelete;
	
		glXMakeCurrent(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, m_pOGLInternaldata->ctx);
		XGetGeometry(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, &winDummy, &m_pOGLInternaldata->x, &m_pOGLInternaldata->y,
					&m_pOGLInternaldata->width, &m_pOGLInternaldata->height, &borderDummy, &m_pOGLInternaldata->bpp);
	
		base::log("OGLDevice::initGL()",base::Message) << "Direct Rendering: " << 
			(glXIsDirect(m_pOGLInternaldata->dpy, m_pOGLInternaldata->ctx) ? "enabled" : "disabled") << "\n";

		base::log("OGLDevice::initGL()",base::Message) << "Fullscreen: " <<
			(m_pOGLInternaldata->fs ? "yes" : "no") << "\n";
	}
	
	bool OGLDevice::handleWindow()
	{
		XEvent ev;
		while (XPending(m_pOGLInternaldata->dpy)>0) {
			XNextEvent(m_pOGLInternaldata->dpy,&ev);
		
			switch(ev.type) {
				case ClientMessage:
					if ( (ev.xclient.format == 32) && (ev.xclient.data.l[0] == (long)(m_pOGLInternaldata->wmDelete)) ) {
						m_Quitting=true;
					} break;
				default:
					break;
				case ConfigureNotify:
					if ((ev.xconfigure.width != (int)(m_pOGLInternaldata->width)) ||
						(ev.xconfigure.height != (int)(m_pOGLInternaldata->height)))
					{
					/*	m_pOGLInternaldata->width = event.xconfigure.width;
						m_pOGLInternaldata->height = event.xconfigure.height;
						//printf("Resize Event\n");
						//resizeGLScene();*/
						// TODO: This
					}
					break;
			}
		}
		
		return false;
	}
	
	void OGLDevice::updateGL()
	{
		glXSwapBuffers(m_pOGLInternaldata->dpy,m_pOGLInternaldata->win);
	}
	
	void OGLDevice::shutdownGL()
	{
		if (!m_pOGLInternaldata) return;

		if(m_pOGLInternaldata->ctx)
		{
			if(!glXMakeCurrent(m_pOGLInternaldata->dpy, None, 0))
				base::log("OGLDevice::shutdownGL()",base::Message) << "Could not release GLX drawing context!";

			glXDestroyContext(m_pOGLInternaldata->dpy,m_pOGLInternaldata->ctx);
			m_pOGLInternaldata->ctx=0;
		}
	
		if(m_pOGLInternaldata->fs)
		{
			XF86VidModeSwitchToMode(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, &m_pOGLInternaldata->deskMode);
			XF86VidModeSetViewPort(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, 0, 0);
		}
		XCloseDisplay(m_pOGLInternaldata->dpy);

		delete m_pOGLInternaldata;
		m_pOGLInternaldata=0;
	}

}
}
