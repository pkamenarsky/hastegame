#ifndef ION_SCENE_NODELIST_HH_INCLUDED
#define ION_SCENE_NODELIST_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace scene {

	class Node;

	class ION_API Nodelist:public base::BaseMemObj
	{
	public:
		Nodelist();
		~Nodelist();

		void addNode(Node &rNode);
		void removeNode(Node &rNode);
		ion_uint32 numNodes() const;
		Node* node(const ion_uint32 index);
		const Node* node(const ion_uint32 index) const;

		bool isValid() const;

	protected:
		bool m_ReferenceCounting;

	private:
		struct Nodearray;
		Nodearray *m_pNodearray;
	};

}
}


#endif
