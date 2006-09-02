#include <algorithm>
#include <vector>

#include "renderjob.hh"
#include "renderjoblist.hh"

namespace ion {
namespace scene {

	struct Renderjoblist::Renderjobarray
	{
		std::vector< Renderjob* > m_Renderjobs;

		std::vector< Renderjob* >::iterator find(Renderjob* pRenderjob)
		{
			return std::find(m_Renderjobs.begin(),m_Renderjobs.end(),pRenderjob);
		}

		bool contains(Renderjob* pRenderjob)
		{
			return (find(pRenderjob)!=m_Renderjobs.end());
		}
	};
	
	
	

	Renderjoblist::Renderjoblist():m_pRenderjobarray(new Renderjobarray)
	{
		m_pRenderjobarray->m_Renderjobs.reserve(64);
	}

	Renderjoblist::~Renderjoblist()
	{
		delete m_pRenderjobarray;
	}

	void Renderjoblist::addRenderjob(Renderjob &rRenderjob)
	{
		// TODO: in debug build, contains() and push_back() are IMMENSE performance eaters! Fix this!
		//if (m_pRenderjobarray->contains(&rRenderjob)) return;

		m_pRenderjobarray->m_Renderjobs.push_back(&rRenderjob);
	}

	void Renderjoblist::removeRenderjob(Renderjob &rRenderjob)
	{
		std::vector< Renderjob* >::iterator it=m_pRenderjobarray->find(&rRenderjob);
		if (it!=m_pRenderjobarray->m_Renderjobs.end()) {
			m_pRenderjobarray->m_Renderjobs.erase(it);
		}
	}

	ion_uint32 Renderjoblist::numRenderjobs() const
	{
		return (ion_uint32)(m_pRenderjobarray->m_Renderjobs.size());
	}

	void Renderjoblist::clear()
	{
		//m_pRenderjobarray->m_Renderjobs.clear();
		m_pRenderjobarray->m_Renderjobs.resize(0);
	}

	Renderjob* Renderjoblist::renderjob(const ion_uint32 index)
	{
		return (index<numRenderjobs()) ? m_pRenderjobarray->m_Renderjobs[index] : 0;
	}

	const Renderjob* Renderjoblist::renderjob(const ion_uint32 index) const
	{
		return (index<numRenderjobs()) ? m_pRenderjobarray->m_Renderjobs[index] : 0;
	}

	bool Renderjoblist::isValid() const
	{
		return (m_pRenderjobarray!=0);
	}

}
}
