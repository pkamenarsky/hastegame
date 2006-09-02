#include "lightlist.hh"
#include "renderable.hh"
#include "renderer.hh"
#include "renderjob.hh"
#include "../video/videodevice.hh"

namespace ion {
namespace scene {

	Renderable::Renderable(const base::String& identifier):Node(identifier),m_pRenderer(0),m_pLightlist(new Lightlist)
	{
	}

	Renderable::~Renderable()
	{
		delete m_pLightlist;
	}

	void Renderable::addRendertargetcamera(Camera& rCamera)
	{
	}

	void Renderable::removeRendertargetcamera(Camera& rCamera)
	{
	}

	unsigned long Renderable::numRendertargetcameras() const
	{
		return 0;
	}

	const Camera* Renderable::rendertargetcamera(const unsigned long index) const
	{
		return 0;
	}

	Lightlist& Renderable::lightlist()
	{
		return *m_pLightlist;
	}

	const Lightlist& Renderable::lightlist() const
	{
		return *m_pLightlist;
	}

	void Renderable::renderer(Renderer& rRenderer)
	{
		bool b=true;
		if (m_pRenderer) b=(m_pRenderer->videodevice()!=rRenderer.videodevice());
		rendererChanging(b,&rRenderer);
		m_pRenderer=&rRenderer;
	}

	Renderer* Renderable::renderer()
	{
		return m_pRenderer;
	}

	const Renderer* Renderable::renderer() const
	{
		return m_pRenderer;
	}

	void Renderable::executeBlendedJob(const ion_uint32 jobnr)
	{
	}

	ion_uint32 Renderable::numExecutableBlendedJobs()const
	{
		return 0;
	}

	void Renderable::executeBlendedLitJob(const ion_uint32 jobnr)
	{
	}

	ion_uint32 Renderable::numExecutableBlendedLitJobs()const
	{
		return 0;
	}

	ion_uint32 Renderable::numLightingJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return numJobs();
	}

	Renderjob* Renderable::lightingjob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return job(jobnr);
	}

	ion_uint32 Renderable::numPointlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return numJobs();
	}

	Renderjob* Renderable::pointlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return job(jobnr);
	}

	ion_uint32 Renderable::numDirlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return numJobs();
	}

	Renderjob* Renderable::dirlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return job(jobnr);
	}

	ion_uint32 Renderable::numSpecularlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return numJobs();
	}

	Renderjob* Renderable::specularlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return job(jobnr);
	}

	/*bool Renderable::extraLightingJobs() const
	{
		return false;
	}*/

}
}
