#include "../base/log.hh"
#include "caps.hh"

namespace ion {

	using namespace base;

	video::Caps::Caps()
	{
		m_NPOTTextures=m_RectangularTextures=m_Cubemaps=m_NPOTCubemaps=m_CompressedTextures=m_ProjectedTextures=
			m_HWOcclusionQueries=m_DisplacementMaps=m_Rendertargets=false;

		m_MaxTextureWidth=m_MaxTextureHeight=m_MaxTextureVolumeDepth=m_MaxAnisotropy=m_MaxTextureBlendStages=
			m_MaxActiveFFLights=m_MaxPrimitiveCount=m_MaxVertexIndex=0;

		m_MaxPointSize=0;

		for (unsigned long l=0;l<Pixelformat_NumFormats;++l) {
			m_Valid2DTextureFormats[l].m_Supported=false;
			m_Valid2DTextureFormats[l].m_Rendertarget=false;
			m_Valid2DTextureFormats[l].m_Dynamic=false;
			m_Valid2DTextureFormats[l].m_Depth=false;
			m_Valid2DTextureFormats[l].m_DisplacementMap=false;

			m_Valid3DTextureFormats[l].m_Supported=false;
			m_Valid3DTextureFormats[l].m_Rendertarget=false;
			m_Valid3DTextureFormats[l].m_Dynamic=false;
			m_Valid3DTextureFormats[l].m_Depth=false;
			m_Valid3DTextureFormats[l].m_DisplacementMap=false;

			m_ValidCubemapFormats[l].m_Supported=false;
			m_ValidCubemapFormats[l].m_Rendertarget=false;
			m_ValidCubemapFormats[l].m_Dynamic=false;
			m_ValidCubemapFormats[l].m_Depth=false;
			m_ValidCubemapFormats[l].m_DisplacementMap=false;
		}
	}

	void video::Caps::dumpToLog() const
	{
#define YESNO(b) ((b) ? "yes" : "no")
#define ISSUPPORTED(b) ((b) ? "supported" : "unsupported")

		logstream() << "Video device capabilities:\n";
		logstream() << "  Non-power of two textures: " << ISSUPPORTED(m_NPOTTextures) << "\n";
		logstream() << "  Rectangular textures: " << ISSUPPORTED(m_RectangularTextures) << "\n";
		logstream() << "  Cubemaps: " << ISSUPPORTED(m_Cubemaps) << "\n";
		logstream() << "  Non-power of two cubemaps: " << ISSUPPORTED(m_NPOTCubemaps) << "\n";
		logstream() << "  Compressed textures: " << ISSUPPORTED(m_CompressedTextures) << "\n";
		logstream() << "  Projected textures: " << ISSUPPORTED(m_ProjectedTextures) << "\n";
		logstream() << "  Hardware occlusion queries: " << ISSUPPORTED(m_HWOcclusionQueries) << "\n";
		logstream() << "  Displacement maps: " << ISSUPPORTED(m_DisplacementMaps) << "\n";
		logstream() << "  Rendertargets: " << ISSUPPORTED(m_Rendertargets) << "\n";

		logstream() << "\n";

		logstream() << "  Max texture width: " << m_MaxTextureWidth << "\n";
		logstream() << "  Max texture height: " << m_MaxTextureHeight << "\n";
		logstream() << "  Max texture depth: " << m_MaxTextureVolumeDepth << "\n";
		logstream() << "  Max anisotropy: " << m_MaxAnisotropy << "\n";
		logstream() << "  Max texture blending stages: " << m_MaxTextureBlendStages << "\n";
		logstream() << "  Max active ffp lights: " << m_MaxActiveFFLights << "\n";
		logstream() << "  Max primitive count: " << m_MaxPrimitiveCount << "\n";
		logstream() << "  Max vertex index: " << m_MaxVertexIndex << "\n";
		logstream() << "  Max point size: " << m_MaxPointSize << "\n";

		logstream() << "\n";

		logstream() << "  Supported GPU program formats:\n";
		for (size_t i=0;i<m_SupportedProgramFormats.numStrings();++i)
			logstream() << "    " << (*m_SupportedProgramFormats.string(i)) << "\n";

		logstream() << "\nCapabilities dump complete.\n";
	}

}
