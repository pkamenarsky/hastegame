#ifndef ION_SCENE_XMLSHADER_HH_INCLUDED
#define ION_SCENE_XMLSHADER_HH_INCLUDED

#include "../base/dll.hh"
#include "../math/transform.hh"
#include "../video/stateblock.hh"
#include "shader.hh"
#include <string>

class TiXmlDocument;
class TiXmlElement;

namespace ion {

namespace base {

	class Datablock;
	class LocalStreamable;
	class Streamable;

}

namespace video {
	class Videodevice;
}

namespace scene {

	class ION_API XMLShader:public Shader
	{
	public:
		XMLShader(const base::String& shaderidentifier,video::Videodevice &rVideodevice);
		~XMLShader();

		void load(TiXmlDocument &xmldocument);
		void load(base::Datablock& rData,const ::std::string& name);
		void load(base::LocalStreamable& stream);
		void load(base::Streamable& stream);

		void begin();
		void end();

		const math::Transform& worldtransform() const;
		void worldtransform(const math::Transform& transform);

		void passValues(const video::Propertytable& values);

		void beginPass(const ion_uint32 passnum,const Passtype passtype);
		void endPass();
		size_t numPasses(const Passtype passtype) const;

		bool blended() const;

		ion_uint32 maxSimultaneousLights(const video::Lighttype lighttype) const;
		void lightlist(const Lightlist& lightlist,const ion_uint32 offset);

		bool isValid() const;

	protected:
		class Variable;
		class Container;	// Contains all variables, techniques, passes, programlists
		class Technique;
		class Lighttab;
		class Pass;
		class GPUProgram;

		void parseXML(TiXmlDocument &xmldocument);

		bool checkForXMLError(TiXmlDocument &xmldocument);
		void missingattribute(TiXmlDocument &xmldocument,TiXmlElement *element,const char *attribute);

		math::Transform m_Worldtransform;
		video::Videodevice &m_rVideodevice;
		Container *m_pContainer;
		Technique *m_pCurrentTechnique;
		video::Stateblock m_States,m_DefaultStates;
		bool m_IsValid;
	};

}

};

#endif
