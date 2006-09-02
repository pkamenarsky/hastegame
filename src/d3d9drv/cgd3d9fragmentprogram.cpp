#define ION_D3D9DRV_CGD3D9FRAGMENTPROGRAM_IMPLEMENTATION
#include "../base/log.hh"
#include "cgd3d9fragmentprogram.hh"
#include "d3d9cubemap.hh"
#include "d3d9texture2d.hh"

namespace ion {
namespace d3d9drv {

	CGD3D9Fragmentprogram::CGD3D9Fragmentprogram(D3D9Device& d3d9device,const base::String& identifier,
		const base::String& entrypoint,const base::String& program):Program(d3d9device,identifier,
		entrypoint,"cg_fprogram"),m_rD3D9Device(d3d9device),m_IsValid(true)
	{
		m_pTexvars=new std::map <std::string, const video::Texture2D* >;
		m_pParameters=new std::map <std::string, CGparameter >;

		m_CgProgram=cgCreateProgram(m_rD3D9Device.cgContext(),CG_SOURCE,program.cstr(),
			m_rD3D9Device.cgFragmentProfile(),entrypoint.cstr(),m_rD3D9Device.cgOptimalFragmentProfileOptions());

		CGerror cgerr=cgGetError();
		if (cgerr!=0) {
			base::log("CGD3D9Fragmentprogram::CGD3D9Fragmentprogram()",base::Error) <<
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

	CGD3D9Fragmentprogram::~CGD3D9Fragmentprogram()
	{
		if (m_CgProgram!=0)
			cgDestroyProgram(m_CgProgram);

		delete m_pTexvars;
		delete m_pParameters;
	}

	void CGD3D9Fragmentprogram::set()
	{
		if (!isValid()) return;

		if (m_rD3D9Device.fragmentprogram()!=this) {
			m_rD3D9Device.fragmentprogram(this);
			cgD3D9BindProgram(m_CgProgram);
		}
	}

	bool CGD3D9Fragmentprogram::isValid() const
	{
		return (m_CgProgram!=0);
	}

	void CGD3D9Fragmentprogram::setFloat(const base::String& varname,const float f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second;
		cgD3D9SetUniform(param,&f);
	}

	void CGD3D9Fragmentprogram::setFloat2(const base::String& varname,const base::Float2& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat3(const base::String& varname,const base::Float3& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat4(const base::String& varname,const base::Float4& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::setFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
//		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		CGparameter param=it->second; const float *pF=f;
		cgD3D9SetUniform(param,pF);
	}

	void CGD3D9Fragmentprogram::bind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
		/*std::map <std::string, const video::Texture2D* >::const_iterator it=m_pTexvars->find(varname);
		if (it!=m_pTexvars->end()) {
			if (it->second==texVar) return;
		}
		(*m_pTexvars)[varname]=texVar;

		//CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		std::map<std::string, CGparameter >::iterator itt=m_pParameters->find(varname);
		if (itt==m_pParameters->end()) return;
		CGparameter param=itt->second;
		cgD3D9SetTexture(param,(texVar==0) ? 0 : ((const D3D9Texture2D*)texVar)->d3d9texture());*/

		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		else {
			ion_uint32 stagenr=cgGetParameterResourceIndex(it->second);
			m_rD3D9Device.bindTexture(stagenr,texVar);
		}

	}

	void CGD3D9Fragmentprogram::bindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
		/*CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		cgD3D9SetTexture(param,(texVar==0) ? 0 : ((const D3D9Cubemap*)texVar)->d3d9cubetexture());*/
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return;
		else {
			ion_uint32 stagenr=cgGetParameterResourceIndex(it->second);
			m_rD3D9Device.bindTexture(stagenr,texVar);
		}
	}

	ion_uint32 CGD3D9Fragmentprogram::constantRegister(const base::String& varname) const
	{
		std::map<std::string, CGparameter >::iterator it=m_pParameters->find(varname);
		if (it==m_pParameters->end()) return 0;
		return cgGetParameterResourceIndex(it->second);
	}

}
}
