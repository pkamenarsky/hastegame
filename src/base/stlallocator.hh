#ifndef ION_STLALLOCATOR_HH_INCLUDED
#define ION_STLALLOCATOR_HH_INCLUDED

#include <cstdlib>
#include <climits>
#include <stddef.h>

namespace ion {
namespace base {

	namespace internalAllocation
	{
		void* allocMem(const size_t size);
		void freeMem(void* memblock);
	
		inline void destruct(char*) {}
		template <typename T> inline void destruct(T* t) { t->~T(); }
	}

	template <typename T> class ionSTLallocator;

	// specialize for void:
	template <> class ionSTLallocator<void>
	{
	public:
		typedef void* pointer;
		typedef const void* const_pointer;
		// reference to void members are impossible.
		typedef void value_type;
		template <class U> struct rebind { typedef ionSTLallocator<U> other; };
	};

	template <typename T> class ionSTLallocator {
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		template <class U> struct rebind { typedef ionSTLallocator<U> other; };
		
		ionSTLallocator(){}
		ionSTLallocator(const ionSTLallocator&) {}
		template <class U> ionSTLallocator(const ionSTLallocator<U>&) {}
		~ionSTLallocator() {}

		pointer address(reference x) const { return &x; }
		const_pointer address(const_reference x) const { return &x; }
		
		//for Dinkumware:
		char *_Charalloc(size_type n) {
			return static_cast<char*>(internalAllocation::allocMem(n));
		}
		// end Dinkumware

		pointer allocate(size_type n,typename ionSTLallocator<T>::const_pointer hint = 0) {
			return (pointer)internalAllocation::allocMem(n*sizeof(T));
		}
		void deallocate(pointer p, size_type n=0) { internalAllocation::freeMem(p); }
		void deallocate(void* p, size_type n=0) { internalAllocation::freeMem(p); }
		size_type max_size() const throw() { return UINT_MAX; }

		void construct(pointer p) {
			new (static_cast<void*>(p)) T();
		}

		void construct(pointer p, const T& val) {
			new (static_cast<void*>(p)) T(val);
		}
		
		void destroy(pointer p) { internalAllocation::destruct(p); }
	};
	
	template <class T1, class T2>
	bool operator==(const ionSTLallocator<T1>&, const ionSTLallocator<T2>&) throw() { return true; }

	template <class T1, class T2>
	bool operator!=(const ionSTLallocator<T1>&, const ionSTLallocator<T2>&) throw() { return false; }

// For VC6/STLPort 4-5-3 see /stl/_alloc.h, line 464
// "If custom allocators are being used without member template classes support :
// user (on purpose) is forced to define rebind/get operations !!!"
#ifdef _WIN32
#define SS_ALLOC_CDECL __cdecl
#else
#define SS_ALLOC_CDECL
#endif

namespace std{
template <class _Tp1, class _Tp2>
inline ionSTLallocator<_Tp2>& SS_ALLOC_CDECL
__stl_alloc_rebind(ionSTLallocator<_Tp1>& __a, const _Tp2*) 
{  
    return (ionSTLallocator<_Tp2>&)(__a); 
}


template <class _Tp1, class _Tp2>
inline ionSTLallocator<_Tp2> SS_ALLOC_CDECL
__stl_alloc_create(const ionSTLallocator<_Tp1>&, const _Tp2*) 
{ 
    return ionSTLallocator<_Tp2>(); 
}

} // namespace std
// end STLPort

	
}
}

#endif
