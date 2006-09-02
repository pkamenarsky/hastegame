#include "renderer.hh"

namespace ion {
namespace scene {

	Renderer::Renderer(video::Videodevice &rVideodevice):m_pVideodevice(&rVideodevice)
	{
	}

	video::Videodevice* Renderer::videodevice()
	{
		return m_pVideodevice;
	}

}
}
