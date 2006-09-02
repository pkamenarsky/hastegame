#include "../math/matrix.hh"
#include "../math/quaternion.hh"
#include "../math/vectors.hh"
#include "propertytable.hh"

namespace ion {
namespace video {


	struct Propertytable::STLMaps
	{
		std::map<std::string,base::Float4> m_Floats;
		std::map<std::string,base::Float4x4> m_Floats4x4;
		std::map<std::string,ion_int32> m_Ints32;
		std::map<std::string,bool> m_Bools;
		std::map<std::string,Texture2D*> m_2DTextures;
		std::map<std::string,Cubemap*> m_Cubemaps;

		STLMaps() {}
		~STLMaps() {}
	};


	Propertytable::Propertytable():m_RefcountTextures(false),m_pSTLMaps(new STLMaps)
	{
	}

	Propertytable::Propertytable(const Propertytable& src):m_RefcountTextures(false),m_pSTLMaps(new STLMaps)
	{
		clone(src);
	}

	Propertytable::~Propertytable()
	{
		if (m_RefcountTextures) {
			std::map<std::string,Texture2D*>::const_iterator ittex2d=m_pSTLMaps->m_2DTextures.begin();
			for (;ittex2d!=m_pSTLMaps->m_2DTextures.end();++ittex2d) {
				(*ittex2d).second->release();
			}

			std::map<std::string,Cubemap*>::const_iterator itcubemap=m_pSTLMaps->m_Cubemaps.begin();
			for (;itcubemap!=m_pSTLMaps->m_Cubemaps.end();++itcubemap) {
				(*itcubemap).second->release();
			}
		}

		delete m_pSTLMaps;
	}

	void Propertytable::clone(const Propertytable& src)
	{
		if (m_RefcountTextures) {
			std::map<std::string,Texture2D*>::const_iterator ittex2d=m_pSTLMaps->m_2DTextures.begin();
			for (;ittex2d!=m_pSTLMaps->m_2DTextures.end();++ittex2d) {
				(*ittex2d).second->release();
			}

			std::map<std::string,Cubemap*>::const_iterator itcubemap=m_pSTLMaps->m_Cubemaps.begin();
			for (;itcubemap!=m_pSTLMaps->m_Cubemaps.end();++itcubemap) {
				(*itcubemap).second->release();
			}
		}
		m_pSTLMaps->m_Floats=src.mapFloat4();
		m_pSTLMaps->m_Floats4x4=src.mapFloat4x4();
		m_pSTLMaps->m_Ints32=src.mapInt();
		m_pSTLMaps->m_Bools=src.mapBool();
		m_pSTLMaps->m_2DTextures=src.mapTex2D();
		m_pSTLMaps->m_Cubemaps=src.mapCubemap();

		m_RefcountTextures=src.texturesRefcounted();
	}


	void Propertytable::inject(const Propertytable& src)
	{
		std::map<std::string,base::Float4>::const_iterator itfloat4=src.mapFloat4().begin();
		for (;itfloat4!=src.mapFloat4().end();++itfloat4) {
			addFloat4((*itfloat4).first,(*itfloat4).second);
		}

		std::map<std::string,base::Float4x4>::const_iterator itfloat4x4=src.mapFloat4x4().begin();
		for (;itfloat4x4!=src.mapFloat4x4().end();++itfloat4x4) {
			addFloat4x4((*itfloat4x4).first,(*itfloat4x4).second);
		}

		std::map<std::string,ion_int32>::const_iterator itint=src.mapInt().begin();
		for (;itint!=src.mapInt().end();++itint) {
			addInt((*itint).first,(*itint).second);
		}

		std::map<std::string,bool>::const_iterator itbool=src.mapBool().begin();
		for (;itbool!=src.mapBool().end();++itbool) {
			addBool((*itbool).first,(*itbool).second);
		}

		std::map<std::string,Texture2D*>::const_iterator ittex2d=src.mapTex2D().begin();
		for (;ittex2d!=src.mapTex2D().end();++ittex2d) {
			add2DTexture((*ittex2d).first,(*ittex2d).second);
		}

		std::map<std::string,Cubemap*>::const_iterator itcubemap=src.mapCubemap().begin();
		for (;itcubemap!=src.mapCubemap().end();++itcubemap) {
			addCubemap((*itcubemap).first,(*itcubemap).second);
		}
	}

