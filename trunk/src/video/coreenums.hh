#ifndef ION_VIDEO_COREENUMS_HH_INCLUDED
#define ION_VIDEO_COREENUMS_HH_INCLUDED

namespace ion {
namespace video {

	//! Window activation state.
	enum WindowActivationState {
		WindowDeactivated,			//!< Window has been deactivated since the last event update
		WindowActivated,			//!< Window has been (re)activated since the last event update
		NoWindowActivationChange	//!< No activation change since the last event update happened
	};

	enum Textureflags {
		Textureflag_IsDisplacementMap=1,
		Textureflag_IsDynamic=2,
		Textureflag_IsRendertaget=4
	};

	enum Streamflags {
		Streamflags_BufferObject=0,
		Streamflags_SoftwareTnL=1,
		Streamflags_Points=2,
		Streamflags_Writeonly=4,
		Streamflags_Dynamic=8
	};

	enum Mapflags {
		Map_Writeonly=1,
		Map_Readonly=2,
		Map_Discard=4,
		Map_Nooverwrite=8
	};

	enum Fixedfunctionactivationtype {
		FixedfunctionVertex,
		FixedfunctionFragment
	};

	enum Lighttype {
		Lighttype_Point,
		Lighttype_Directional,
		Lighttype_Spot
	};

	enum BlendFunc {
		BlendFunc_Zero=0,
		BlendFunc_One,
		BlendFunc_Src_Color,
		BlendFunc_One_Minus_Src_Color,
		BlendFunc_Dest_Color,
		BlendFunc_One_Minus_Dest_Color,
		BlendFunc_Src_Alpha,
		BlendFunc_One_Minus_Src_Alpha,
		BlendFunc_Dest_Alpha,
		BlendFunc_One_Minus_Dest_Alpha
	};

	enum CompareFunc {
		CompareFunc_Never=0,
		CompareFunc_Less,
		CompareFunc_Lessequal,
		CompareFunc_Equal,
		CompareFunc_Greaterequal,
		CompareFunc_Greater,
		CompareFunc_Notequal,
		CompareFunc_Always
	};

	enum StencilOp {
		StencilOp_Keep=0,
		StencilOp_Zero,
		StencilOp_Set_To_Reference,
		StencilOp_Incr,
		StencilOp_Decr,
		StencilOp_Invert
	};

	enum SpaceType {
		Space_Worldspace=0,
		Space_Objectspace,
		Space_Cameraspace
	};

	enum Solidshading {
		Solidshading_Flat,
		Solidshading_Smooth
	};

	enum Clearflags {
		Clear_Colorbuffer=1,
		Clear_Depthbuffer=2,
		Clear_Stencilbuffer=4
	};

	const unsigned int MaxNumSamplerstages=64;

	enum Samplerstageparameter
	{
		Samplerstageparameter_Minfilter=0,
		Samplerstageparameter_Magfilter,
		Samplerstageparameter_Mipfilter,
		Samplerstageparameter_UWrap,
		Samplerstageparameter_VWrap,
		Samplerstageparameter_WWrap,
		Samplerstageparameter_MaxAnisotropy,
	
		NumSamplerstageparameters
	};

	const unsigned int MaxNumTexstages=64;

	enum Texstageparameter
	{
		Texstageparameter_Colorop=0,
		Texstageparameter_Colorarg1,
		Texstageparameter_Colorarg2,
		Texstageparameter_Alphaop,
		Texstageparameter_Alphaarg1,
		Texstageparameter_Alphaarg2,
		Texstageparameter_Texcoordindex,
		Texstageparameter_Textransformations,
		Texstageparameter_Constant,

		Texstageparameter_Texcoordgeneration,
	
		NumTexstageparameters
	};

	enum Texstageoperation
	{
		Texstageoperation_Disable,
		Texstageoperation_Selectarg1,
		Texstageoperation_Selectarg2,
		Texstageoperation_Modulate1x,
		Texstageoperation_Modulate2x,
		Texstageoperation_Modulate4x,
		Texstageoperation_Add,
		Texstageoperation_Addsigned,
		Texstageoperation_Addsigned2x,
		Texstageoperation_Subtract,
		Texstageoperation_Addsmooth,
		Texstageoperation_BlendWithDiffusealpha,
		Texstageoperation_BlendWithTexturealpha,
		Texstageoperation_BlendWithFactoralpha,
		Texstageoperation_BlendWithCurrentalpha,
		Texstageoperation_Dotproduct3
	};

	enum Texstageargument
	{
		Texstageargument_Constant,
		Texstageargument_Current,
		Texstageargument_Diffuse,
		Texstageargument_Specular,
		Texstageargument_Texture,
		Texstageargument_Factor
	};

	enum Texcoordgeneration
	{
		Texcoordgeneration_None,
		Texcoordgeneration_CameraspaceNormal,
		Texcoordgeneration_CameraspacePosition,
		Texcoordgeneration_CameraspaceReflectionVector,
		Texcoordgeneration_Spheremapping
	};

	enum Texcoordtransform
	{
		Texcoordtransform_None=0,
		Texcoordtransform_1Coord=1,
		Texcoordtransform_2Coords=2,
		Texcoordtransform_3Coords=3,
		Texcoordtransform_4Coords=4,
		Texcoordtransform_ProjectedTextureMode=256
	};

	enum Texfilter
	{
		Texfilter_None=0,
		Texfilter_Point=1,
		Texfilter_Linear=2,
		Texfilter_Anisotropic=3
	};

	enum Texwrap
	{
		Texwrap_Repeat=1,
		Texwrap_Mirror=2,
		Texwrap_Clamp=3
	};

	enum Drawingmode
	{
		Drawingmode_Points,
		Drawingmode_Lines,
		Drawingmode_Faces
	};

	enum PrimitivesType
	{
		Primitives_Points,
		Primitives_Linestrip,
		Primitives_Lines,
		Primitives_Trianglestrip,
		Primitives_Trianglefan,
		Primitives_Triangles
	};

	enum Cullingmode
	{
		Cullingmode_Clockwise,
		Cullingmode_Counterclockwise
	};

}
}

#endif
