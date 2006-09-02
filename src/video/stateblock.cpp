#include "coreenums.hh"
#include "stateblock.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	void Stateblock::setDefaultValues()
	{
		m_pCurrentVertexProgram=0;
		m_pCurrentFragmentProgram=0;

		m_Blending=false;
		m_Depthbuffer=true;
		m_Depthwrite=true;
		m_Stencilbuffer=false;
		m_Culling=true;
		m_Stencilwritemask=0xFFFFFFFF;
		m_Stencilref=0;
		m_Stencilmask=0;
		m_Cullingmode=Cullingmode_Counterclockwise;
		m_Srcblend=BlendFunc_One;
		m_Destblend=BlendFunc_Zero;
		m_Depthfunc=CompareFunc_Lessequal;
		m_Stencilfunc=CompareFunc_Lessequal;
		m_StencilFail=StencilOp_Zero;
		m_StencilDepthFail=StencilOp_Zero;
		m_StencilPass=StencilOp_Zero;
		m_Drawingmode=Drawingmode_Faces;
		m_Solidshading=Solidshading_Smooth;
		m_Depthscaledbias=0;
		m_Depthuniformbias=0;

		unsigned int i=0;

		for (i=0;i<MaxNumSamplerstages;++i) {
			m_Samplerstageparameter[i][Samplerstageparameter_Minfilter]=Texfilter_Point;
			m_Samplerstageparameter[i][Samplerstageparameter_Magfilter]=Texfilter_Point;
			m_Samplerstageparameter[i][Samplerstageparameter_Mipfilter]=Texfilter_Point;
			m_Samplerstageparameter[i][Samplerstageparameter_UWrap]=Texwrap_Repeat;
			m_Samplerstageparameter[i][Samplerstageparameter_VWrap]=Texwrap_Repeat;
			m_Samplerstageparameter[i][Samplerstageparameter_WWrap]=Texwrap_Repeat;
			m_Samplerstageparameter[i][Samplerstageparameter_MaxAnisotropy]=0;
		}

		for (i=0;i<MaxNumTexstages;++i) {
			m_Texstageparameter[i][Texstageparameter_Colorop]=(i==0) ? Texstageoperation_Selectarg1 : Texstageoperation_Disable;
			m_Texstageparameter[i][Texstageparameter_Colorarg1]=(i==0) ? Texstageargument_Texture : Texstageargument_Current;
			m_Texstageparameter[i][Texstageparameter_Colorarg2]=Texstageargument_Current;
			m_Texstageparameter[i][Texstageparameter_Alphaop]=Texstageoperation_Disable;
			m_Texstageparameter[i][Texstageparameter_Alphaarg1]=Texstageargument_Current;
			m_Texstageparameter[i][Texstageparameter_Alphaarg2]=Texstageargument_Current;
			m_Texstageparameter[i][Texstageparameter_Texcoordindex]=0;
			m_Texstageparameter[i][Texstageparameter_Textransformations]=0;
			m_Texstageparameter[i][Texstageparameter_Constant]=0;
			m_Texstageparameter[i][Texstageparameter_Texcoordgeneration]=Texcoordgeneration_None;
		}
	}

	Stateblock::Stateblock():m_pVideodevice(0)
	{
		setDefaultValues();
	}

	Stateblock::Stateblock(Videodevice* pVideodevice):m_pVideodevice(pVideodevice)
	{
		setDefaultValues();
	}

	Stateblock::Stateblock(const Stateblock& src):m_pVideodevice(0)
	{
		setDefaultValues();
		clone(src);
	}

	Stateblock::~Stateblock()
	{
	}

	Stateblock& Stateblock::operator =(const Stateblock& src)
	{
		clone(src);
		return *this;
	}

	void Stateblock::clone(const Stateblock& src)
	{
		m_pVideodevice=src.videodevice();

		m_Blending=src.blending();
		m_Depthbuffer=src.depthbuffer();
		m_Depthwrite=src.depthwrite();
		m_Stencilbuffer=src.stencilbuffer();
		m_Culling=src.culling();
		m_Stencilwritemask=src.stencilwritemask();
		m_Stencilref=src.stencilfuncref();
		m_Stencilmask=src.stencilfuncmask();
		m_Cullingmode=src.cullmode();
		m_Srcblend=src.srcblend();
		m_Destblend=src.destblend();
		m_Depthfunc=src.depthfunc();
		m_Stencilfunc=src.stencilcomparefunc();
		m_StencilFail=src.stencilopfail();
		m_StencilDepthFail=src.stencilopdepthfail();
		m_StencilPass=src.stenciloppass();
		m_Drawingmode=src.drawingmode();
		m_Solidshading=src.solidshading();
		m_Depthscaledbias=src.depthscaledbias();
		m_Depthuniformbias=src.depthuniformbias();

		unsigned int i;

		for (i=0;i<MaxNumSamplerstages;++i) {
			m_Samplerstageparameter[i][Samplerstageparameter_Minfilter]=src.samplerstageParameterInt(i,Samplerstageparameter_Minfilter);
			m_Samplerstageparameter[i][Samplerstageparameter_Magfilter]=src.samplerstageParameterInt(i,Samplerstageparameter_Magfilter);
			m_Samplerstageparameter[i][Samplerstageparameter_Mipfilter]=src.samplerstageParameterInt(i,Samplerstageparameter_Mipfilter);
			m_Samplerstageparameter[i][Samplerstageparameter_UWrap]=src.samplerstageParameterInt(i,Samplerstageparameter_UWrap);
			m_Samplerstageparameter[i][Samplerstageparameter_VWrap]=src.samplerstageParameterInt(i,Samplerstageparameter_VWrap);
			m_Samplerstageparameter[i][Samplerstageparameter_WWrap]=src.samplerstageParameterInt(i,Samplerstageparameter_WWrap);
			m_Samplerstageparameter[i][Samplerstageparameter_MaxAnisotropy]=src.samplerstageParameterInt(i,Samplerstageparameter_MaxAnisotropy);
		}

		for (i=0;i<MaxNumTexstages;++i) {
			m_Texstageparameter[i][Texstageparameter_Colorop]=src.texstageParameterInt(i,Texstageparameter_Colorop);
			m_Texstageparameter[i][Texstageparameter_Colorarg1]=src.texstageParameterInt(i,Texstageparameter_Colorarg1);
			m_Texstageparameter[i][Texstageparameter_Colorarg2]=src.texstageParameterInt(i,Texstageparameter_Colorarg2);
			m_Texstageparameter[i][Texstageparameter_Alphaop]=src.texstageParameterInt(i,Texstageparameter_Alphaop);
			m_Texstageparameter[i][Texstageparameter_Alphaarg1]=src.texstageParameterInt(i,Texstageparameter_Alphaarg1);
			m_Texstageparameter[i][Texstageparameter_Alphaarg2]=src.texstageParameterInt(i,Texstageparameter_Alphaarg2);
			m_Texstageparameter[i][Texstageparameter_Texcoordindex]=src.texstageParameterInt(i,Texstageparameter_Texcoordindex);
			m_Texstageparameter[i][Texstageparameter_Textransformations]=src.texstageParameterInt(i,Texstageparameter_Textransformations);
			m_Texstageparameter[i][Texstageparameter_Constant]=src.texstageParameterInt(i,Texstageparameter_Constant);
			m_Texstageparameter[i][Texstageparameter_Texcoordgeneration]=src.texstageParameterInt(i,Texstageparameter_Texcoordgeneration);
		}
	}

	Videodevice* Stateblock::videodevice() const
	{
		return m_pVideodevice;
	}

