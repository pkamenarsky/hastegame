#include <string.h>
#include "physfsfile.hh"

extern "C" {
#include "../extern/physfs/globbing.h"
#include "ignorecase.h"
}

namespace ion {
namespace base {

	PhysFSFile::PhysFSFile():m_pFile(0)
	{
	}

	PhysFSFile::PhysFSFile(const String& filename,const Openmode openmode):m_pFile(0)
	{
		open(filename,openmode);
	}

	PhysFSFile::~PhysFSFile()
	{
		close();
	}

	void PhysFSFile::open(const String& filename,const Openmode openmode)
	{
		if (m_pFile!=0) return;

		m_Name=filename;

		char *cstr=new char[filename.length()+1];
		cstr[filename.length()]=0;
		strncpy(cstr,filename.cstr(),filename.length());
		PHYSFSEXT_locateCorrectCase(cstr);

		switch (openmode)
		{
			case OpenRead:m_pFile=PHYSFS_openRead(cstr); break;
			case OpenWrite:m_pFile=PHYSFS_openWrite(cstr); break;
			case OpenAppend:m_pFile=PHYSFS_openAppend(cstr); break;
		};

		delete [] cstr;
	}

	void PhysFSFile::close()
	{
		if (m_pFile==0) return;
		PHYSFS_close(m_pFile);
		m_pFile=0;
	}

	unsigned long PhysFSFile::size() const
	{
		return (m_pFile==0) ? 0 : (unsigned long)PHYSFS_fileLength(m_pFile);
	}

	unsigned long PhysFSFile::tell() const
	{
		return (m_pFile==0) ? 0 : (unsigned long)PHYSFS_tell(m_pFile);
	}

	void PhysFSFile::seek(const long Pos,const Seekmode seekmode)
	{
		if (m_pFile==0) return;

		PHYSFS_uint64 p=Pos;

		switch (seekmode)
		{
			case SeekRelative:p+=PHYSFS_tell(m_pFile); break;
			case SeekFromEnd:p=PHYSFS_fileLength(m_pFile)-p; break;
			default:break;
		};

		PHYSFS_seek(m_pFile,p);
	}

	bool PhysFSFile::streamIsOk() const
	{
		return (m_pFile!=0);
	}

	const std::string& PhysFSFile::name() const
	{
		return m_Name;
	}

	unsigned long PhysFSFile::write(const void *Src,const unsigned long wsize)
	{
		if (m_pFile==0) return 0;
		return (unsigned long)PHYSFS_write(m_pFile,Src,1,wsize);
	}

	unsigned long PhysFSFile::read(void *Dest,const unsigned long rsize)
	{
		if (m_pFile==0) return 0;
		return (unsigned long)PHYSFS_read(m_pFile,Dest,1,rsize);
	}

	bool PhysFSFile::eof() const
	{
		return (m_pFile==0) ? true : (PHYSFS_eof(m_pFile)!=0);
	}





	void findPhysfsFiles(Stringlist& result,const String& dir,const String& wildcard,const bool caseSensitive)
	{
		char **cresults=PHYSFSEXT_enumerateFilesWildcard(dir.cstr(),wildcard.cstr(),caseSensitive ? 1 : 0);
		result.emptyList();
		if (cresults) {
			for (char **filename=cresults;(*filename)!=0;++filename) {
				result.addString(String(*filename));
			}
			PHYSFS_freeList(cresults);
		}
	}


}
}
