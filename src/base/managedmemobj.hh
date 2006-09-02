#ifndef ION_MANAGED_MEM_OBJECT_HH_INCLUDED
#define ION_MANAGED_MEM_OBJECT_HH_INCLUDED

#include <cstdlib>
#include "dll.hh"

namespace ion {
namespace base {

	class String;

	/**
	Similar to BaseMemObj, except that this class also implements a garbage collector.
	Not in the Java style, though; that is, not deallocated ManagedMemObjs will
	be automatically deallocated when calling endInstanceLogging(), thus preventing
	memory leaks. Client code also gets easier and shorter, since for simple code
	no manual delete calls need to be performed. It is still recommended to delete
	unused objects, however, to prevent excessive RAM usage.

	startInstanceLogging() starts the ManagedMemObj instance logging.
	Any allocations of a ManagedMemObj using new or new[] will be logged.
	When calling endInstanceLogging(), the logging is stopped, and any
	allocated ManagedMemObj will be deallocated.
	dumpInstanceLoggingStats() outputs the current logging list to stdout and
	is intended as a help for debugging.

	The client code does not have to call startInstanceLogging() and
	endInstanceLogging(), as these are automatically called in the init functions
	(initSystem() and shutdownSystem() in init.h).

	Update: the widgets now delete their children. This should further help memory
	management, since it doesnt make sense to have children objects floating in
	nirvana if their parent was just destroyed. ManagedMemObj has a detection
	mechanism which finds out if the object was allocated on the heap, and deletes
	if this turns out to have happened; otherwise, delete and delete[] do nothing.
	*/
	class ION_API ManagedMemObj {
	public:
		virtual ~ManagedMemObj();
	
		//! Overloaded new operator for memory management.
		void * operator new(size_t size);
		//! Overloaded delete operator for memory management.
		void operator delete(void *mem);

		//! Overloaded new[] operator for memory management.
		void * operator new[](size_t size);
		//! Overloaded delete[] operator for memory management.
		void operator delete[](void *mem);
	
	
		//! The object's identifier.
		/**
		* Each object has an identifier that ideally should be unique.
		* The type, format, meaning of the identifier is not defined and can be chosen freely.
		* This is mainly used for serialization and debugging purposes.
		* @return the object identifier
		*/
		const String& objIdentifier() const;
	
		void operator =(const ManagedMemObj& src);
	
	
		// Getter for sanity check
		/**
		* This is intended for implementation in derived classes. It allows
		* a sanity check whether the object's state is OK or invalid.
		* For example, when a component could not be initialized, the object
		* is in an invalid state, and this returns false.
		* @return true if the object is valid, false otherwise.
		*/	
		virtual bool isValid() const=0;
	
	protected:
	
		// Each object must have an identifier; useful for leak detection and removal
		ManagedMemObj(const String& objectidentifier);
		ManagedMemObj(const ManagedMemObj& src);
	
		String *m_ObjIdentifier;
	};

	void ION_API startInstanceLogging();
	void ION_API endInstanceLogging();
	void ION_API dumpInstanceLoggingStats();

}
}

#endif
