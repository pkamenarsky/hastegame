#include "stlallocator.hh"
#include <memory.h>

namespace ion {
namespace base {

	namespace internalAllocation
	{
		void* allocMem(const size_t size)
		{
			return malloc(size);
		}

		void freeMem(void* memblock)
		{
			free(memblock);
		}

	}

}
}
