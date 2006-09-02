#ifndef ION_VIDEO_CAPABILITIES_HH_INCLUDED
#define ION_VIDEO_CAPABILITIES_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../base/stringlist.hh"
#include "pixelformat.hh"

namespace ion {
namespace video {

	class ION_API Caps
	{
	public:
		Caps();
		void dumpToLog() const;





		bool m_NPOTTextures,m_RectangularTextures,m_Cubemaps,m_NPOTCubemaps,m_CompressedTextures,
			m_ProjectedTextures,m_HWOcclusionQueries,m_DisplacementMaps,m_Rendertargets;

		ion_uint32 m_MaxTextureWidth,m_MaxTextureHeight,m_MaxTextureVolumeDepth,m_MaxAnisotropy,
			m_MaxTextureBlendStages,m_MaxActiveFFLights,
			m_MaxPrimitiveCount,m_MaxVertexIndex;
		float m_MaxPointSize;

		struct Texflags {
			bool m_Supported,m_Rendertarget,m_Dynamic,m_Depth,m_DisplacementMap;
		};

		Texflags m_Valid2DTextureFormats[Pixelformat_NumFormats];
		Texflags m_Valid3DTextureFormats[Pixelformat_NumFormats];
		Texflags m_ValidCubemapFormats[Pixelformat_NumFormats];

		base::Stringlist m_SupportedProgramFormats;
	};

}
}

#endif
