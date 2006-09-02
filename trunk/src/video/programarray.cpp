#include <assert.h>
#include <tinyxml.h>
#include <algorithm>
#include <map>
#include <string>
#include <tinyxml.h>
#include "../base/log.hh"
#include "program.hh"
#include "programarray.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	struct Programarray::Internaldata
	{
		std::vector < video::Program* > m_Programs;

		Internaldata() {}

		~Internaldata()
		{
			std::vector < video::Program* >::iterator itp=m_Programs.begin();
			for (;itp!=m_Programs.end();++itp) {
				video::Program* pProgram=(*itp);
				pProgram->release();
			}
		}

		void clone(const Internaldata& src)
		{
			const std::vector < video::Program* >& srcprograms=src.m_Programs;

			std::vector < video::Program* >::const_iterator itp=srcprograms.begin();
			for (;itp!=srcprograms.end();++itp) {
				video::Program* pProgram=(*itp);
				pProgram->addRef();
			}

			for (itp=m_Programs.begin();itp!=m_Programs.end();++itp) {
				video::Program* pProgram=(*itp);
				pProgram->release();
			}

			m_Programs=srcprograms;
		}
	};

	const Programarray::Internaldata& Programarray::internaldata() const
	{
		return *m_pInternaldata;
	}

	Programarray::Programarray(Videodevice &rVideodevice,const Type programstype):m_rVideodevice(rVideodevice),
		m_pInternaldata(new Internaldata),m_Programstype(programstype)
	{
	}

	Programarray::Programarray(const Programarray& src):m_rVideodevice(src.videodevice()),
		m_pInternaldata(new Internaldata),m_Programstype(src.programstype())
	{
		clone(src);
	}

	Programarray::~Programarray()
	{
		delete m_pInternaldata;
	}

	Programarray::Type Programarray::programstype() const
	{
		return m_Programstype;
	}

	Videodevice& Programarray::videodevice() const
	{
		return m_rVideodevice;
	}

	void Programarray::clone(const Programarray& src)
	{
		m_Programstype=src.programstype();
	}

	Programarray& Programarray::operator =(const Programarray& src)
	{
		clone(src);
		m_pInternaldata->clone(src.internaldata());
		return *this;
	}

	ion_uint32 Programarray::numPrograms() const
	{
		return (ion_uint32)(m_pInternaldata->m_Programs.size());
	}

	Program* Programarray::program(const ion_uint32 index) const
	{
		return (index<numPrograms()) ? m_pInternaldata->m_Programs[index] : 0;
	}

	bool Programarray::addProgram(const base::String& identifier,const base::String& entrypoint,const base::String& program,
		const base::String& format)
	{
		Program *pProgram=0;
		
		switch (m_Programstype) {
			case Vertexprograms:pProgram=m_rVideodevice.createVertexProgram(identifier,entrypoint,program,format); break;
			case Fragmentprograms:pProgram=m_rVideodevice.createFragmentProgram(identifier,entrypoint,program,format); break;
			default:break;
		}

		bool isValid=(pProgram!=0);
		if (isValid) isValid=pProgram->isValid();

		if (isValid) {
			m_pInternaldata->m_Programs.push_back(pProgram);
			return true;
		} else {
			delete pProgram;
			return false;
		}
	}

	video::Program* Programarray::getFirstValidProgram()
	{
		std::vector < video::Program* >::const_iterator itp=m_pInternaldata->m_Programs.begin();
		for (;itp!=m_pInternaldata->m_Programs.end();++itp) {
			video::Program* pProgram=(*itp);
			if (pProgram->isValid()) {
				return pProgram;
			}
		}

		return 0;
	}

	bool Programarray::isValid() const
	{
		return m_rVideodevice.isValid();
	}


	bool checkForXMLError(TiXmlDocument &xmldocument)
	{
		if (xmldocument.Error()) {
			base::log("XMLShader::checkForXMLError()",base::Error) << "shader [" << xmldocument.Value() << "] line " << xmldocument.ErrorRow()
			<< " col " << xmldocument.ErrorCol() << " : " << xmldocument.ErrorDesc() << "\n";
			xmldocument.ClearError();

			return true;
		} else return false;
	}

	void missingattribute(TiXmlDocument &xmldocument,TiXmlElement *element,const char *attribute)
	{
		base::log("XMLShader::missingattribute()",base::Error) << "Error in XML [" << xmldocument.Value() << "] line " << element->Row()
		 << " col " << element->Column() << " : element attribute \"" << attribute << "\" missing\n";
	}



	Programarray* Programarray::loadFromXML(base::Streamable& streamable,Videodevice &rVideodevice)
	{
		base::Datablock dblock;
		dblock.transferFrom(streamable);
		dblock.expand(1);
		dblock[dblock.size()-1]=0;

		return loadFromXML(dblock,rVideodevice,false);
	}

	Programarray* Programarray::loadFromXML(base::LocalStreamable& localstreamable,Videodevice &rVideodevice)
	{
		base::Datablock dblock;
		dblock.transferFrom(localstreamable);
		dblock.expand(1);
		dblock[dblock.size()-1]=0;

		return loadFromXML(dblock,rVideodevice,false);
	}

	Programarray* Programarray::loadFromXML(base::Datablock& datablock,Videodevice &rVideodevice,const bool addNullbyte)
	{
		if (addNullbyte) {
			datablock.expand(1);
			datablock[datablock.size()-1]=0;
		}

		if (datablock.data()[datablock.size()-1]!=0) {
			base::log("XMLShader::load()",base::Error) << "XML parsing error: XML input does not have a nullbyte at the end of the datablock\n";
		}

		TiXmlDocument doc;
		doc.Parse((const char*)(datablock.data()));

		bool noError=!checkForXMLError(doc);
		if (!noError) return 0;

		// Now for the actual parsing

		TiXmlElement *root=doc.RootElement();
		if (std::string(root->Value())!="array") return 0;

		Programarray::Type type;
		std::string typestr=root->Attribute("type");
		if (typestr=="vertexprogram") type=Programarray::Vertexprograms;
		else if (typestr=="fragmentprogram") type=Programarray::Fragmentprograms;

		Programarray *pArray=new Programarray(rVideodevice,type);

		for (TiXmlElement *entry=root->FirstChildElement();entry!=0;entry=entry->NextSiblingElement()) {
			std::string namestr=entry->Attribute("name");
			std::string formatstr=entry->Attribute("format"),entrypointstr=entry->Attribute("entrypoint");
			
			TiXmlText *pCode=entry->FirstChild()->ToText();
			std::string code=pCode->Value();

			pArray->addProgram(namestr,entrypointstr,code,formatstr);
		}

		return pArray;
	}

}
}
