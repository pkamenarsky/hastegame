#ifndef ION_SCENE_RENDERJOBLIST_HH_INCLUDED
#define ION_SCENE_RENDERJOBLIST_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace scene {

	class Renderjob;

	class ION_API Renderjoblist:public base::BaseMemObj
	{
	public:
		Renderjoblist();
		~Renderjoblist();

		void addRenderjob(Renderjob &rRenderjob);
		void removeRenderjob(Renderjob &rRenderjob);
		ion_uint32 numRenderjobs() const;
		void clear();
		Renderjob* renderjob(const ion_uint32 index);
		const Renderjob* renderjob(const ion_uint32 index) const;

		bool isValid() const;

	private:
		struct Renderjobarray;
		Renderjobarray *m_pRenderjobarray;
	};

}
}


#endif
