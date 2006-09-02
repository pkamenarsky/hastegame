#ifndef ION_SCENE_RENDERABLELIST_HH_INCLUDED
#define ION_SCENE_RENDERABLELIST_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace scene {

	class Renderable;

	class ION_API Renderablelist:public base::BaseMemObj
	{
	public:
		Renderablelist();
		~Renderablelist();

		void addRenderable(Renderable &rRenderable);
		void removeRenderable(Renderable &rRenderable);
		ion_uint32 numRenderables() const;
		void clear();
		Renderable* renderable(const ion_uint32 index);
		const Renderable* renderable(const ion_uint32 index) const;

		bool isValid() const;

	protected:
		bool m_ReferenceCounting;

	private:
		struct Renderablearray;
		Renderablearray *m_pRenderablearray;
	};

}
}


#endif
