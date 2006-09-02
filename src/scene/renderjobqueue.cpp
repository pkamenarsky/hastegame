#include <algorithm>
#include <assert.h>
#include <set>

#include "../base/log.hh"
#include "camera.hh"
#include "renderjob.hh"
#include "renderjobqueue.hh"
#include "shader.hh"

namespace ion {
namespace scene {

	bool Renderjobqueue::sortShadedOpaqueJobs(const Renderjob* pJob1,const Renderjob* pJob2)
	{
		return pJob1->shader()==pJob2->shader();
	}

	bool Renderjobqueue::sortBlendedJobs(const Renderjob* pJob1,const Renderjob* pJob2)
	{
		return pJob1->viewerDistance()<pJob2->viewerDistance();
	}

	struct Renderjobqueue::Joblist
	{
		std::vector< Renderjob * > m_Jobs;
		std::vector< Renderjob * > m_ShadedOpaqueJobs,m_UnshadedOpaqueJobs,m_BlendedJobs;
	};

	Renderjobqueue::Renderjobqueue():m_pJoblist(new Joblist)
	{
	}

	Renderjobqueue::~Renderjobqueue()
	{
		clear();
		delete m_pJoblist;
	}

	void Renderjobqueue::clear()
	{
		m_pJoblist->m_Jobs.clear();
	}

	void Renderjobqueue::deleteJobs()
	{
		std::vector< Renderjob * >::iterator it=m_pJoblist->m_Jobs.begin();
		for (;it!=m_pJoblist->m_Jobs.end();++it) delete (*it);

		m_pJoblist->m_Jobs.clear();
	}

	bool Renderjobqueue::isValid() const
	{
		return m_pJoblist!=0;
	}

	void Renderjobqueue::addJob(Renderjob& rRenderjob)
	{
		m_pJoblist->m_Jobs.push_back(&rRenderjob);
	}

	ion_uint32 Renderjobqueue::numJobs() const
	{
		return (ion_uint32)(m_pJoblist->m_Jobs.size());
	}

	Renderjob* Renderjobqueue::job(const ion_uint32 jobnr)
	{
		return (jobnr<numJobs()) ? m_pJoblist->m_Jobs[jobnr] : 0;
	}

	void Renderjobqueue::appendQueue(const Renderjobqueue& queue)
	{
		std::vector< Renderjob * >::iterator it=queue.jobs().begin();
		for (;it!=queue.jobs().end();++it) {
			m_pJoblist->m_Jobs.push_back(*it);
		}
	}

	void Renderjobqueue::sortJobs(const math::Vector3f& viewerPosition)
	{
		m_pJoblist->m_ShadedOpaqueJobs.clear();
		m_pJoblist->m_UnshadedOpaqueJobs.clear();
		m_pJoblist->m_BlendedJobs.clear();

		std::set < video::Texture2D* > rendertargets;

		std::vector< Renderjob * >::iterator it=m_pJoblist->m_Jobs.begin();
		std::vector< Renderjob * >::iterator itend=it+numJobs();
		for (;it!=itend;++it) {
			Renderjob *pJob=(*it);

			if (pJob->shader()!=0) {
				if (pJob->shader()->blended()) {
					float distance=(pJob->worldtransform().position()-viewerPosition).lengthSquared();
					pJob->viewerDistance(distance);
					m_pJoblist->m_BlendedJobs.push_back(pJob);
				} else
					m_pJoblist->m_ShadedOpaqueJobs.push_back(pJob);
			} else m_pJoblist->m_UnshadedOpaqueJobs.push_back(pJob);

		}

		if (!m_pJoblist->m_ShadedOpaqueJobs.empty())
			std::sort(m_pJoblist->m_ShadedOpaqueJobs.begin(),m_pJoblist->m_ShadedOpaqueJobs.end(),sortShadedOpaqueJobs);

		if (!m_pJoblist->m_BlendedJobs.empty())
			std::sort(m_pJoblist->m_BlendedJobs.begin(),m_pJoblist->m_BlendedJobs.end(),sortBlendedJobs);
	}

	std::vector< Renderjob * >& Renderjobqueue::jobs() const
	{
		return m_pJoblist->m_Jobs;
	}

	std::vector< Renderjob * >& Renderjobqueue::shadedOpaqueJobs()
	{
		return m_pJoblist->m_ShadedOpaqueJobs;
	}

	std::vector< Renderjob * >& Renderjobqueue::unshadedOpaqueJobs()
	{
		return m_pJoblist->m_UnshadedOpaqueJobs;
	}

	std::vector< Renderjob * >& Renderjobqueue::blendedJobs()
	{
		return m_pJoblist->m_BlendedJobs;
	}

}
}