#define SETSTATEONEPARAM(param1,var1,func1) \
		{ \
			if ((param1)==(var1)) return; \
			if (m_pVideodevice!=0) m_pVideodevice->func1(param1); \
			var1=param1; \
		}

	// hw blending. default: false
	void Stateblock::blending(const bool state)
	{
		SETSTATEONEPARAM(state,m_Blending,blending);
		/*if (m_Blending==state) return;
		if (m_pVideodevice!=0) m_pVideodevice->blending(state);
		m_Blending=state;*/
	}

	bool Stateblock::blending() const
	{
		return m_Blending;
	}

	// handles depthbuffer (both test & writing). default: true
	void Stateblock::depthbuffer(const bool state)
	{
		SETSTATEONEPARAM(state,m_Depthbuffer,zbuffer);
	}

	bool Stateblock::depthbuffer() const
	{
		return m_Depthbuffer;
	}

	// depth writing. default: true
	void Stateblock::depthwrite(const bool state)
	{
		SETSTATEONEPARAM(state,m_Depthwrite,zwrite);
	}

	bool Stateblock::depthwrite() const
	{
		return m_Depthwrite;
	}

	// handles stencil buffer (both test & writing). default: false
	void Stateblock::stencilbuffer(const bool state)
	{
		SETSTATEONEPARAM(state,m_Stencilbuffer,stencilbuffer);
	}

	bool Stateblock::stencilbuffer() const
	{
		return m_Stencilbuffer;
	}

	// stencil writing. default: 0xFFFFFFFF
	void Stateblock::stencilwritemask(const ion_uint32 stencilmask)
	{
		SETSTATEONEPARAM(stencilmask,m_Stencilwritemask,stencilwritemask);
	}

	ion_uint32 Stateblock::stencilwritemask() const
	{
		return m_Stencilwritemask;
	}

	// backface culling. default: true
	void Stateblock::culling(const bool state)
	{
		SETSTATEONEPARAM(state,m_Culling,culling);
	}

	bool Stateblock::culling() const
	{
		return m_Culling;
	}

	// culling mode. default: counter clockwise
	void Stateblock::cullmode(const Cullingmode mode)
	{
		SETSTATEONEPARAM(mode,m_Cullingmode,cullmode);
	}

	Cullingmode Stateblock::cullmode() const
	{
		return m_Cullingmode;
	}

	// blending function. default: one,zero
	void Stateblock::blendfunc(const BlendFunc srcblend,const BlendFunc destblend)
	{
		if ((m_Srcblend==srcblend) && (m_Destblend==destblend)) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->blendfunc(srcblend,destblend);
		}
		m_Srcblend=srcblend;
		m_Destblend=destblend;
	}

	BlendFunc Stateblock::srcblend() const
	{
		return m_Srcblend;
	}

	BlendFunc Stateblock::destblend() const
	{
		return m_Destblend;
	}

	// depth test function. default: lessequal
	void Stateblock::depthfunc(const CompareFunc func)
	{
		SETSTATEONEPARAM(func,m_Depthfunc,zfunc);
	}

	CompareFunc Stateblock::depthfunc() const
	{
		return m_Depthfunc;
	}

	// stencil test function. default: lessequal,0,0
	void Stateblock::stencilfunc(const CompareFunc func,const ion_uint32 ref,const ion_uint32 mask)
	{
		if ((m_Stencilfunc==func) && (m_Stencilref==ref) && (m_Stencilmask==mask)) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->stencilfunc(func,ref,mask);
		}
		m_Stencilfunc=func;
		m_Stencilref=ref;
		m_Stencilmask=mask;
	}

	CompareFunc Stateblock::stencilcomparefunc() const
	{
		return m_Stencilfunc;
	}

	ion_uint32 Stateblock::stencilfuncref() const
	{
		return m_Stencilref;
	}

	ion_uint32 Stateblock::stencilfuncmask() const
	{
		return m_Stencilmask;
	}

	// stencil operations. default: zero,zero,zero
	void Stateblock::stencilop(const StencilOp opfail,const StencilOp opdepthfail,const StencilOp oppass)
	{
		if ((m_StencilFail==opfail) && (m_StencilDepthFail==opdepthfail) && (m_StencilPass==oppass)) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->stencilop(opfail,opdepthfail,oppass);
		}
		m_StencilFail=opfail;
		m_StencilDepthFail=opdepthfail;
		m_StencilPass=oppass;
	}

	StencilOp Stateblock::stencilopfail() const
	{
		return m_StencilFail;
	}

	StencilOp Stateblock::stencilopdepthfail() const
	{
		return m_StencilDepthFail;
	}

	StencilOp Stateblock::stenciloppass() const
	{
		return m_StencilPass;
	}

	// geometry drawing mode. default: faces
	void Stateblock::drawingmode(const Drawingmode mode)
	{
		SETSTATEONEPARAM(mode,m_Drawingmode,drawingmode);
	}

	Drawingmode Stateblock::drawingmode() const
	{
		return m_Drawingmode;
	}

	// shading of filled triangles. default: smooth
	void Stateblock::solidshading(const Solidshading type)
	{
		SETSTATEONEPARAM(type,m_Solidshading,solidshading);
	}

	Solidshading Stateblock::solidshading() const
	{
		return m_Solidshading;
	}

	// depth bias. default: 0,0 (disables depth bias)
	void Stateblock::depthbias(const float scaledbias,const float uniformbias)
	{
		if ((m_Depthscaledbias==scaledbias) && (m_Depthuniformbias==uniformbias)) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->depthbias(scaledbias,uniformbias);
		}
		m_Depthscaledbias=scaledbias;
		m_Depthuniformbias=uniformbias;
	}

	float Stateblock::depthscaledbias() const
	{
		return m_Depthscaledbias;
	}

	float Stateblock::depthuniformbias() const
	{
		return m_Depthuniformbias;
	}


	// ************* Sample stage parameters

	unsigned long Stateblock::samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const
	{
		return m_Samplerstageparameter[samplerstage][sampparam];
	}

	float Stateblock::samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const
	{
		//return m_Samplerstageparameter[samplerstage][sampparam];
		return 0;
	}

	void Stateblock::samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const unsigned long value)
	{
		if (m_Samplerstageparameter[samplerstage][sampparam]==value) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->samplerstageParameterInt(samplerstage,sampparam,value);
		}
		m_Samplerstageparameter[samplerstage][sampparam]=value;
	}

	void Stateblock::samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const float value)
	{
	}

	// ************* Texture stage parameters

	unsigned long Stateblock::texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam) const
	{
		return m_Samplerstageparameter[texstage][texparam];
	}

	float Stateblock::texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam) const
	{
		return 0;
	}

	void Stateblock::texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam,const unsigned long value)
	{
		if (m_Texstageparameter[texstage][texparam]==value) return;
		if (m_pVideodevice!=0) {
			m_pVideodevice->texstageParameterInt(texstage,texparam,value);
		}
		m_Texstageparameter[texstage][texparam]=value;
	}

	void Stateblock::texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam,const float value)
	{
	}




	void Stateblock::invoke(Videodevice* pVideodevice,Stateblock *pReferenceBlock)
	{
		if ((m_pVideodevice==0) && (pVideodevice==0)) return;

		Videodevice *pV=(pVideodevice!=0) ? pVideodevice : m_pVideodevice;

		if (pReferenceBlock==0) invokeNoRef(pV);
		else invokeWithRef(pV,pReferenceBlock);
	}

	void Stateblock::invokeNoRef(Videodevice* pVideodevice)
	{
		pVideodevice->blending(m_Blending);
		pVideodevice->zbuffer(m_Depthbuffer);
		pVideodevice->zwrite(m_Depthwrite);
		pVideodevice->stencilbuffer(m_Stencilbuffer);
		pVideodevice->stencilwritemask(m_Stencilwritemask);
		pVideodevice->culling(m_Culling);
		pVideodevice->cullmode(m_Cullingmode);
		pVideodevice->blendfunc(m_Srcblend,m_Destblend);
		pVideodevice->zfunc(m_Depthfunc);
		pVideodevice->stencilfunc(m_Stencilfunc,m_Stencilref,m_Stencilmask);
		pVideodevice->stencilop(m_StencilFail,m_StencilDepthFail,m_StencilPass);
		pVideodevice->drawingmode(m_Drawingmode);
		pVideodevice->solidshading(m_Solidshading);
		pVideodevice->depthbias(m_Depthscaledbias,m_Depthuniformbias);

		unsigned int i=0;

		for (i=0;i<MaxNumSamplerstages;++i) {
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,m_Samplerstageparameter[i][Samplerstageparameter_Minfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,m_Samplerstageparameter[i][Samplerstageparameter_Magfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,m_Samplerstageparameter[i][Samplerstageparameter_Mipfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_UWrap,m_Samplerstageparameter[i][Samplerstageparameter_UWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_VWrap,m_Samplerstageparameter[i][Samplerstageparameter_VWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_WWrap,m_Samplerstageparameter[i][Samplerstageparameter_WWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_MaxAnisotropy,m_Samplerstageparameter[i][Samplerstageparameter_MaxAnisotropy]);
		}

		for (i=0;i<MaxNumTexstages;++i) {
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorop,m_Texstageparameter[i][Texstageparameter_Colorop]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorarg1,m_Texstageparameter[i][Texstageparameter_Colorarg1]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorarg2,m_Texstageparameter[i][Texstageparameter_Colorarg2]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaop,m_Texstageparameter[i][Texstageparameter_Alphaop]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaarg1,m_Texstageparameter[i][Texstageparameter_Alphaarg1]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaarg2,m_Texstageparameter[i][Texstageparameter_Alphaarg2]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Texcoordindex,m_Texstageparameter[i][Texstageparameter_Texcoordindex]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Textransformations,m_Texstageparameter[i][Texstageparameter_Textransformations]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Constant,m_Texstageparameter[i][Texstageparameter_Constant]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Texcoordgeneration,m_Texstageparameter[i][Texstageparameter_Texcoordgeneration]);
		}
	}

	void Stateblock::invokeWithRef(Videodevice* pVideodevice,Stateblock *pRef)
	{
		if (pRef->blending()!=m_Blending) pVideodevice->blending(m_Blending);
		if (pRef->depthbuffer()!=m_Depthbuffer) pVideodevice->zbuffer(m_Depthbuffer);
		if (pRef->depthwrite()!=m_Depthwrite) pVideodevice->zwrite(m_Depthwrite);
		if (pRef->stencilbuffer()!=m_Stencilbuffer) pVideodevice->stencilbuffer(m_Stencilbuffer);
		if (pRef->stencilwritemask()!=m_Stencilwritemask) pVideodevice->stencilwritemask(m_Stencilwritemask);
		if (pRef->culling()!=m_Culling) pVideodevice->culling(m_Culling);
		if (pRef->cullmode()!=m_Cullingmode) pVideodevice->cullmode(m_Cullingmode);
		if ((pRef->srcblend()!=m_Srcblend) || (pRef->destblend()!=m_Destblend))
			pVideodevice->blendfunc(m_Srcblend,m_Destblend);
		if (pRef->depthfunc()!=m_Depthfunc) pVideodevice->zfunc(m_Depthfunc);
		if ((pRef->stencilcomparefunc()!=m_Stencilfunc) || (pRef->stencilfuncref()!=m_Stencilref) || (pRef->stencilfuncmask()!=m_Stencilmask))
			pVideodevice->stencilfunc(m_Stencilfunc,m_Stencilref,m_Stencilmask);
		if ((pRef->stencilopfail()!=m_StencilFail) || (pRef->stencilopdepthfail()!=m_StencilDepthFail) || (pRef->stenciloppass()!=m_StencilPass))
			pVideodevice->stencilop(m_StencilFail,m_StencilDepthFail,m_StencilPass);
		if (pRef->drawingmode()!=m_Drawingmode) pVideodevice->drawingmode(m_Drawingmode);
		if (pRef->solidshading()!=m_Solidshading) pVideodevice->solidshading(m_Solidshading);
		if ((pRef->depthscaledbias()!=m_Depthscaledbias) || (pRef->depthuniformbias()!=m_Depthuniformbias))
			pVideodevice->depthbias(m_Depthscaledbias,m_Depthuniformbias);

		unsigned int i=0;

		for (i=0;i<MaxNumSamplerstages;++i) {
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,m_Samplerstageparameter[i][Samplerstageparameter_Minfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,m_Samplerstageparameter[i][Samplerstageparameter_Magfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,m_Samplerstageparameter[i][Samplerstageparameter_Mipfilter]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_UWrap,m_Samplerstageparameter[i][Samplerstageparameter_UWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_VWrap,m_Samplerstageparameter[i][Samplerstageparameter_VWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_WWrap,m_Samplerstageparameter[i][Samplerstageparameter_WWrap]);
			pVideodevice->samplerstageParameterInt(i,Samplerstageparameter_MaxAnisotropy,m_Samplerstageparameter[i][Samplerstageparameter_MaxAnisotropy]);
		}

		for (i=0;i<MaxNumTexstages;++i) {
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorop,m_Texstageparameter[i][Texstageparameter_Colorop]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorarg1,m_Texstageparameter[i][Texstageparameter_Colorarg1]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Colorarg2,m_Texstageparameter[i][Texstageparameter_Colorarg2]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaop,m_Texstageparameter[i][Texstageparameter_Alphaop]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaarg1,m_Texstageparameter[i][Texstageparameter_Alphaarg1]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Alphaarg2,m_Texstageparameter[i][Texstageparameter_Alphaarg2]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Texcoordindex,m_Texstageparameter[i][Texstageparameter_Texcoordindex]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Textransformations,m_Texstageparameter[i][Texstageparameter_Textransformations]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Constant,m_Texstageparameter[i][Texstageparameter_Constant]);
			pVideodevice->texstageParameterInt(i,Texstageparameter_Texcoordgeneration,m_Texstageparameter[i][Texstageparameter_Texcoordgeneration]);
		}
	}

}
}
