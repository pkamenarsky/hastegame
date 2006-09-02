#include "../video/propertytable.hh"
#include "dot3shader.hh"
#include "light.hh"
#include "lightlist.hh"

namespace ion {
namespace scene {

	struct Programentry {
		const char *m_Identifier,*m_Entrypoint,*m_Code,*m_Format;
	};

	const Programentry dot3vertexprograms[]={
		{
			//"d3d_vs20_dot3",
			"cgvertexprogram",
			"vs_main",
			"\
uniform float4x4 matWorldViewProjection;\
uniform float3 lightpos;\
uniform float4 vViewPosition;\
\
struct VS_INPUT \
{\
   float4 Position : POSITION;\
   float3 Normal: NORMAL;\
   float2 Texcoord: TEXCOORD0;\
   float3 Tangent: TEXCOORD1;\
   float3 Binormal: TEXCOORD2;\
};\
\
struct VS_OUTPUT \
{\
   float4 Position : POSITION;\
   float2 Texcoord: TEXCOORD0;\
   float3 Lightvector: TEXCOORD1;\
   float3 ViewVector: TEXCOORD2;\
};\
\
VS_OUTPUT vs_main( VS_INPUT Input )\
{\
   VS_OUTPUT Output;\
\
   Output.Position = mul( matWorldViewProjection, Input.Position );\
   Output.Texcoord = Input.Texcoord;\
\
   float3x3 texspace=float3x3(Input.Tangent,Input.Binormal,Input.Normal);\
\
   float3 lightvector=normalize(lightpos-Input.Position );\
\
   Output.Lightvector=mul(texspace,lightvector);\
   Output.ViewVector=mul(texspace,(vViewPosition-Input.Position).xyz);\
\
   return( Output );\
\
}\
",
			//"d3d_hlsl_vs20"
			"cg_vprogram"
		},

		{0,0,0,0}
	};

	const Programentry dot3fragmentprograms[]={
		{
			"d3d_ps20_dot3",
			//"cgfragmentprogram",
			"ps_main",
			"\
uniform sampler2D Diffuse;\
uniform sampler2D NMap;\
uniform float exponent;\
\
float4 ps_main(float2 Texcoord: TEXCOORD0,float3 Lightvector: TEXCOORD1,float3 ViewVector: TEXCOORD2) : COLOR0\
{\
   float3 color=tex2D(Diffuse,Texcoord);\
   float3 normal=tex2D(NMap,Texcoord)*2.0f-1.0f;\
\
   float3 lightvec=normalize(Lightvector);\
   float3 viewvec=normalize(ViewVector);\
   float3 n=normalize(normal);\
\
   float fd=clamp(dot(lightvec,n),0,1);\
\
   float fs=pow(clamp(dot(normalize(lightvec+viewvec),n),0,1),exponent);\
\
   return( float4(fd*color+fs*float3(1,1,1), 1.0f ) ); \
  /*return float4(fd*color,1.0f);*/ \
\
}\
",
			//"d3d_hlsl_ps20"
			"cg_fprogram"
		},

		{0,0,0,0}
	};

	Dot3Shader::Dot3Shader(const base::String& shaderidentifier,video::Videodevice &rVideodevice):Shader(shaderidentifier),
		m_rVideodevice(rVideodevice),m_pVertexprogram(0),m_pFragmentprogram(0),m_pLightlist(0),m_Lightlistoffset(0)
	{
		const Programentry *progentry=dot3vertexprograms;

		while (progentry->m_Identifier!=0) {
			m_pVertexprogram=m_rVideodevice.createVertexProgram(progentry->m_Identifier,progentry->m_Entrypoint,progentry->m_Code,progentry->m_Format);

			if (m_pVertexprogram!=0) {
				if (m_pVertexprogram->isValid()) break;
				else { delete m_pVertexprogram; m_pVertexprogram=0; }
			}

			++progentry;
		}

		progentry=dot3fragmentprograms;
		while (progentry->m_Identifier!=0) {
			m_pFragmentprogram=m_rVideodevice.createFragmentProgram(progentry->m_Identifier,progentry->m_Entrypoint,progentry->m_Code,progentry->m_Format);

			if (m_pFragmentprogram!=0) {
				if (m_pFragmentprogram->isValid()) break;
				else { delete m_pFragmentprogram; m_pFragmentprogram=0; }
			}

			++progentry;
		}
	}

