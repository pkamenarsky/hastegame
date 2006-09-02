#include "../base/log.hh"
#include "cgoglfragmentprogram.hh"
#include "oglcubemap.hh"
#include "ogl2dtexture.hh"

namespace ion {
namespace opengldrv {

	CGOGLFragmentprogram::CGOGLFragmentprogram(OGLDevice& ogldevice,const base::String& identifier,
		const base::String& entrypoint,const base::String& program):Program(ogldevice,identifier,
		entrypoint,"cg_fprogram"),m_rOGLDevice(ogldevice),m_IsValid(true)
	{
		m_CgProgram=cgCreateProgram(m_rOGLDevice.cgContext(),CG_SOURCE,program.cstr(),
			m_rOGLDevice.cgFragmentProfile(),entrypoint.cstr(),0);

		CGerror cgerr=cgGetError();
		if (cgerr!=0) {
			base::log("CGOGLFragmentprogram::CGOGLFragmentprogram()",base::Error) << cgGetErrorString(cgerr) << "\n";
			if (cgerr==CG_COMPILER_ERROR) base::logstream() << cgGetLastListing(m_rOGLDevice.cgContext()) << "\n";
			m_IsValid=false;
		}

		cgGLLoadProgram(m_CgProgram);
	}

	CGOGLFragmentprogram::~CGOGLFragmentprogram()
	{
		if (m_CgProgram!=0)
			cgDestroyProgram(m_CgProgram);
	}

	void CGOGLFragmentprogram::set()
	{
		if (!isValid()) return;

		if (m_rOGLDevice.fragmentprogram()!=this) {
			m_rOGLDevice.fragmentprogram(this);
			cgGLBindProgram(m_CgProgram);
		}
	}

	bool CGOGLFragmentprogram::isValid() const
	{
		return (m_CgProgram!=0);
	}

	void CGOGLFragmentprogram::setFloat(const base::String& varname,const float f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		cgGLSetParameter1f(param,f);
	}

	void CGOGLFragmentprogram::setFloat2(const base::String& varname,const base::Float2& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetParameter2fv(param,pF);
	}

	void CGOGLFragmentprogram::setFloat3(const base::String& varname,const base::Float3& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetParameter3fv(param,pF);
	}

	void CGOGLFragmentprogram::setFloat4(const base::String& varname,const base::Float4& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetParameter4fv(param,pF);
	}

	void CGOGLFragmentprogram::setFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetMatrixParameterfr(param,pF);
	}

	void CGOGLFragmentprogram::setFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetMatrixParameterfr(param,pF);
	}

	void CGOGLFragmentprogram::setFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetMatrixParameterfr(param,pF);
	}

	void CGOGLFragmentprogram::setFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr()); const float *pF=f;
		cgGLSetMatrixParameterfr(param,pF);
	}

	// TODO: cgGLDisableTextureParameter insertion

	void CGOGLFragmentprogram::bind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
		/*CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		if (texVar==0)
			cgGLDisableTextureParameter(param);
		else {
			cgGLSetTextureParameter(param,((const OGL2DTexture*)texVar)->handle());
			cgGLEnableTextureParameter(param);
		}*/
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		ion_uint32 stage=cgGetParameterResourceIndex(param);
		m_rOGLDevice.bindTexture(stage,texVar);
	}

	void CGOGLFragmentprogram::bindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
		/*CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		if (texVar==0)
			cgGLDisableTextureParameter(param);
		else {
			cgGLSetTextureParameter(param,((const OGLCubemap*)texVar)->handle());
			cgGLEnableTextureParameter(param);
		}*/
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		ion_uint32 stage=cgGetParameterResourceIndex(param);
		m_rOGLDevice.bindTexture(stage,texVar);
	}

	ion_uint32 CGOGLFragmentprogram::constantRegister(const base::String& varname) const
	{
		CGparameter param=cgGetNamedParameter(m_CgProgram,varname.cstr());
		return cgGetParameterResourceIndex(param);
	}

}
}
