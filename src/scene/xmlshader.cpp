#include <assert.h>
#include <tinyxml.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "../base/log.hh"
#include "../base/string.hh"
#include "../video/program.hh"
#include "../video/propertytable.hh"
#include "../video/videodevice.hh"
#include "light.hh"
#include "lightlist.hh"
#include "xmlshader.hh"

namespace ion {
namespace scene {



	class XMLShader::Container
	{
	public:
		Container(video::Videodevice &rVideodevice):m_NumTechniques(0),m_rVideodevice(rVideodevice) {}
		~Container();

		void addTechnique(XMLShader::Technique *pTechnique);
		XMLShader::Technique* technique(const size_t index);
		size_t numTechniques() const { return m_NumTechniques; }

		inline video::Propertytable& variablevalues() { return m_Variablevalues; }
		inline const video::Propertytable& variablevalues() const { return m_Variablevalues; }

		GPUProgram* addVertexprogram(const std::string& name,video::Program *pProgram);
		GPUProgram* addFragmentprogram(const std::string& name,video::Program *pProgram);
		GPUProgram* vertexprogram(const std::string& name);
		GPUProgram* fragmentprogram(const std::string& name);

		inline std::map < std::string, GPUProgram* >& vertexprograms() { return m_Vertexprograms; }
		inline std::map < std::string, GPUProgram* >& fragmentprograms() { return m_Fragmentprograms; }

		inline std::vector < XMLShader::Variable >& variables() { return m_Variables; }
		inline std::vector < XMLShader::Variable >& varsWithSemantics() { return m_VarsWithSemantics; }

		inline const std::vector < XMLShader::Variable >& variables() const { return m_Variables; }
		inline const std::vector < XMLShader::Variable >& varsWithSemantics() const { return m_VarsWithSemantics; }

		const XMLShader::Variable* findVariable(const std::string& name);

		inline video::Videodevice& videodevice() { return m_rVideodevice; }
	protected:
		std::map < std::string, GPUProgram* > m_Vertexprograms,m_Fragmentprograms;
		std::vector < XMLShader::Technique * > m_Techniques;
		std::vector < XMLShader::Variable > m_Variables,m_VarsWithSemantics;
		size_t m_NumTechniques;
		video::Videodevice &m_rVideodevice;
		video::Propertytable m_Variablevalues;
	};



	class XMLShader::Variable
	{
	public:
		enum Type
		{
			VarFloat1,
			VarFloat2,
			VarFloat3,
			VarFloat4,
			VarFloat3x3,
			VarFloat4x3,
			VarFloat3x4,
			VarFloat4x4,

			VarTexture2D,
			VarCubemap
		};

		enum Semantic
		{
			SemanticWorld,
			SemanticView,
			SemanticProjection,
			SemanticWorldView,
			SemanticViewProjection,
			SemanticWorldViewProjection,
			SemanticEye,
			SemanticNone
		};

		Variable(const base::String& type,const base::String& name,const base::String& semantic);
		Variable(const Type type,const base::String& name,const Semantic semantic);
		Variable(const Variable& src) { clone(src); }

		Variable& operator =(const Variable& src) { clone(src); return *this; }

		void clone(const Variable& src) { m_Type=src.type(); m_Name=src.m_Name; m_Semantic=src.semantic(); validate(); }

		Type type() const { return m_Type; }
		const base::String& name() const { return m_Name; }
		Semantic semantic() const { return m_Semantic; }
		bool isValid() { return m_IsValid; }

		~Variable() {}
	protected:
		void validate();

		Type m_Type;
		base::String m_Name;
		Semantic m_Semantic;
		bool m_IsValid;
	};

	XMLShader::Variable::Variable(const base::String& type,const base::String& name,const base::String& semantic):
	m_Name(name),m_IsValid(true)
	{
		if ((type=="float") || (type=="float1")) m_Type=VarFloat1;
		else if (type=="float2") m_Type=VarFloat2;
		else if (type=="float3") m_Type=VarFloat3;
		else if (type=="float4") m_Type=VarFloat4;
		else if (type=="float3x3") m_Type=VarFloat3x3;
		else if (type=="float4x3") m_Type=VarFloat4x3;
		else if (type=="float3x4") m_Type=VarFloat3x4;
		else if (type=="float4x4") m_Type=VarFloat4x4;
		else if (type=="texture2d") m_Type=VarTexture2D;
		else if (type=="cubemap") m_Type=VarCubemap;
		else m_IsValid=false;

		if (semantic=="world") m_Semantic=SemanticWorld;
		else if (semantic=="view") m_Semantic=SemanticView;
		else if (semantic=="projection") m_Semantic=SemanticProjection;
		else if (semantic=="worldview") m_Semantic=SemanticWorldView;
		else if (semantic=="viewprojection") m_Semantic=SemanticViewProjection;
		else if (semantic=="worldviewprojection") m_Semantic=SemanticWorldViewProjection;
		else if (semantic=="eye") m_Semantic=SemanticEye;
		else m_Semantic=SemanticNone;

		if (m_IsValid) validate(); // further checks for ensuring validity; not needed if already invalid
	}

	XMLShader::Variable::Variable(const Type type,const base::String& name,const Semantic semantic):
	m_Type(type),m_Name(name),m_Semantic(semantic)
	{
		validate();
	}

	void XMLShader::Variable::validate()
	{
		switch (m_Semantic) {
			case SemanticWorld:
			case SemanticView:
			case SemanticProjection:
			case SemanticWorldView:
			case SemanticViewProjection:
			case SemanticWorldViewProjection:
				switch (m_Type) {
					case VarFloat3x3:
					case VarFloat4x3:
					case VarFloat3x4:
					case VarFloat4x4:
						m_IsValid=true; break;
					default:
						m_IsValid=false;
				} break;
			case SemanticEye:
				switch (m_Type) {
					case VarFloat1:
					case VarFloat2:
					case VarFloat3:
					case VarFloat4:
						m_IsValid=true; break;
					default:
						m_IsValid=false;
				} break;
			case SemanticNone:m_IsValid=true; break;
		}
	}




	class XMLShader::GPUProgram
	{
	public:

		// Constantname = name of the constant in the gpu program; Variablename = name of the corresponding variable in the container
		struct Constant
		{
			base::String m_Variablename,m_Constantname;
			Variable::Type m_Type;
		};


