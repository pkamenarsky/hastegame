#include "../base/log.hh"
#define ION_D3D9DRV_CGD3D9VERTEXPROGRAM_IMPLEMENTATION
#include "cgd3d9vertexprogram.hh"
#include "d3d9cubemap.hh"
#include "d3d9texture2d.hh"
#include "tod3d9convs.hh"

namespace ion {
namespace d3d9drv {

	CGD3D9Vertexprogram::CGD3D9Vertexprogram(D3D9Device& d3d9device,const base::String& identifier,
		const base::String& entrypoint,const base::String& program):Program(d3d9device,identifier,
		entrypoint,"cg_vprogram"),m_rD3D9Device(d3d9device),m_IsValid(true)
	{
		m_pParameters=new std::map <std::string, CGparameter >;

		m_CgProgram=cgCreateProgram(m_rD3D9Device.cgContext(),CG_SOURCE,program.cstr(),
			m_rD3D9Device.cgVertexProfile(),entrypoint.cstr(),m_rD3D9Device.cgOptimalVertexProfileOptions());

		CGerror cgerr=cgGetError();
		if (cgerr!=0) {
			base::log("CGD3D9Vertexprogram::CGD3D9Vertexprogram()",base::Error) <<
				cgGetErrorString(cgerr) << "\n";
			base::logstream() << cgGetLastListing(m_rD3D9Device.cgContext()) << "\n";

			m_IsValid=false;
		}

		cgD3D9LoadProgram(m_CgProgram,FALSE,0);

		CGparameter param=cgGetFirstParameter(m_CgProgram,CG_GLOBAL);
		while (param!=0) {
			const char *pName=cgGetParameterName(param);
			base::logstream() << pName << "\n";
			(*m_pParameters)[pName]=param;
			param=cgGetNextParameter(param);
		}
	}

	CGD3D9Vertexprogram::~CGD3D9Vertexprogram()
	{
		if (m_CgProgram!=0)
			cgDestroyProgram(m_CgProgram);

		delete m_pParameters;
	}

	void CGD3D9Vertexprogram::set()
	{
		if (!isValid()) return;

		if (m_rD3D9Device.vertexprogram()!=this) {
			m_rD3D9Device.vertexprogram(this);
			cgD3D9BindProgram(m_CgProgram);
		}
	}

	bool CGD3D9Vertexprogram::isValid() const
	{
		return (m_CgProgram!=0);
	}

	void CGD3D9Vertexprogram::setFloat(const base::String& varname,const float f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		cgD3D9SetUniform(param,&f);
	}

	void CGD3D9Vertexprogram::setFloat2(const base::String& varname,const base::Float2& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Vertexprogram::setFloat3(const base::String& varname,const base::Float3& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Vertexprogram::setFloat4(const base::String& varname,const base::Float4& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Vertexprogram::setFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,true);
		cgD3D9SetUniformMatrix(param,&m);
	}

	void CGD3D9Vertexprogram::setFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,true);
		cgD3D9SetUniformMatrix(param,&m);
	}

	void CGD3D9Vertexprogram::setFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,true);
		cgD3D9SetUniformMatrix(param,&m);
	}

	void CGD3D9Vertexprogram::setFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		D3DXMATRIX m; tod3dxmatrix(f,m,true);
		cgD3D9SetUniformMatrix(param,&m);
	}

	void CGD3D9Vertexprogram::bind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		cgD3D9SetTexture(param,((const D3D9Texture2D*)texVar)->d3d9texture());
	}

	void CGD3D9Vertexprogram::bindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		cgD3D9SetTexture(param,((const D3D9Cubemap*)texVar)->d3d9cubetexture());
	}

	ion_uint32 CGD3D9Vertexprogram::constantRegister(const base::String& varname) const
	{
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return 0;
		return cgGetParameterResourceIndex(it->second);
	}

}
}
