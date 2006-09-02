#ifndef ION_OPENGLDRV_CGOGLVERTEXPROGRAM_HH_INCLUDED
#define ION_OPENGLDRV_CGOGLVERTEXPROGRAM_HH_INCLUDED

#include "../base/dll.hh"
#include "ogldevice.hh"

#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace ion {
namespace opengldrv {

	class ION_API CGOGLVertexprogram:public video::Program
	{
	public:
		CGOGLVertexprogram(OGLDevice& ogldevice,const base::String& identifier,
			const base::String& entrypoint,const base::String& program);
		~CGOGLVertexprogram();

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
		OGLDevice& m_rOGLDevice;

		CGprogram m_CgProgram;
		bool m_IsValid;
	};

}
}

#endif
