#include <algorithm>
#include <vector>

#include "node.hh"
#include "nodelist.hh"

namespace ion {
namespace scene {

	struct Nodelist::Nodearray
	{
		std::vector< Node* > m_Nodes;

		std::vector< Node* >::iterator find(Node* pNode)
		{
			return std::find(m_Nodes.begin(),m_Nodes.end(),pNode);
		}

		bool contains(Node* pNode)
		{
			return (find(pNode)!=m_Nodes.end());
		}
	};
	
	
	

	Nodelist::Nodelist():m_pNodearray(new Nodearray)
	{
	}

	Nodelist::~Nodelist()
	{
		delete m_pNodearray;
	}

	void Nodelist::addNode(Node &rNode)
	{
		if (m_pNodearray->contains(&rNode)) return;

		if (m_ReferenceCounting) rNode.addRef();
		m_pNodearray->m_Nodes.push_back(&rNode);
	}

	void Nodelist::removeNode(Node &rNode)
	{
		std::vector< Node* >::iterator it=m_pNodearray->find(&rNode);
		if (it!=m_pNodearray->m_Nodes.end()) {
			m_pNodearray->m_Nodes.erase(it);
			if (m_ReferenceCounting) rNode.release();
		}
	}

	ion_uint32 Nodelist::numNodes() const
	{
		return (ion_uint32)(m_pNodearray->m_Nodes.size());
	}

	Node* Nodelist::node(const ion_uint32 index)
	{
		return (index<numNodes()) ? m_pNodearray->m_Nodes[index] : 0;
	}

	const Node* Nodelist::node(const ion_uint32 index) const
	{
		return (index<numNodes()) ? m_pNodearray->m_Nodes[index] : 0;
	}

	bool Nodelist::isValid() const
	{
		return (m_pNodearray!=0);
	}

}
}
