#ifndef ION_VIDEO_INDEXITERATOR_HH_INCLUDED
#define ION_VIDEO_INDEXITERATOR_HH_INCLUDED

#include "../base/iontypes.hh"
#include "../base/dll.hh"
#include "indexformat.hh"

namespace ion {
namespace video {

class Indexstream;

class ION_API Indexiterator {
friend class Indexstream;
public:
	Indexiterator(Indexstream& rIndexstream);
	Indexiterator(Indexstream& rIndexstream,const ion_uint32 position);
	~Indexiterator();

	inline ion_uint32 positionInStream() const { return m_CurrentPosition; }
	void positionInStream(const unsigned int index);

	friend bool operator ==(const Indexiterator &a,const Indexiterator &b) { return a.positionInStream()==b.positionInStream(); }
	friend bool operator !=(const Indexiterator &a,const Indexiterator &b) { return a.positionInStream()!=b.positionInStream(); }

	Indexiterator& operator ++();
	Indexiterator& operator ++(const int);
	Indexiterator& operator --();
	Indexiterator& operator --(const int);
	Indexiterator& operator +=(const int offset);
	Indexiterator& operator -=(const int offset);

	inline Indexiterator& operator =(const ion_uint32 newindex) { index(newindex); return *this; }
	inline operator ion_uint32() const { return index(); }

	void index(const ion_uint32 newindex);
	ion_uint32 index() const;
protected:

	mutable bool m_CurrentindexValid;
	mutable ion_uint32 m_CurrentIndex;
	ion_uint32 m_CurrentPosition;
	
	Indexstream &m_rIndexstream;
};

}
}

#endif
