#include "../base/iontypes.hh"
#include "indexiterator.hh"
#include "indexstream.hh"

namespace ion {
namespace video {

	Indexstream::Indexstream(const Indexformat format,const ion_uint32 _capacity):
	Geometrystream(_capacity),m_Format(format)
	{
		m_pBegin=new Indexiterator(*this,0);
		m_pEnd=new Indexiterator(*this,_capacity);
	}

	Indexstream::~Indexstream()
	{
		delete m_pEnd;
		delete m_pBegin;
	}

	void Indexstream::transferFrom(const Indexstream& from,const ion_uint32 numIndices)
	{
		Indexiterator ifrom((Indexstream&)from),ito(*this);

		for (ion_uint32 idx=0;idx<numIndices;++idx) {
			ito.index(ifrom.index());
			++ifrom; ++ito;
		}
	}

}
}
