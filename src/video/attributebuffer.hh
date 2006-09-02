#ifndef ION_VIDEO_ATTRIBUTESTREAM_HH_INCLUDED
#define ION_VIDEO_ATTRIBUTESTREAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/iontypes.hh"

namespace ion {
namespace video {

	class Indexstream;

	//! Buffer holding attributes for each face of a geometry.
	/**
	* A geometry (for example, a mesh) consists of vertices and faces. This buffer stores one attribute for
	* each face. The attribute is a 32-bit unsigned integer and is usually applied for material indexing purposes.
	* For example, mesh X has 300 triangles (which are its faces; in fact, on most occasions faces are triangles
	* in ion), triangles 0-29 use material #1, 30-284 material #2, 285 #1, 286-300 #3. This buffer then stores
	* "1" as attribute for triangles 0-29, "2" for 30-284, "1" for 285, "3" for 286-300.
	* It is wise to group together faces with the same attribute (this reduces the amount of API calls,
	* since after grouping only one call per material has to be done). This is done by calling reorganize().
	* The API calls aren't performed with the buffer directly, though. Rather, the Attributetable class is used
	* (which is constructed with an Attributebuffer instance).
	*/
	class ION_API Attributebuffer:public base::Datablock
	{
	public:
		//! Resizes the buffer.
		/**
		* @param numAttributes Amount of attributes (should equal the amount of faces used).
		*/
		inline void attributeAmount(const ion_uint32 numAttributes) { resize(numAttributes*sizeof(ion_uint32)); }
		//! Returns the amount of attributes that fit in the buffer.
		inline ion_uint32 attributeAmount() const { return size()/sizeof(ion_uint32); }
		
		//! Returns the attribute with the given index.
		/**
		* @param index Zero-based index used for retrieving the attribute.
		*/
		inline ion_uint32 operator [](const ion_uint32 index) const { return (reinterpret_cast<const ion_uint32*>(data()))[index]; }
		//! Returns a reference to the attribute with the given index.
		/**
		* @param index Zero-based index used for retrieving the reference.
		* @note Use this one for setting an attribute, for example: buffer[index]=attribute;
		*/
		inline ion_uint32& operator [](const ion_uint32 index) { return (reinterpret_cast<ion_uint32*>(data()))[index]; }

		//! Reorganizes the attributes so that faces with the same attribute are grouped together.
		/**
		* This is very useful for optimizing render performance. Example:
		* Attributes 0,1,1,2,1,2,0. This order implies five API drawing calls.
		* After regrouping, the order goes: 0,0,1,1,1,2,2 , reducing it to three calls.
		* If a pointer to an indexstream is passed, the corresponding indices in the stream will be
		* reorganized too, so that the indices in the stream match the attributes in this buffer.
		* @note The stream has to be writable for this! A MemIndexstream suits perfectly.
		* @param pIndexstream Optional indexstream to reorganize.
		*/
		void reorganize(Indexstream* pIndexstream=0);
	private:
		void mergesort(Attributebuffer& array,Attributebuffer& permutation,int lo,int hi);
	};

}
}

#endif
