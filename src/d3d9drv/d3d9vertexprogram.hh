#ifndef ION_D3D9DRV_D3D9VERTEXPROGRAM_HH_INCLUDED
#define ION_D3D9DRV_D3D9VERTEXPROGRAM_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "../base/dll.hh"
#include "d3d9device.hh"

#ifdef ION_D3D9DRV_D3D9VERTEXPROGRAM_IMPLEMENTATION
#include <map>
#endif

namespace ion {
namespace d3d9drv {

	class ION_API D3D9Vertexprogram:public video::Program
	{
	public:
		D3D9Vertexprogram(D3D9Device& d3d9device,const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);
		~D3D9Vertexprogram();

		LPDIRECT3DVERTEXSHADER9 vertexshader();
		void set();
		bool isValid() const;
		LPD3DXCONSTANTTABLE constantTable();

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

#ifdef ION_D3D9DRV_D3D9VERTEXPROGRAM_IMPLEMENTATION
		std::map <std::string, D3DXHANDLE > *m_pConstants;
#else
		void *m_pDummy;
#endif
		LPDIRECT3DVERTEXSHADER9 m_pVS;
		LPD3DXCONSTANTTABLE m_pConstantTable;
		bool m_IsValid;
	};

}
}

#endif
