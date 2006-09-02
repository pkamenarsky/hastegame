#ifndef ION_BASE_PHYSFSFILE_HH_INCLUDED
#define ION_BASE_PHYSFSFILE_HH_INCLUDED

#include "localstreamable.hh"
#include "string.hh"
#include "stringlist.hh"
#include "../extern/physfs/physfs.h"

namespace ion {
namespace base {

	class ION_API PhysFSFile:public LocalStreamable
	{
	public:
		enum Openmode
		{
			OpenRead,
			OpenWrite,
			OpenAppend
		};

		PhysFSFile();
		PhysFSFile(const String& filename,const Openmode openmode);
		~PhysFSFile();

		void open(const String& filename,const Openmode openmode);
		void close();

		unsigned long size() const;
		unsigned long tell() const;
		void seek(const long Pos,const Seekmode seekmode);

		bool streamIsOk() const;

		const std::string& name() const;

		unsigned long write(const void *Src,const unsigned long wsize);
		unsigned long read(void *Dest,const unsigned long rsize);

		bool eof() const;

	protected:
		String m_Name;
		PHYSFS_file* m_pFile;
	};

	ION_API void findPhysfsFiles(Stringlist& result,const String& dir,const String& wildcard,const bool caseSensitive);

}
}

#endif