	void Propertytable::clear()
	{
		m_pSTLMaps->m_Floats.clear();
		m_pSTLMaps->m_Floats4x4.clear();
		m_pSTLMaps->m_Ints32.clear();
		m_pSTLMaps->m_Bools.clear();
		m_pSTLMaps->m_2DTextures.clear();
		m_pSTLMaps->m_Cubemaps.clear();
	}

	void Propertytable::addFloat(const base::String& name,const float f)
	{
		m_pSTLMaps->m_Floats[name]=base::Float4(f);
	}

	void Propertytable::addFloat2(const base::String& name,const base::Float2& float2)
	{
		m_pSTLMaps->m_Floats[name]=base::Float4(float2);
	}

	void Propertytable::addFloat3(const base::String& name,const base::Float3& float3)
	{
		m_pSTLMaps->m_Floats[name]=base::Float4(float3);
	}

	void Propertytable::addFloat4(const base::String& name,const base::Float4& float4)
	{
		m_pSTLMaps->m_Floats[name]=float4;
	}

	void Propertytable::addFloat3x3(const base::String& name,const base::Float3x3& float3x3)
	{
		m_pSTLMaps->m_Floats4x4[name]=base::Float4x4(float3x3);
	}

	void Propertytable::addFloat4x3(const base::String& name,const base::Float4x3& float4x3)
	{
		m_pSTLMaps->m_Floats4x4[name]=base::Float4x4(float4x3);
	}

	void Propertytable::addFloat3x4(const base::String& name,const base::Float3x4& float3x4)
	{
		m_pSTLMaps->m_Floats4x4[name]=base::Float4x4(float3x4);
	}

	void Propertytable::addFloat4x4(const base::String& name,const base::Float4x4& float4x4)
	{
		m_pSTLMaps->m_Floats4x4[name]=float4x4;
	}

	void Propertytable::addInt(const base::String& name,const ion_int32 i)
	{
		m_pSTLMaps->m_Ints32[name]=i;
	}

	void Propertytable::addBool(const base::String& name,const bool b)
	{
		m_pSTLMaps->m_Bools[name]=b;
	}

	void Propertytable::add2DTexture(const base::String& name,Texture2D* tex)
	{
		if (m_RefcountTextures) {
			std::map<std::string,Texture2D*>::iterator it=m_pSTLMaps->m_2DTextures.find(name);
			if (it!=m_pSTLMaps->m_2DTextures.end()) (*it).second->release();
			tex->addRef();
		}
		m_pSTLMaps->m_2DTextures[name]=tex;
		//m_pSTLMaps->m_2DTextures.insert(std::map<std::string,Texture2D*>::value_type(name,tex));
	}

	void Propertytable::addCubemap(const base::String& name,Cubemap* cubemap)
	{
		if (m_RefcountTextures) {
			std::map<std::string,Cubemap*>::iterator it=m_pSTLMaps->m_Cubemaps.find(name);
			if (it!=m_pSTLMaps->m_Cubemaps.end()) (*it).second->release();
			cubemap->addRef();
		}
		m_pSTLMaps->m_Cubemaps[name]=cubemap;
	}


	// UTILITY FUNCTIONS

	void Propertytable::addColorRGB(const base::String& name,const float r,const float g,const float b)
	{
		addFloat4(name,base::Float4(r,g,b,1));
	}

	void Propertytable::addColorRGBA(const base::String& name,const float r,const float g,const float b,const float a)
	{
		addFloat4(name,base::Float4(r,g,b,a));
	}

	void Propertytable::addMatrix(const base::String& name,const math::Matrix4f &m)
	{
		addFloat4x4(name,m);
	}

	void Propertytable::addQuaternion(const base::String& name,const math::Quaternion &q)
	{
		addFloat4(name,q);
	}

	void Propertytable::addVector2f(const base::String& name,const math::Vector2f &v)
	{
		addFloat2(name,v);
	}

	void Propertytable::addVector3f(const base::String& name,const math::Vector3f &v)
	{
		addFloat3(name,v);
	}

	void Propertytable::addVector4f(const base::String& name,const math::Vector4f &v)
	{
		addFloat4(name,v);
	}



