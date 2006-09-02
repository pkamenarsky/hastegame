#include "group.hh"

namespace ion {
namespace scene {

	Group::Group(const base::String& identifier):Node(identifier)
	{
	}

	Group::~Group()
	{
	}

	const char* Group::type() const
	{
		return "Group";
	}

	void Group::respawn(base::Streamable& source)
	{
	}

	void Group::serialize(base::Streamable& dest)
	{
	}

	bool Group::isValid() const
	{
		return true;
	}

}
}
