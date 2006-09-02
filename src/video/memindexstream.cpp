#include "indexformat.hh"
#include "memindexstream.hh"

namespace ion {
namespace video {

	MemIndexstream::MemIndexstream(const ion_uint32 size):Indexstream(Indexformat_32bit,size)
	{
		m_Indices.resize(sizeof(ion_uint32)*size);
		memset(m_Indices.data(),0,m_Indices.size());
	}

	bool MemIndexstream::isValid() const
	{
		return true;
	}

	bool MemIndexstream::isDataOK() const
	{
		return true;
	}

}
}
