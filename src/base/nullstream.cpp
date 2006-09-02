#include "nullstream.hh"
#include <string>

const std::string& ion::base::Nullstream::name() const
{
	static std::string nullstreamname("Nullstream");
	return nullstreamname;
}
