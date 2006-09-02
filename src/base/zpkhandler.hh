#ifndef ION_BASE_ZPKHANDLER_HH_INCLUDED
#define ION_BASE_ZPKHANDLER_HH_INCLUDED

#include "dll.hh"
#include "pkdatablock.hh"

namespace ion {
namespace base {

	//! PackHandler for zlib-compressed data.

	class ION_API ZPackHandler:public PackHandler
	{
	protected:
		bool m_ZlibHeader;
		unsigned long m_CompressedSize,m_UncompressedSize;
	public:
		ZPackHandler():m_ZlibHeader(true),m_CompressedSize(0),m_UncompressedSize(0) {}
		
		inline void zlibHeader(bool state) { m_ZlibHeader=state; }
		inline bool zlibHeader() const { return m_ZlibHeader; }
		inline void compressedSize(unsigned long size) { m_CompressedSize=size; }
		inline unsigned long compressedSize() const { return m_CompressedSize; }
		inline void uncompressedSize(unsigned long size) { m_UncompressedSize=size; }
		inline unsigned long uncompressedSize() const { return m_UncompressedSize; }

		void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest);
		void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest);
	};

}
}

#endif