	Dot3Shader::~Dot3Shader()
	{
		if (m_pVertexprogram) delete m_pVertexprogram;
		if (m_pFragmentprogram) delete m_pFragmentprogram;
	}

	void Dot3Shader::begin()
	{
	}

	void Dot3Shader::end()
	{
		m_rVideodevice.blending(false);
	}

	const math::Transform& Dot3Shader::worldtransform() const
	{
		return m_Worldtransform;
	}

	void Dot3Shader::worldtransform(const math::Transform& transform)
	{
		m_Worldtransform=transform;
		m_rVideodevice.worldmatrix(m_Worldtransform.matrix());
	}

	void Dot3Shader::passValues(const video::Propertytable& values)
	{
		video::Texture2D *pTex=0,*pNMap=0;
		
		// TODO: optimize fetch2DTexture()
		values.fetch2DTexture("diffuseTexture",pTex);
		values.fetch2DTexture("normalMap",pNMap);

		m_pFragmentprogram->bind2DTexture("Diffuse",pTex);
		m_pFragmentprogram->bind2DTexture("NMap",pNMap);
	}

	void Dot3Shader::beginPass(const ion_uint32 passnum,const Passtype passtype)
	{
		switch (passtype) {
			case Shader::AmbientPass:
				m_rVideodevice.activateFFPipeline(video::FixedfunctionVertex);
				m_rVideodevice.activateFFPipeline(video::FixedfunctionFragment);
				m_rVideodevice.blending(false);
				m_rVideodevice.blendfunc(video::BlendFunc_Zero,video::BlendFunc_Zero);
				m_rVideodevice.fixedSetMaterialAmbientColor(base::Float4(1,0,0,0));
				m_rVideodevice.fixedSetMaterialDiffuseColor(base::Float4(1,0,0,0));
				m_rVideodevice.fixedSetMaterialEmissiveColor(base::Float4(1,0,0,0));
				m_rVideodevice.fixedSetMaterialShininess(0);
				m_rVideodevice.zwrite(true);
				m_rVideodevice.zfunc(video::CompareFunc_Lessequal);
				m_rVideodevice.fixedLighting(true);
				m_rVideodevice.depthbias(0,0);
				break;
			case Shader::PointlightPass:
				m_pVertexprogram->set();
				m_pFragmentprogram->set();

				m_rVideodevice.blending(true);
				m_rVideodevice.blendfunc(video::BlendFunc_One,video::BlendFunc_One);
				m_rVideodevice.fixedSetMaterialDiffuseColor(base::Float4(1,1,1,1));
				m_rVideodevice.zwrite(false);
				m_rVideodevice.zfunc(video::CompareFunc_Lessequal);
				m_rVideodevice.fixedLighting(false);
				m_rVideodevice.depthbias(-0.01f,0);
				break;
			default:break;
		}
	}

	void Dot3Shader::endPass()
	{
	}

	ion_uint32 Dot3Shader::numPasses(const Passtype passtype) const
	{
		return ((passtype==Shader::AmbientPass) || (passtype==Shader::PointlightPass)) ? 1 : 0;
	}

	bool Dot3Shader::blended() const
	{
		return false;
	}

	ion_uint32 Dot3Shader::maxSimultaneousLights(const video::Lighttype lighttype) const
	{
		return (lighttype==video::Lighttype_Point) ? 1 : 0;
	}

	void Dot3Shader::lightlist(const Lightlist& lightlist,const ion_uint32 offset)
	{
		m_pLightlist=&lightlist;
		m_Lightlistoffset=offset;

		const Light& rLight=*(lightlist.pointlight(offset));

		math::Vector3f lpos=m_rVideodevice.worldmatrix().invert()*rLight.transform().position();

		m_pFragmentprogram->setFloat("exponent",rLight.specularExponent());
		m_pVertexprogram->setFloat4x4("matWorldViewProjection",m_rVideodevice.worldViewProj());
		m_pVertexprogram->setFloat3("lightpos",lpos);
		m_pVertexprogram->setFloat4("vViewPosition",m_rVideodevice.eyePosition());
	}

	bool Dot3Shader::supported() const
	{
		return (m_pVertexprogram!=0) && (m_pFragmentprogram!=0);
	}

	bool Dot3Shader::isValid() const
	{
		return supported() && m_rVideodevice.isValid();
	}

}
}
