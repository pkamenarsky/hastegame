#ifndef ION_SCENE_Q3SHADERS_HH_INCLUDED
#define ION_SCENE_Q3SHADERS_HH_INCLUDED

#include "../base/string.hh"
#include "shader.hh"

namespace ion {
namespace scene {

	class ION_API Q3Shader:public Shader
	{
	};

	class ION_API Q3Shaderlist
	{
	public:
		Q3Shaderlist();
		~Q3Shaderlist();

		void scanForShaders();

		Q3Shader *shader(const base::String& name);

	protected:
		struct Shadermap;
		Shadermap *m_pShadermap;
	};

}
}

#endif
