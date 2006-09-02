#ifndef ION_VIDEO_STATEBLOCK_HH_INCLUDED
#define ION_VIDEO_STATEBLOCK_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/coreenums.hh"
#include "../video/program.hh"

namespace ion {
namespace video {

	class Videodevice;

	class ION_API Stateblock
	{
	public:
		Stateblock();
		Stateblock(Videodevice* pVideodevice);
		Stateblock(const Stateblock& src);
		~Stateblock();

		Stateblock& operator =(const Stateblock& src);
		void clone(const Stateblock& src);

		Videodevice* videodevice() const;

		// hw blending. default: false
		void blending(const bool state);
		bool blending() const;
		// handles depthbuffer (both test & writing). default: true
		void depthbuffer(const bool state);
		bool depthbuffer() const;
		// depth writing. default: true
		void depthwrite(const bool state);
		bool depthwrite() const;
		// handles stencil buffer (both test & writing). default: false
		void stencilbuffer(const bool state);
		bool stencilbuffer() const;
		// stencil writing. default: 0xFFFFFFFF
		void stencilwritemask(const ion_uint32 stencilmask);
		ion_uint32 stencilwritemask() const;
		// backface culling. default: true
		void culling(const bool state);
		bool culling() const;
		// culling mode. default: counter clockwise
		void cullmode(const Cullingmode mode);
		Cullingmode cullmode() const;
		// blending function. default: one,zero
		void blendfunc(const BlendFunc srcblend,const BlendFunc destblend);
		BlendFunc srcblend() const;
		BlendFunc destblend() const;
		// depth test function. default: lessequal
		void depthfunc(const CompareFunc func);
		CompareFunc depthfunc() const;
		// stencil test function. default: lessequal,0,0
		void stencilfunc(const CompareFunc func,const ion_uint32 ref,const ion_uint32 mask);
		CompareFunc stencilcomparefunc() const;
		ion_uint32 stencilfuncref() const;
		ion_uint32 stencilfuncmask() const;
		// stencil operations. default: zero,zero,zero
		void stencilop(const StencilOp opfail,const StencilOp opdepthfail,const StencilOp oppass);
		StencilOp stencilopfail() const;
		StencilOp stencilopdepthfail() const;
		StencilOp stenciloppass() const;
		// geometry drawing mode. default: faces
		void drawingmode(const Drawingmode mode);
		Drawingmode drawingmode() const;
		// shading of filled triangles. default: smooth
		void solidshading(const Solidshading type);
		Solidshading solidshading() const;
		// depth bias. default: 0,0 (disables depth bias)
		void depthbias(const float scaledbias,const float uniformbias);
		float depthscaledbias() const;
		float depthuniformbias() const;

		// ************* Sample stage parameters

		unsigned long samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const;
		float samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam) const;
		void samplerstageParameterInt(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const unsigned long value);
		void samplerstageParameterFloat(const ion_uint32 samplerstage,const Samplerstageparameter sampparam,const float value);

		// ************* Texture stage parameters

		unsigned long texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam) const;
		float texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam) const;
		void texstageParameterInt(const ion_uint32 texstage,const Texstageparameter texparam,const unsigned long value);
		void texstageParameterFloat(const ion_uint32 texstage,const Texstageparameter texparam,const float value);




		void setDefaultValues();
		void invoke(Videodevice* pVideodevice=0,Stateblock *pReferenceBlock=0);

	protected:
		void invokeNoRef(Videodevice* pVideodevice);
		void invokeWithRef(Videodevice* pVideodevice,Stateblock *pRef);

		Videodevice *m_pVideodevice;
		video::Program *m_pCurrentVertexProgram;
		video::Program *m_pCurrentFragmentProgram;

		bool m_Blending,m_Depthbuffer,m_Depthwrite,m_Stencilbuffer,m_Culling;
		ion_uint32 m_Stencilwritemask,m_Stencilref,m_Stencilmask;
		Cullingmode m_Cullingmode;
		BlendFunc m_Srcblend,m_Destblend;
		CompareFunc m_Depthfunc,m_Stencilfunc;
		StencilOp m_StencilFail,m_StencilDepthFail,m_StencilPass;
		Drawingmode m_Drawingmode;
		Solidshading m_Solidshading;
		float m_Depthscaledbias,m_Depthuniformbias;

		ion_uint32 m_Samplerstageparameter[MaxNumSamplerstages][NumSamplerstageparameters];
		ion_uint32 m_Texstageparameter[MaxNumTexstages][NumTexstageparameters];
	};

}
}

#endif
