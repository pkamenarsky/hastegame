#include <map>
#include "q3shaders.hh"
#include "../base/physfsfile.hh"

namespace ion {
namespace scene {

	struct Q3Shaderlist::Shadermap
	{
		std::map <std::string, Q3Shader* > m_Shaders;
	};

	Q3Shaderlist::Q3Shaderlist():m_pShadermap(0)
	{
		m_pShadermap=new Shadermap;
	}

	Q3Shaderlist::~Q3Shaderlist()
	{
		if (m_pShadermap) {
			std::map <std::string, Q3Shader* >::iterator it=m_pShadermap->m_Shaders.begin();
			for (;it!=m_pShadermap->m_Shaders.end();++it) {
				it->second->release();
			}
			delete m_pShadermap;
		}
	}

	void Q3Shaderlist::scanForShaders()
	{
		base::Stringlist result;
		findPhysfsFiles(result,"scripts","*.shader",false);
	}

	Q3Shader* Q3Shaderlist::shader(const base::String& name)
	{
		std::map <std::string, Q3Shader* >::iterator it=m_pShadermap->m_Shaders.find(name.STLstring());
		return (it!=m_pShadermap->m_Shaders.end()) ? it->second : 0;
	}



}
}
