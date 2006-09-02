#include <algorithm>
#include <vector>

#include "../base/iontypes.hh"
#include "indexlist.hh"

namespace ion {
namespace scene {

	Indexlist::Indexlist()
	{
		m_pIndices=new std::vector <ion_uint32>;
	}

	Indexlist::~Indexlist()
	{
		delete m_pIndices;
	}

	ion_uint32 Indexlist::addIndex(const ion_uint32 index)
	{
		m_pIndices->push_back(index);
		return (ion_uint32)(m_pIndices->size()-1);
	}

	ion_uint32 Indexlist::numIndices() const
	{
		return (ion_uint32)(m_pIndices->size());
	}

	void Indexlist::clear()
	{
		m_pIndices->clear();
	}

	ion_uint32 Indexlist::index(const ion_uint32 indexNr) const
	{
		return (ion_uint32)((*m_pIndices)[indexNr]);
	}

	bool Indexlist::isValid() const
	{
		return m_pIndices!=0;
	}

}
}
