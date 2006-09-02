#ifndef ION_SCENE_INDEXLIST_HH_INCLUDED
#define ION_SCENE_INDEXLIST_HH_INCLUDED

#include <vector>

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace scene {

	class ION_API Indexlist:public base::BaseMemObj
	{
	public:
		Indexlist();
		~Indexlist();

		ion_uint32 addIndex(const ion_uint32 index);
		ion_uint32 numIndices() const;
		void clear();
		ion_uint32 index(const ion_uint32 indexNr) const;

		bool isValid() const;

	private:
		std::vector <ion_uint32> *m_pIndices;
	};

}
}


#endif
