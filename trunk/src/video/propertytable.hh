#ifndef ION_VIDEO_PROPERTYTABLE_HH_INCLUDED
#define ION_VIDEO_PROPERTYTABLE_HH_INCLUDED

#include "../base/baserefcounted.hh"
#include "../base/iontypes.hh"
#include "../base/numericarrays.hh"
#include "../base/string.hh"
#include "cubemap.hh"
#include "texture2d.hh"

#include <map>
#include <string>

namespace ion {

namespace math {
	class Matrix4f;
	class Quaternion;
	class Vector2f;
	class Vector3f;
	class Vector4f;
}

namespace video {

	class ION_API Propertytable:public base::BaseRefCounted
	{
	public:
		Propertytable();
		Propertytable(const Propertytable& src);
		virtual ~Propertytable();

		inline Propertytable& operator = (const Propertytable& src) { clone(src); return *this; }
		
		virtual void clone(const Propertytable& src);
		void inject(const Propertytable& src);

		void clear();

		void addFloat(const base::String& name,const float f);
		void addFloat2(const base::String& name,const base::Float2& float2);
		void addFloat3(const base::String& name,const base::Float3& float3);
		void addFloat4(const base::String& name,const base::Float4& float4);
		void addFloat3x3(const base::String& name,const base::Float3x3& float3x3);
		void addFloat4x3(const base::String& name,const base::Float4x3& float4x3);
		void addFloat3x4(const base::String& name,const base::Float3x4& float3x4);
		void addFloat4x4(const base::String& name,const base::Float4x4& float4x4);
		void addInt(const base::String& name,const ion_int32 i);
		void addBool(const base::String& name,const bool b);
		void add2DTexture(const base::String& name,Texture2D* tex);
		void addCubemap(const base::String& name,Cubemap* cubemap);

		void addColorRGB(const base::String& name,const float r,const float g,const float b);
		void addColorRGBA(const base::String& name,const float r,const float g,const float b,const float a);
		void addMatrix(const base::String& name,const math::Matrix4f &m);
		void addQuaternion(const base::String& name,const math::Quaternion &q);
		void addVector2f(const base::String& name,const math::Vector2f &v);
		void addVector3f(const base::String& name,const math::Vector3f &v);
		void addVector4f(const base::String& name,const math::Vector4f &v);

		bool fetchColorRGB(const base::String& name,float &r,float &g,float &b) const;
		bool fetchColorRGBA(const base::String& name,float &r,float &g,float &b,float &a) const;
		bool fetchFloat(const base::String& name,float& f) const;
		bool fetchFloat2(const base::String& name,base::Float2& float2) const;
		bool fetchFloat3(const base::String& name,base::Float3& float3) const;
		bool fetchFloat4(const base::String& name,base::Float4& float4) const;
		bool fetchFloat3x3(const base::String& name,base::Float3x3& float3x3) const;
		bool fetchFloat4x3(const base::String& name,base::Float4x3& float4x3) const;
		bool fetchFloat3x4(const base::String& name,base::Float3x4& float3x4) const;
		bool fetchFloat4x4(const base::String& name,base::Float4x4& float4x4) const;
		bool fetchInt(const base::String& name,ion_int32& i) const;
		bool fetchBool(const base::String& name,bool& b) const;
		bool fetch2DTexture(const base::String& name,Texture2D*& tex) const;
		bool fetchCubemap(const base::String& name,Cubemap*& cubemap) const;
		
		const ::std::map<std::string,base::Float4>& mapFloat4() const;
		const ::std::map<std::string,base::Float4x4>& mapFloat4x4() const;
		const ::std::map<std::string,ion_int32>& mapInt() const;
		const ::std::map<std::string,bool>& mapBool() const;
		const ::std::map<std::string,Texture2D*>& mapTex2D() const;
		const ::std::map<std::string,Cubemap*>& mapCubemap() const;

		bool isValid() const;

		inline void refcountTextures(const bool b) { m_RefcountTextures=b; }
		inline bool texturesRefcounted() const { return m_RefcountTextures; }
	protected:
		bool m_RefcountTextures;

		struct STLMaps;
		STLMaps *m_pSTLMaps;
	};

}
}

#endif
