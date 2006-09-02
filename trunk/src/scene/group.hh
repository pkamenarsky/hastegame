#ifndef ION_SCENE_GROUP_HH_INCLUDED
#define ION_SCENE_GROUP_HH_INCLUDED

#include "node.hh"

namespace ion {
namespace scene {

	class ION_API Group:public Node
	{
	public:
		Group(const base::String& identifier);
		~Group();

		virtual const char *type() const;
		virtual void respawn(base::Streamable& source);
		virtual void serialize(base::Streamable& dest);

		bool isValid() const;
	};

}
}

#endif
