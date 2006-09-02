#ifndef ION_D3D9DRV_CGD3D9FRAGMENTPROGRAM_HH_INCLUDED
#define ION_D3D9DRV_CGD3D9FRAGMENTPROGRAM_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <Cg/cg.h>
#include <Cg/cgD3D9.h>

#include "../base/dll.hh"
#include "d3d9device.hh"

#ifdef ION_D3D9DRV_CGD3D9FRAGMENTPROGRAM_IMPLEMENTATION
#include <map>
#endif

namespace ion {
namespace d3d9drv {

	class ION_API CGD3D9Fragmentprogram:public video::Program
	{
	public:
		CGD3D9Fragmentprogram(D3D9Device& d3d9device,const base::String& identifier,
			const base::String& entrypoint,const base::String& program);
		~CGD3D9Fragmentprogram();

		void set();
		bool isValid() const;

		void setFloat(const base::String& varname,const float);
		void setFloat2(const base::String& varname,const base::Float2&);
		void setFloat3(const base::String& varname,const base::Float3&);
		void setFloat4(const base::String& varname,const base::Float4&);
		void setFloat3x3(const base::String& varname,const base::Float3x3&);
		void setFloat4x3(const base::String& varname,const base::Float4x3&);
		void setFloat3x4(const base::String& varname,const base::Float3x4&);
		void setFloat4x4(const base::String& varname,const base::Float4x4&);
		void bind2DTexture(const base::String& varname,const video::Texture2D* texVar);
		void bindCubemap(const base::String& varname,const video::Cubemap* texVar);

		ion_uint32 constantRegister(const base::String& varname) const;

	protected:
		D3D9Device& m_rD3D9Device;

#ifdef ION_D3D9DRV_CGD3D9FRAGMENTPROGRAM_IMPLEMENTATION
		std::map <std::string, const video::Texture2D* > *m_pTexvars;
		std::map <std::string, CGparameter > *m_pParameters;
#else
		void *m_pDummy[2];
#endif
		CGprogram m_CgProgram;
		bool m_IsValid;
	};

}
}

#endif