	bool Propertytable::fetchColorRGB(const base::String& name,float &r,float &g,float &b) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) {
			r=((*it).second)[0];
			g=((*it).second)[1];
			b=((*it).second)[2];
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchColorRGBA(const base::String& name,float &r,float &g,float &b,float &a) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) {
			r=((*it).second)[0];
			g=((*it).second)[1];
			b=((*it).second)[2];
			a=((*it).second)[3];
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat(const base::String& name,float& f) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) { f=(*it).second[0]; return true; }
		else return false;
	}

	bool Propertytable::fetchFloat2(const base::String& name,base::Float2& float2) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) {
			float2=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat3(const base::String& name,base::Float3& float3) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) {
			float3=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat4(const base::String& name,base::Float4& float4) const
	{
		std::map<std::string,base::Float4>::iterator it=m_pSTLMaps->m_Floats.find(name);
		if (it!=m_pSTLMaps->m_Floats.end()) {
			float4=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat3x3(const base::String& name,base::Float3x3& float3x3) const
	{
		std::map<std::string,base::Float4x4>::iterator it=m_pSTLMaps->m_Floats4x4.find(name);
		if (it!=m_pSTLMaps->m_Floats4x4.end()) {
			float3x3=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat4x3(const base::String& name,base::Float4x3& float4x3) const
	{
		std::map<std::string,base::Float4x4>::iterator it=m_pSTLMaps->m_Floats4x4.find(name);
		if (it!=m_pSTLMaps->m_Floats4x4.end()) {
			float4x3=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat3x4(const base::String& name,base::Float3x4& float3x4) const
	{
		std::map<std::string,base::Float4x4>::iterator it=m_pSTLMaps->m_Floats4x4.find(name);
		if (it!=m_pSTLMaps->m_Floats4x4.end()) {
			float3x4=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchFloat4x4(const base::String& name,base::Float4x4& float4x4) const
	{
		std::map<std::string,base::Float4x4>::iterator it=m_pSTLMaps->m_Floats4x4.find(name);
		if (it!=m_pSTLMaps->m_Floats4x4.end()) {
			float4x4=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchInt(const base::String& name,ion_int32& i) const
	{
		std::map<std::string,ion_int32>::iterator it=m_pSTLMaps->m_Ints32.find(name);
		if (it!=m_pSTLMaps->m_Ints32.end()) {
			i=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetchBool(const base::String& name,bool& b) const
	{
		std::map<std::string,bool>::iterator it=m_pSTLMaps->m_Bools.find(name);
		if (it!=m_pSTLMaps->m_Bools.end()) {
			b=(*it).second;
			return true;
		}
		else return false;
	}

	bool Propertytable::fetch2DTexture(const base::String& name,Texture2D*& tex) const
	{
		std::map<std::string,Texture2D*>::iterator it=m_pSTLMaps->m_2DTextures.find(name);
		if (it!=m_pSTLMaps->m_2DTextures.end()) {
			tex=(*it).second;
			return true;
		}
		else { return false; }
	}

	bool Propertytable::fetchCubemap(const base::String& name,Cubemap*& cubemap) const
	{
		std::map<std::string,Cubemap*>::iterator it=m_pSTLMaps->m_Cubemaps.find(name);
		if (it!=m_pSTLMaps->m_Cubemaps.end()) {
			cubemap=(*it).second;
			return true;
		}
		else { return false; }
	}


	const std::map<std::string,base::Float4>& Propertytable::mapFloat4() const { return m_pSTLMaps->m_Floats; }
	const std::map<std::string,base::Float4x4>& Propertytable::mapFloat4x4() const { return m_pSTLMaps->m_Floats4x4; }
	const std::map<std::string,ion_int32>& Propertytable::mapInt() const { return m_pSTLMaps->m_Ints32; }
	const std::map<std::string,bool>& Propertytable::mapBool() const { return m_pSTLMaps->m_Bools; }
	const std::map<std::string,Texture2D*>& Propertytable::mapTex2D() const { return m_pSTLMaps->m_2DTextures; }
	const std::map<std::string,Cubemap*>& Propertytable::mapCubemap() const { return m_pSTLMaps->m_Cubemaps; }

	bool Propertytable::isValid() const
	{
		return (m_pSTLMaps!=0);
	}


}
}