		GPUProgram(Container &rContainer):m_pProgram(0),m_rContainer(rContainer) {}
		~GPUProgram() { if (m_pProgram) delete m_pProgram; }

		const std::vector < Constant >& constants() const { return m_Constants; }
		std::vector < Constant >& constants() { return m_Constants; }

		void program(video::Program* pProgram) { m_pProgram=pProgram; }
		video::Program* program() { return m_pProgram; }
		const video::Program* program() const { return m_pProgram; }

		void finishConstants();
		void transferConstantValues();

	protected:
		video::Program *m_pProgram;
		std::vector <Constant> m_Constants;
		Container &m_rContainer;
	};

	void XMLShader::GPUProgram::transferConstantValues()
	{
		std::vector <Constant>::iterator it=m_Constants.begin();
		for (;it!=m_Constants.end();++it) {
			const Constant &rConstant=(*it);

			switch (rConstant.m_Type) {
				case Variable::VarFloat1: {
					float f;
					m_rContainer.variablevalues().fetchFloat(rConstant.m_Variablename,f);
					m_pProgram->setFloat(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat2: {
					base::Float2 f;
					m_rContainer.variablevalues().fetchFloat2(rConstant.m_Variablename,f);
					m_pProgram->setFloat2(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat3: {
					base::Float3 f;
					m_rContainer.variablevalues().fetchFloat3(rConstant.m_Variablename,f);
					m_pProgram->setFloat3(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat4: {
					base::Float4 f;
					m_rContainer.variablevalues().fetchFloat4(rConstant.m_Variablename,f);
					m_pProgram->setFloat4(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat3x3: {
					base::Float3x3 f;
					m_rContainer.variablevalues().fetchFloat3x3(rConstant.m_Variablename,f);
					m_pProgram->setFloat3x3(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat4x3: {
					base::Float4x3 f;
					m_rContainer.variablevalues().fetchFloat4x3(rConstant.m_Variablename,f);
					m_pProgram->setFloat4x3(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat3x4: {
					base::Float3x4 f;
					m_rContainer.variablevalues().fetchFloat3x4(rConstant.m_Variablename,f);
					m_pProgram->setFloat3x4(rConstant.m_Constantname,f);
				} break;
				case Variable::VarFloat4x4: {
					base::Float4x4 f;
					m_rContainer.variablevalues().fetchFloat4x4(rConstant.m_Variablename,f);
					m_pProgram->setFloat4x4(rConstant.m_Constantname,f);
				} break;
				case Variable::VarTexture2D: {
					video::Texture2D* tex2d;
					m_rContainer.variablevalues().fetch2DTexture(rConstant.m_Variablename,tex2d);
					m_pProgram->bind2DTexture(rConstant.m_Constantname,tex2d);
				} break;
				case Variable::VarCubemap: {
					video::Cubemap* cubemap;
					m_rContainer.variablevalues().fetchCubemap(rConstant.m_Variablename,cubemap);
					m_pProgram->bindCubemap(rConstant.m_Constantname,cubemap);
				} break;
				default:break;
			}
		}
	}




	class XMLShader::Pass
	{
	public:
		enum Renderstates {
			Renderstate_Blending,
			Renderstate_Depthbuffer,
			Renderstate_Depthwrite,
			Renderstate_Stencilbuffer,
			Renderstate_Stencilwritemask,
			Renderstate_Culling,
			Renderstate_Cullingmode,
			Renderstate_Blendfunc,
			Renderstate_Depthfunc,
			Renderstate_Stencilfunc,
			Renderstate_Stencilop,
			Renderstate_Drawingmode,
			Renderstate_Solidshading,
			Renderstate_Depthbias,
			Renderstate_Fixedlighting
		};



		Pass(XMLShader &rShader,Container &rContainer,Technique &rTechnique,TiXmlElement *xmlelement):m_rShader(rShader),
		m_rContainer(rContainer),m_rTechnique(rTechnique),m_rVideodevice(rContainer.videodevice()),
		m_pVertexprogram(0),m_pGeometryprogram(0),m_pFragmentprogram(0),
		m_IsValid(false)
		{
			parseXML(xmlelement);
		}
		~Pass()
		{
			/*if (m_pVertexprogram) delete m_pVertexprogram;
			if (m_pGeometryprogram) delete m_pGeometryprogram;
			if (m_pFragmentprogram) delete m_pFragmentprogram;*/
		}

		void fetchShaders();

		void invoke();

		inline bool isValid() const { return m_IsValid; }

		inline std::string name() const { return m_Name; }
		inline Passtype type() const { return m_Type; }
	protected:
		struct State
		{
			ion_uint32 m_State;
			float m_Float[3];
			unsigned long m_ULong[3];
		};

		void parseXML(TiXmlElement *xmlelement);

		std::string m_Name,m_VProgramName,m_FProgramName;
		Passtype m_Type;
		XMLShader &m_rShader;
		Container &m_rContainer;
		Technique &m_rTechnique;
		video::Videodevice &m_rVideodevice;
		std::vector <State> m_States;
		video::Program *m_pVertexprogram,*m_pGeometryprogram,*m_pFragmentprogram;
		bool m_IsValid;
	};

	void XMLShader::Pass::parseXML(TiXmlElement *xmlelement)
	{
		const char *c;
		std::string str;

		m_IsValid=true;

		assert(xmlelement!=0);

		m_Name="";
		c=xmlelement->Attribute("name");
		if (c!=0) m_Name=c;

		m_Type=UnlitPass;
		c=xmlelement->Attribute("type");
		if (c!=0) {
			/*AmbientPass,
			UnlitPass,
			PointlightPass,
			SpotlightPass,
			DirectionalLightPass*/
			str=c;
			if (str=="ambient") m_Type=AmbientPass;
			else if (str=="unlit") m_Type=UnlitPass;
			else if (str=="pointlight") m_Type=PointlightPass;
			else if (str=="spotlight") m_Type=SpotlightPass;
			else if (str=="directionallight") m_Type=DirectionalLightPass;
		}

		for (TiXmlElement* element=xmlelement->FirstChildElement();element!=0;element=element->NextSiblingElement()) {
			std::string elementname=element->Value();

			if (elementname=="blending") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Blending;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			} else if (elementname=="depthbuffer") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Depthbuffer;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			} else if (elementname=="depthwrite") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Depthwrite;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			} else if (elementname=="stencilbuffer") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Stencilbuffer;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			// TODO: stencilbuffer
			} else if (elementname=="culling") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Culling;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			} else if (elementname=="cullingmode") {
				c=element->Attribute("type");
				if (c!=0) str=c;

				State state;
				state.m_State=Renderstate_Cullingmode;
				state.m_ULong[0]=(c!=0) ? ((str=="counterclockwise") ? video::Cullingmode_Counterclockwise : video::Cullingmode_Clockwise) : video::Cullingmode_Counterclockwise;

				m_States.push_back(state);
			} else if (elementname=="blendfunc") {
				const char *srcblendc=element->Attribute("srcblend");
				const char *destblendc=element->Attribute("destblend");

				if ((srcblendc!=0) && (destblendc!=0)) {
					State state;
					state.m_State=Renderstate_Blendfunc;

					bool unknownfactor=false;

					for (int i=0;i<2;++i) {
						str=(i==0) ? srcblendc : destblendc;

						if (str=="zero") state.m_ULong[i]=video::BlendFunc_Zero;
						else if (str=="one") state.m_ULong[i]=video::BlendFunc_One;
						else if (str=="srccolor") state.m_ULong[i]=video::BlendFunc_Src_Color;
						else if (str=="oneminussrccolor") state.m_ULong[i]=video::BlendFunc_One_Minus_Src_Color;
						else if (str=="destcolor") state.m_ULong[i]=video::BlendFunc_Dest_Color;
						else if (str=="oneminusdestcolor") state.m_ULong[i]=video::BlendFunc_One_Minus_Dest_Color;
						else if (str=="srcalpha") state.m_ULong[i]=video::BlendFunc_Src_Alpha;
						else if (str=="oneminussrcalpha") state.m_ULong[i]=video::BlendFunc_One_Minus_Src_Alpha;
						else if (str=="destalpha") state.m_ULong[i]=video::BlendFunc_Dest_Alpha;
						else if (str=="oneminusdestalpha") state.m_ULong[i]=video::BlendFunc_One_Minus_Dest_Alpha;
						else unknownfactor=true;
					}

					if (!unknownfactor) m_States.push_back(state);
				}
			} else if (elementname=="depthfunc") {
				c=element->Attribute("type");
				if (c!=0) {
					str=c;
					State state;
					state.m_State=Renderstate_Depthfunc;
					state.m_ULong[0]=video::CompareFunc_Lessequal;

					if (str=="always") state.m_ULong[0]=video::CompareFunc_Always;
					else if (str=="notequal") state.m_ULong[0]=video::CompareFunc_Notequal;
					else if (str=="greater") state.m_ULong[0]=video::CompareFunc_Greater;
					else if (str=="greaterequal") state.m_ULong[0]=video::CompareFunc_Greaterequal;
					else if (str=="equal") state.m_ULong[0]=video::CompareFunc_Equal;
					else if (str=="lessequal") state.m_ULong[0]=video::CompareFunc_Lessequal;
					else if (str=="less") state.m_ULong[0]=video::CompareFunc_Less;
					else if (str=="never") state.m_ULong[0]=video::CompareFunc_Never;

					m_States.push_back(state);
				}
			// TODO: stencilfunc, stencilop
			} else if (elementname=="drawingmode") {
				c=element->Attribute("type");
				if (c!=0) {
					str=c;
					State state;
					state.m_State=Renderstate_Drawingmode;
					state.m_ULong[0]=video::Drawingmode_Faces;

					if (str=="points") state.m_ULong[0]=video::Drawingmode_Points;
					else if (str=="lines") state.m_ULong[0]=video::Drawingmode_Lines;
					else if (str=="faces") state.m_ULong[0]=video::Drawingmode_Faces;

					m_States.push_back(state);
				}

			} else if (elementname=="solidshading") {
				c=element->Attribute("type");
				if (c!=0) {
					str=c;
					State state;
					state.m_State=Renderstate_Solidshading;
					state.m_ULong[0]=video::Solidshading_Smooth;

					if (str=="flat") state.m_ULong[0]=video::Solidshading_Flat;
					else if (str=="smooth") state.m_ULong[0]=video::Solidshading_Smooth;

					m_States.push_back(state);
				}

			} else if (elementname=="depthbias") {
				double scaledbias=0,uniformbias=0;
				
				bool b=(element->QueryDoubleAttribute("scaledbias",&scaledbias)==TIXML_SUCCESS);
				b=b&&(element->QueryDoubleAttribute("uniformbias",&uniformbias)==TIXML_SUCCESS);

				if (b) {
					State state;
					state.m_State=Renderstate_Depthbias;
					state.m_Float[0]=(float)scaledbias;
					state.m_Float[1]=(float)uniformbias;
					m_States.push_back(state);
				}
			} else if (elementname=="fixedlighting") {
				c=element->Attribute("enabled");

				State state;
				state.m_State=Renderstate_Fixedlighting;
				state.m_ULong[0]=(c!=0) ? ((c[0]=='1') ? 1 : 0) : 1; // If no "enabled" attribute is given, set to true

				m_States.push_back(state);
			} else if (elementname=="vertexprogram") {
				c=element->Attribute("name");
				if (c!=0) m_VProgramName=c;
			} else if (elementname=="fragmentprogram") {
				c=element->Attribute("name");
				if (c!=0) m_FProgramName=c;
			}
		}

	}




	class XMLShader::Lighttab
	{
	public:
		enum Entrytype {
			EntryPosition,
			EntryDirection,
			EntryRange,
			EntryColor,
			EntryConstantAttenuation,
			EntryLinearAttenuation,
			EntryQuadraticAttenuation
		};

		struct Entry {
			std::string m_Variablename;
			Variable::Type m_Variabletype;
			Entrytype m_Entrytype;
		};

		std::vector <Entry>& entries() { return m_Entries; }
		const std::vector <Entry>& entries() const { return m_Entries; }

		void finishLighttabInitializations();

		Lighttab(Container &rContainer):m_rContainer(rContainer) {}
	protected:
		std::vector <Entry> m_Entries;
		Container& m_rContainer;
	};

	void XMLShader::Lighttab::finishLighttabInitializations()
	{
		std::vector <Entry>::iterator ite=m_Entries.begin();
		for (;ite!=m_Entries.end();++ite) {
			Entry &rEntry=(*ite);
			const XMLShader::Variable* pVar=m_rContainer.findVariable(rEntry.m_Variablename);
			if (pVar!=0) { // TODO: Error checking (pVar == 0 should not happen!)
				rEntry.m_Variabletype=pVar->type();
			}

		}
	}





	class XMLShader::Technique
	{
	public:
		Technique(XMLShader &rShader,Container &rContainer,TiXmlElement *xmlelement):
		m_rShader(rShader),m_rContainer(rContainer),m_pLightlist(0),m_Lightlistoffset(0),m_NumPasses(0),m_pCurrentPass(0),m_IsValid(false),
		m_Lighttab(rContainer)
		{
			parseXML(xmlelement);
		}

		~Technique() { for (std::vector< XMLShader::Pass * >::iterator it=m_Passes.begin();it!=m_Passes.end();++it) delete (*it); }

		void beginPass(const size_t passnum);
		void beginPass(const size_t passnum,const Passtype passtype);
		void endPass();
		XMLShader::Pass *pass(const size_t passnum);
		XMLShader::Pass *pass(const size_t passnum,const Passtype passtype);
		size_t numPasses() const;
		size_t numPasses(const Passtype passtype) const;

		ion_uint32 maxSimultaneousLights() const;
		void lightlist(const Lightlist& lightlist,const ion_uint32 offset);

		void finishPassInitializations();

		inline bool isValid() const { return m_IsValid; }

		const std::string& name() const { return m_Name; }

	protected:
		void parseXML(TiXmlElement *xmlelement);

		XMLShader &m_rShader;
		Container &m_rContainer;

		const Lightlist* m_pLightlist;
		ion_uint32 m_Lightlistoffset;

		std::vector < XMLShader::Pass * > m_Passes,m_AmbientPasses,m_UnlitPasses,
			m_PointlightPasses,m_SpotlightPasses,m_DirectionalLightPasses;
		size_t m_NumPasses;
		XMLShader::Pass *m_pCurrentPass;
		std::string m_Name;
		bool m_IsValid;

		Lighttab m_Lighttab;
	};

	void XMLShader::Technique::beginPass(const size_t passnum)
	{
		if ((m_pCurrentPass!=0) || (passnum>=m_NumPasses)) return;
		m_pCurrentPass=m_Passes[passnum];
		m_pCurrentPass->invoke(); // TODO: invoke() should be passed a journal for storing alterations to the states
	}


	void XMLShader::Technique::beginPass(const size_t passnum,const Passtype passtype)
	{
		if (m_pCurrentPass!=0) return;

		XMLShader::Pass *pPass=0;

		switch (passtype) {
			case AmbientPass:			pPass=m_AmbientPasses[passnum]; break;
			case UnlitPass:				pPass=m_UnlitPasses[passnum]; break;
			case PointlightPass:		pPass=m_PointlightPasses[passnum]; break;
			case SpotlightPass:			pPass=m_SpotlightPasses[passnum]; break;
			case DirectionalLightPass:	pPass=m_DirectionalLightPasses[passnum]; break;
			default:break;
		}

		assert(pPass!=0);
		m_pCurrentPass=pPass;

		pPass->invoke();
	}

	void XMLShader::Technique::endPass()
	{
		if (m_pCurrentPass==0) return;
		// TODO: Call a journal to rollback alterations
		m_pCurrentPass=0;
	}

	XMLShader::Pass *XMLShader::Technique::pass(const size_t passnum)
	{
		return (passnum<numPasses()) ? m_Passes[passnum] : 0;
	}

	XMLShader::Pass *XMLShader::Technique::pass(const size_t passnum,const Passtype passtype)
	{
		switch (passtype) {
			case AmbientPass:return m_AmbientPasses[passnum];
			case UnlitPass:return m_UnlitPasses[passnum];
			case PointlightPass:return m_PointlightPasses[passnum];
			case SpotlightPass:return m_DirectionalLightPasses[passnum];
			case DirectionalLightPass:return m_DirectionalLightPasses[passnum];
			default:return 0;
		}
	}

	size_t XMLShader::Technique::numPasses() const
	{
		return m_NumPasses;
	}

	size_t XMLShader::Technique::numPasses(const Passtype passtype) const
	{
		switch (passtype) {
			case AmbientPass:return m_AmbientPasses.size();
			case UnlitPass:return m_UnlitPasses.size();
			case PointlightPass:return m_PointlightPasses.size();
			case SpotlightPass:return m_DirectionalLightPasses.size();
			case DirectionalLightPass:return m_DirectionalLightPasses.size();
			default:return 0;
		}
	}

	ion_uint32 XMLShader::Technique::maxSimultaneousLights() const
	{
		return (ion_uint32)(m_Lighttab.entries().size());
	}

/*
		enum Entrytype {
			EntryPosition,
			EntryDirection,
			EntryRange,
			EntryColor,
			EntryConstantAttenuation,
			EntryLinearAttenuation,
			EntryQuadraticAttenuation
		};

		struct Entry {
			std::string m_Variablename;
			Variable::Type m_Variabletype;
			Entrytype m_Entrytype;
		};
		
			VarFloat1,
			VarFloat2,
			VarFloat3,
			VarFloat4,
			VarFloat3x3,
			VarFloat4x3,
			VarFloat3x4,
			VarFloat4x4,

			VarTexture2D,
			VarCubemap		
		*/
	void XMLShader::Technique::lightlist(const Lightlist& lightlist,const ion_uint32 offset)
	{
	/*	if ((m_pLightlist==&lightlist) && (m_Lightlistoffset==offset)) return;

		m_pLightlist=&lightlist;
		m_Lightlistoffset=offset;

		{
			video::Propertytable& rVars=m_rContainer.variablevalues();

			std::vector <Lighttab::Entry>::const_iterator ite=m_Lighttab.entries().begin();
			std::vector< Light * >::const_iterator itl=lightlist.lights().begin()+offset;

			for (;(ite!=m_Lighttab.entries().end()) && (itl!=lightlist.lights().end());++ite,++itl) {
				const Lighttab::Entry &rLEntry=(*ite);
				const Light* pLight=(*itl);

				switch (rLEntry.m_Entrytype) {
					case Lighttab::EntryPosition: {
						switch (rLEntry.m_Variabletype) {
							case Variable::VarFloat1: rVars.addFloat(rLEntry.m_Variablename,pLight->transform().position().x()); break;
							case Variable::VarFloat2:
								rVars.addFloat2(rLEntry.m_Variablename,base::Float2(pLight->transform().position().x(),pLight->transform().position().y())); break;
							case Variable::VarFloat3: rVars.addFloat3(rLEntry.m_Variablename,pLight->transform().position()); break;
							case Variable::VarFloat4:
								rVars.addFloat4(rLEntry.m_Variablename,base::Float4(pLight->transform().position().x(),pLight->transform().position().y(),
									pLight->transform().position().z(),0)); break;
								
							default:break;
						}
					} break;
					case Lighttab::EntryDirection: {
						switch (rLEntry.m_Variabletype) {
							case Variable::VarFloat1: rVars.addFloat(rLEntry.m_Variablename,pLight->direction().x()); break;
							case Variable::VarFloat2:
								rVars.addFloat2(rLEntry.m_Variablename,base::Float2(pLight->direction().x(),pLight->direction().y())); break;
							case Variable::VarFloat3: rVars.addFloat3(rLEntry.m_Variablename,pLight->direction()); break;
							case Variable::VarFloat4:
								rVars.addFloat4(rLEntry.m_Variablename,base::Float4(pLight->direction().x(),pLight->direction().y(),pLight->direction().z(),0)); break;
								
							default:break;
						}
					} break;
					case Lighttab::EntryRange: {
						switch (rLEntry.m_Variabletype) {
							case Variable::VarFloat1: rVars.addFloat(rLEntry.m_Variablename,pLight->range()); break;
							case Variable::VarFloat2: rVars.addFloat2(rLEntry.m_Variablename,base::Float2(pLight->range(),0)); break;
							case Variable::VarFloat3: rVars.addFloat3(rLEntry.m_Variablename,base::Float3(pLight->range(),0,0)); break;
							case Variable::VarFloat4: rVars.addFloat4(rLEntry.m_Variablename,base::Float4(pLight->range(),0,0,0)); break;
								
							default:break;
						}
					} break;
					case Lighttab::EntryColor: {
						switch (rLEntry.m_Variabletype) {
							case Variable::VarFloat1: rVars.addFloat(rLEntry.m_Variablename,pLight->color()[0]); break;
							case Variable::VarFloat2: rVars.addFloat2(rLEntry.m_Variablename,pLight->color()); break;
							case Variable::VarFloat3: rVars.addFloat3(rLEntry.m_Variablename,pLight->color()); break;
							case Variable::VarFloat4: rVars.addFloat4(rLEntry.m_Variablename,pLight->color()); break;
								
							default:break;
						}
					} break;
					default:break;
				}

			}
		}*/
	}

	void XMLShader::Technique::parseXML(TiXmlElement *xmlelement)
	{
		assert(xmlelement!=0);

		// Fetch the technique name
		{
			const char *name=xmlelement->Attribute("name");
			m_Name=name;
		}

		for (TiXmlElement* element=xmlelement->FirstChildElement();element!=0;element=element->NextSiblingElement()) {
			std::string elementname=element->Value();

			// Parse a lighttab
			if (elementname=="lighttab") {
				Lighttab::Entry entry;

				for (TiXmlElement* ltabelem=element->FirstChildElement();ltabelem!=0;ltabelem=ltabelem->NextSiblingElement()) {
					std::string ltabelemname=ltabelem->Value();

					// Right now only lights are allowed in a light tab
					if (ltabelemname!="light") continue;

					// parse the lights contents
					for (TiXmlElement* lightelem=ltabelem->FirstChildElement();lightelem!=0;lightelem=lightelem->NextSiblingElement()) {
						std::string lightelemname=lightelem->Value();
						const char *varname=lightelem->Attribute("variable");
						if (varname) entry.m_Variablename=varname;

						if (lightelemname=="position") entry.m_Entrytype=Lighttab::EntryPosition;
						else if (lightelemname=="direction") entry.m_Entrytype=Lighttab::EntryDirection;
						else if (lightelemname=="direction") entry.m_Entrytype=Lighttab::EntryDirection;
						else if (lightelemname=="range") entry.m_Entrytype=Lighttab::EntryRange;
						else if (lightelemname=="color") entry.m_Entrytype=Lighttab::EntryColor;
						else if (lightelemname=="constantattenuation") entry.m_Entrytype=Lighttab::EntryConstantAttenuation;
						else if (lightelemname=="linearattenuation") entry.m_Entrytype=Lighttab::EntryLinearAttenuation;
						else if (lightelemname=="quadraticattenuation") entry.m_Entrytype=Lighttab::EntryQuadraticAttenuation;
					}
				}

				m_Lighttab.entries().push_back(entry);

			// Parse a pass
			} else if (elementname=="pass") {
				XMLShader::Pass *pPass=new XMLShader::Pass(m_rShader,m_rContainer,*this,element);
				m_Passes.push_back(pPass);

				switch (pPass->type()) {
					case AmbientPass:m_AmbientPasses.push_back(pPass); break;
					case UnlitPass:m_UnlitPasses.push_back(pPass); break;
					case PointlightPass:m_PointlightPasses.push_back(pPass); break;
					case SpotlightPass:m_DirectionalLightPasses.push_back(pPass); break;
					case DirectionalLightPass:m_DirectionalLightPasses.push_back(pPass); break;
					default:break;
				}

			}

		}
	}

	void XMLShader::Technique::finishPassInitializations()
	{
		m_IsValid=true;

		std::vector < XMLShader::Pass * >::iterator it=m_Passes.begin();
		for (;it!=m_Passes.end();++it) {
			XMLShader::Pass *pPass=(*it);
			pPass->fetchShaders();
			if (!(pPass->isValid())) { m_IsValid=false; return; }
		}

		// If all passes are valid, finish lighttab initialization
		if (m_IsValid)
			m_Lighttab.finishLighttabInitializations();
	}




	XMLShader::Container::~Container()
	{
		for (std::vector< XMLShader::Technique * >::iterator itt=m_Techniques.begin();itt!=m_Techniques.end();++itt)
			delete (*itt);

		std::map < std::string, GPUProgram* >::iterator itm;
		for (itm=m_Vertexprograms.begin();itm!=m_Vertexprograms.end();++itm) delete (*itm).second;
		for (itm=m_Fragmentprograms.begin();itm!=m_Fragmentprograms.end();++itm) delete (*itm).second;
	}

	void XMLShader::Container::addTechnique(XMLShader::Technique *pTechnique)
	{
		m_Techniques.push_back(pTechnique);
		++m_NumTechniques;
	}

	XMLShader::Technique* XMLShader::Container::technique(const size_t index)
	{
		return (index<m_NumTechniques) ? m_Techniques[index] : 0;
	}

	XMLShader::GPUProgram* XMLShader::Container::addVertexprogram(const std::string& name,video::Program *pProgram)
	{
		base::logstream() << "XMLShader::Container::addVertexprogram( " << name << " )\n";

		GPUProgram *pGPUProgram=new GPUProgram(*this);
		pGPUProgram->program(pProgram);
		m_Vertexprograms[name]=pGPUProgram;

		return pGPUProgram;
	}

	XMLShader::GPUProgram* XMLShader::Container::addFragmentprogram(const std::string& name,video::Program *pProgram)
	{
		base::logstream() << "XMLShader::Container::addFragmentprogram( " << name << " )\n";

		GPUProgram *pGPUProgram=new GPUProgram(*this);
		pGPUProgram->program(pProgram);
		m_Fragmentprograms[name]=pGPUProgram;

		return pGPUProgram;
	}

	XMLShader::GPUProgram* XMLShader::Container::vertexprogram(const std::string& name)
	{
		base::logstream() << "XMLShader::Container::vertexprogram( " << name << " )\n";

		std::map < std::string, GPUProgram* >::iterator it=m_Vertexprograms.find(name);
		return (it!=m_Vertexprograms.end()) ? ((*it).second) : 0;
	}

	XMLShader::GPUProgram* XMLShader::Container::fragmentprogram(const std::string& name)
	{
		base::logstream() << "XMLShader::Container::fragmentprogram( " << name << " )\n";

		std::map < std::string, GPUProgram* >::iterator it=m_Fragmentprograms.find(name);
		return (it!=m_Fragmentprograms.end()) ? ((*it).second) : 0;
	}

	const XMLShader::Variable* XMLShader::Container::findVariable(const std::string& name)
	{
		// NOTE: The pointers returned by this method may change after an operation is performed upon the m_Variables vector!
		// Consider the pointers for local use only!
		std::vector < XMLShader::Variable >::const_iterator itv=m_Variables.begin();
		for (;itv!=m_Variables.end();++itv) {
			const Variable &rVar=(*itv);
			if (rVar.name()==name) return &rVar;
		}

		return 0;
	}



	void XMLShader::Pass::fetchShaders()
	{
		if (!(m_VProgramName.empty())) {
			GPUProgram* pProgram=m_rContainer.vertexprogram(m_VProgramName);
			if (pProgram==0) m_IsValid=false;
			else m_pVertexprogram=pProgram->program();
		}
		if (!(m_FProgramName.empty())) {
			GPUProgram* pProgram=m_rContainer.fragmentprogram(m_FProgramName);
			if (pProgram==0) m_IsValid=false;
			else m_pFragmentprogram=pProgram->program();
		}
	}

	void XMLShader::Pass::invoke()
	{
		// Set programs (if available)
		if ((m_pVertexprogram!=0) && (m_pFragmentprogram!=0)) {
			m_pVertexprogram->set();
			m_pFragmentprogram->set();
		} else {// fixed function
			m_rVideodevice.worldmatrix(m_rShader.worldtransform().matrix());
		}

/*
		struct State
		{
			ion_uint32 m_State;
			float m_Float[3];
			unsigned long m_ULong[3];
		};


	Renderstate_Blending,
			Renderstate_Depthbuffer,
			Renderstate_Depthwrite,
			Renderstate_Stencilbuffer,
			Renderstate_Stencilwritemask,
			Renderstate_Culling,
			Renderstate_Cullingmode,
			Renderstate_Blendfunc,
			Renderstate_Depthfunc,
			Renderstate_Stencilfunc,
			Renderstate_Stencilop,
			Renderstate_Drawingmode,
			Renderstate_Solidshading,
			Renderstate_Depthbias,
			Renderstate_Fixedlighting*/

		// Set states
		std::vector <State>::const_iterator itstate=m_States.begin();
		for (;itstate!=m_States.end();++itstate) {
			const State& state=(*itstate);
			switch (state.m_State) {
				case Renderstate_Blending:m_rVideodevice.blending(state.m_ULong[0]!=0); break;
				case Renderstate_Depthbuffer:m_rVideodevice.zbuffer(state.m_ULong[0]!=0); break;
				case Renderstate_Depthwrite:m_rVideodevice.zwrite(state.m_ULong[0]!=0); break;
				case Renderstate_Stencilbuffer:m_rVideodevice.stencilbuffer(state.m_ULong[0]!=0); break;
				case Renderstate_Culling:m_rVideodevice.culling(state.m_ULong[0]!=0); break;
				case Renderstate_Cullingmode:m_rVideodevice.cullmode((video::Cullingmode)(state.m_ULong[0])); break;
				case Renderstate_Blendfunc:m_rVideodevice.blendfunc(
					(video::BlendFunc)(state.m_ULong[0]),(video::BlendFunc)(state.m_ULong[1])); break;
				case Renderstate_Depthfunc:m_rVideodevice.zfunc((video::CompareFunc)(state.m_ULong[0])); break;
				case Renderstate_Drawingmode:m_rVideodevice.drawingmode((video::Drawingmode)(state.m_ULong[0])); break;
				case Renderstate_Solidshading:m_rVideodevice.solidshading((video::Solidshading)(state.m_ULong[0])); break;
				case Renderstate_Depthbias:m_rVideodevice.depthbias(state.m_Float[0],state.m_Float[1]); break;
				case Renderstate_Fixedlighting:m_rVideodevice.fixedLighting(state.m_ULong[0]!=0); break;
				default:break;
			};
		}
	}




	void XMLShader::GPUProgram::finishConstants()
	{
		std::vector <Constant>::iterator itc=m_Constants.begin();
		for (;itc!=m_Constants.end();++itc) {
			Constant &rConstant=(*itc);
			const XMLShader::Variable* pVar=m_rContainer.findVariable(rConstant.m_Variablename);
			if (pVar!=0) { // TODO: Error checking (pVar == 0 should not happen!)
				rConstant.m_Type=pVar->type();
			}

		}
	}







	// XMLSHADER

	XMLShader::XMLShader(const base::String& shaderidentifier,video::Videodevice &rVideodevice):Shader(shaderidentifier),
		m_rVideodevice(rVideodevice),m_pContainer(new Container(rVideodevice)),m_pCurrentTechnique(0),m_IsValid(false)
	{
	}

	XMLShader::~XMLShader()
	{
		delete m_pContainer;
	}

	void XMLShader::load(TiXmlDocument &xmldocument)
	{
		parseXML(xmldocument);

		//After parsing, certain elements aren't fully initialized; they are finished now.

		std::map < std::string, GPUProgram* >::iterator itp;

		// Finish contants initialization
		for (itp=m_pContainer->vertexprograms().begin();itp!=m_pContainer->vertexprograms().end();++itp)
			(*itp).second->finishConstants();

		for (itp=m_pContainer->fragmentprograms().begin();itp!=m_pContainer->fragmentprograms().end();++itp)
			(*itp).second->finishConstants();

		// Retrieve valid pointers to GPU programs etc.
		for (size_t t=0;t<m_pContainer->numTechniques();++t) {
			Technique *pTechnique=m_pContainer->technique(t);
			pTechnique->finishPassInitializations();

			if ((m_pCurrentTechnique==0) && pTechnique->isValid()) {
				m_pCurrentTechnique=pTechnique;
				base::logstream() << "Current technique: " << pTechnique->name() << "\n";
			}
		}
	}

	void XMLShader::load(base::Datablock& rData,const std::string& name)
	{
		rData.expand(1);
		rData[rData.size()-1]=0;
		TiXmlDocument doc(name);
		doc.Parse((const char*)(rData.data()));

		bool noError=checkForXMLError(doc);
		if (!noError) {
			if (rData.data()[rData.size()-1]!=0) {
				base::log("XMLShader::load()",base::Error) << "XML parsing error: XML input does not have a nullbyte at the end of the datablock\n";
			}/* else {
				ion::primaryLogger() << "XML dump (" << rData.size() <<  " byte): -------\n";
				ion::primaryLogger() << (const char*)(rData.data());
				ion::primaryLogger() << "\n------- XML dump end\n";
			}*/
		}

		load(doc);
	}

	void XMLShader::load(base::LocalStreamable& stream)
	{
		base::Datablock dblock;
		dblock.transferFrom(stream);

		load(dblock,stream.name());
	}

	void XMLShader::load(base::Streamable& stream)
	{
		base::Datablock dblock;
		dblock.transferFrom(stream);

		load(dblock,stream.name());
	}

	void XMLShader::begin()
	{
		//m_States.setDefaultValues();
		//TODO: Set the values of vars with semantics here

/*inline std::vector < XMLShader::Variable >& varsWithSemantics() { return m_VarsWithSemantics; }
		enum Type
		{
			VarFloat1,
			VarFloat2,
			VarFloat3,
			VarFloat4,
			VarFloat3x3,
			VarFloat4x3,
			VarFloat3x4,
			VarFloat4x4,

			VarTexture2D,
			VarCubemap
		};

		enum Semantic
		{
			SemanticWorld,
			SemanticView,
			SemanticProjection,
			SemanticWorldView,
			SemanticViewProjection,
			SemanticWorldViewProjection,
			SemanticEye,
			SemanticNone
		};*/
		
		m_rVideodevice.worldmatrix(m_Worldtransform.matrix());

		std::vector < XMLShader::Variable >::const_iterator itv=m_pContainer->varsWithSemantics().begin();
		for (;itv!=m_pContainer->varsWithSemantics().end();++itv) {
			const XMLShader::Variable& var=(*itv);

			switch (var.semantic()) {
				case Variable::SemanticWorld:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.worldmatrix()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.worldmatrix()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.worldmatrix()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.worldmatrix()); break;
						default:break;
					} break;
				case Variable::SemanticView:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.viewmatrix()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.viewmatrix()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.viewmatrix()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.viewmatrix()); break;
						default:break;
					} break;
				case Variable::SemanticProjection:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.projmatrix()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.projmatrix()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.projmatrix()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.projmatrix()); break;
						default:break;
					} break;
				case Variable::SemanticWorldView:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.worldView()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.worldView()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.worldView()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.worldView()); break;
						default:break;
					} break;
				case Variable::SemanticViewProjection:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.viewProj()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.viewProj()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.viewProj()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.viewProj()); break;
						default:break;
					} break;
				case Variable::SemanticWorldViewProjection:
					switch (var.type()) {
						case Variable::VarFloat3x3:m_pContainer->variablevalues().addFloat3x3(var.name(),m_rVideodevice.worldViewProj()); break;
						case Variable::VarFloat4x3:m_pContainer->variablevalues().addFloat4x3(var.name(),m_rVideodevice.worldViewProj()); break;
						case Variable::VarFloat3x4:m_pContainer->variablevalues().addFloat3x4(var.name(),m_rVideodevice.worldViewProj()); break;
						case Variable::VarFloat4x4:m_pContainer->variablevalues().addFloat4x4(var.name(),m_rVideodevice.worldViewProj()); break;
						default:break;
					} break;
				case Variable::SemanticEye:
					switch (var.type()) {
						case Variable::VarFloat1:m_pContainer->variablevalues().addFloat(var.name(),m_rVideodevice.eyePosition()[0]); break;
						case Variable::VarFloat2:m_pContainer->variablevalues().addFloat2(var.name(),base::Float2(m_rVideodevice.eyePosition()[0],m_rVideodevice.eyePosition()[1])); break;
						case Variable::VarFloat3:m_pContainer->variablevalues().addFloat3(var.name(),m_rVideodevice.eyePosition()); break;
						case Variable::VarFloat4:m_pContainer->variablevalues().addFloat4(var.name(),
							base::Float4(	m_rVideodevice.eyePosition()[0],m_rVideodevice.eyePosition()[1],
											m_rVideodevice.eyePosition()[2],0)); break;
						default:break;
					} break;
				default:break;
			}
		}
		
		std::map < std::string, GPUProgram* >::iterator itp;

		for (itp=m_pContainer->vertexprograms().begin();itp!=m_pContainer->vertexprograms().end();++itp)
			(*itp).second->transferConstantValues();

		for (itp=m_pContainer->fragmentprograms().begin();itp!=m_pContainer->fragmentprograms().end();++itp)
			(*itp).second->transferConstantValues();
	}

	void XMLShader::end()
	{
		//m_DefaultStates.invoke(0,&m_States);
		m_rVideodevice.activateFFPipeline(video::FixedfunctionVertex);
		m_rVideodevice.activateFFPipeline(video::FixedfunctionFragment);
	}

	const math::Transform& XMLShader::worldtransform() const
	{
		return m_Worldtransform;
	}

	void XMLShader::worldtransform(const math::Transform& transform)
	{
		m_Worldtransform=transform;
	}

	void XMLShader::passValues(const video::Propertytable& values)
	{
		m_pContainer->variablevalues().inject(values);
	}

	void XMLShader::beginPass(const ion_uint32 passnum,const Passtype passtype)
	{
		if (m_pCurrentTechnique)
			m_pCurrentTechnique->beginPass(passnum,passtype);
	}

	void XMLShader::endPass()
	{
		if (m_pCurrentTechnique)
			m_pCurrentTechnique->endPass();
	}

	size_t XMLShader::numPasses(const Passtype passtype) const
	{
		return (m_pCurrentTechnique!=0) ? m_pCurrentTechnique->numPasses(passtype) : 0;
	}

	bool XMLShader::blended() const
	{
		return false;
	}

	ion_uint32 XMLShader::maxSimultaneousLights(const video::Lighttype lighttype) const
	{
		return (m_pCurrentTechnique!=0) ? m_pCurrentTechnique->maxSimultaneousLights() : 0;
	}

	void XMLShader::lightlist(const Lightlist& lightlist,const ion_uint32 offset)
	{
		if (m_pCurrentTechnique)
			m_pCurrentTechnique->lightlist(lightlist,offset);
	}

	bool XMLShader::isValid() const
	{
		return true;
	}

	void XMLShader::parseXML(TiXmlDocument &xmldocument)
	{
		m_IsValid=true;

		TiXmlElement* rootElement=xmldocument.RootElement();
		checkForXMLError(xmldocument);

		if (strcmp(rootElement->Value(),"ionshader")!=0) {
			base::log("XMLShader::parseXML()",base::Error) << "Error in XML [" << xmldocument.Value() << "] : "
				<< "Root element should be named \"ionshader\", not \"" << rootElement->Value() << "\"\n";

			return;
		}

		for (TiXmlElement* element=rootElement->FirstChildElement();element!=0;element=element->NextSiblingElement()) {
			std::string elementname=element->Value();
			if (elementname=="technique") {
				Technique *pNewTechnique=new Technique(*this,*m_pContainer,element);
				m_pContainer->addTechnique(pNewTechnique);
			// TODO: Support geometry programs when available
			} else if ((elementname=="vertexprogram") || (elementname=="fragmentprogram")) {
				int gpuptype=0;
				if (elementname=="vertexprogram") gpuptype=0;
				else if (elementname=="fragmentprogram") gpuptype=1;

				const char *name=element->Attribute("name");
				const char *type=element->Attribute("type");
				const char *entrypoint=element->Attribute("entrypoint");
				TiXmlText *pCode=0;

				std::vector < std::pair< std::string, std::string > > constants;

				TiXmlNode* node=element->FirstChild();
				while (node) {
					switch (node->Type()) {
						case TiXmlNode::TEXT: pCode=node->ToText(); break;
						case TiXmlNode::ELEMENT: {
							TiXmlElement *pElement=node->ToElement();
							assert(pElement!=0);
							std::string elementname=pElement->Value();
							if (elementname=="constant") {
								std::string varname=pElement->Attribute("variablename");
								std::string constname=pElement->Attribute("constantname");

								// TODO: Error checking

								constants.push_back(std::pair <std::string , std::string >(varname,constname));
							}
						}
						default:break;
					}

					node=node->NextSibling();
				}

				if (name && type && entrypoint && pCode) {
					base::String code(pCode->Value());

					video::Program *pProgram=0;
					
					switch (gpuptype) {
						case 0:pProgram=m_rVideodevice.createVertexProgram(name,entrypoint,code,type); break;
						case 1:pProgram=m_rVideodevice.createFragmentProgram(name,entrypoint,code,type); break;
						default:break;
					}

					GPUProgram *pGPUProgram=0;

					if (pProgram) {
						if (pProgram->isValid()) {
							switch (gpuptype) {
								case 0:pGPUProgram=m_pContainer->addVertexprogram(name,pProgram); break;
								case 1:pGPUProgram=m_pContainer->addFragmentprogram(name,pProgram); break;
								default:break;
							}
						}

						if (pGPUProgram==0) { m_IsValid=false; delete pProgram; }
					} else m_IsValid=false;

					if (m_IsValid==false) {
						base::log("XMLShader::parseXML()",base::Message) << "GPU program " << name << " could not be initialized\n";
					} else { // Add constants to the gpu program
						GPUProgram::Constant constant;

						std::vector < std::pair< std::string, std::string > >::iterator itconst=constants.begin();
						for (;itconst!=constants.end();++itconst) {
							constant.m_Variablename=(*itconst).first;
							constant.m_Constantname=(*itconst).second;

							pGPUProgram->constants().push_back(constant);
						}
					}
				}
			} else if ((elementname=="float1") ||
			(elementname=="float2") ||
			(elementname=="float3") ||
			(elementname=="float4") ||
			(elementname=="float3x3") ||
			(elementname=="float4x3") ||
			(elementname=="float3x4") ||
			(elementname=="float4x4") ||
			(elementname=="texture2d") ||
			(elementname=="cubemap")) {
				const char *c;
				std::string varname,semantic;
				c=element->Attribute("name"); if (c!=0) varname=c;
				c=element->Attribute("semantic"); if (c!=0) semantic=c;
				Variable var(elementname,varname,semantic); // NOTE: elementname equals to the type here!

				m_pContainer->variables().push_back(var);
				if (var.semantic()!=Variable::SemanticNone)
					m_pContainer->varsWithSemantics().push_back(var);
			}
		}

	}

	bool XMLShader::checkForXMLError(TiXmlDocument &xmldocument)
	{
		if (xmldocument.Error()) {
			base::log("XMLShader::checkForXMLError()",base::Error) << "shader [" << xmldocument.Value() << "] line " << xmldocument.ErrorRow()
			<< " col " << xmldocument.ErrorCol() << " : " << xmldocument.ErrorDesc() << "\n";
			xmldocument.ClearError();

			return true;
		} else return false;
	}

	void XMLShader::missingattribute(TiXmlDocument &xmldocument,TiXmlElement *element,const char *attribute)
	{
		base::log("XMLShader::missingattribute()",base::Error) << "Error in XML [" << xmldocument.Value() << "] line " << element->Row()
		 << " col " << element->Column() << " : element attribute \"" << attribute << "\" missing\n";
	}

}
}
