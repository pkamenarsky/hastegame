#ifndef ION_VIDEO_INDEXSTREAM_HH_INCLUDED
#define ION_VIDEO_INDEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "geometrystream.hh"
#include "indexiterator.hh"

namespace ion {
namespace video {

class ION_API Indexstream:public Geometrystream
{
friend class Indexiterator;
public:
	virtual ~Indexstream();
	inline Indexformat indexformat() const { return m_Format; }

	inline const Indexiterator& begin() const { return *m_pBegin; }
	inline const Indexiterator& end() const { return *m_pEnd; }

	inline void transferFrom(const Indexstream& from) { transferFrom(from,from.capacity()); }
	void transferFrom(const Indexstream& from,const ion_uint32 numIndices);

	virtual ion_uint32 index(const ion_uint32 position)=0;
	virtual void index(const ion_uint32 position,const ion_uint32 newindex)=0;
protected:
	Indexstream(const Indexformat format,const ion_uint32 _capacity);

	Indexiterator *m_pBegin,*m_pEnd;
	Indexformat m_Format;
};

}
}

#endif
