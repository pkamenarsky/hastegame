#ifndef ION_BASE_STDOUTSTREAM_HH_INCLUDED
#define ION_BASE_STDOUTSTREAM_HH_INCLUDED

#include "dll.hh"
#include "localstreamable.hh"

namespace ion {
namespace base {

	//! Streamable sending output to stdout.
	/**
	* Characteristics:
	* - All write calls() send the data to stdout
	* - read() does nothing
	* - eof() always returns false
	* - name() returns "Stdoutstream"
	*
	* Usually, one does not create an instance of this; instead, it is common to use the static
	* instance returned by stream().
	*/
	class ION_API Stdoutstream:public Streamable
	{
	public:
		Stdoutstream();
		~Stdoutstream();

		const std::string& name() const;

		unsigned long write(const void *Src,const unsigned long wsize);
		unsigned long read(void *Dest,const unsigned long rsize);

		bool eof() const;

		//! Returns a reference to a static instance of Stdoutstream.
		static Stdoutstream& stream();
	};

}
}

#endif
