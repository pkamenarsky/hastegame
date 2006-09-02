#ifndef ION_VIDEO_ATTRIBUTETABLE_HH_INCLUDED
#define ION_VIDEO_ATTRIBUTETABLE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"

namespace ion {
namespace video {

	class Attributebuffer;

	class ION_API Attributearrayentry
	{
	public:
		Attributearrayentry();
		Attributearrayentry(const Attributearrayentry& entry);
		~Attributearrayentry();

		Attributearrayentry& operator = (const Attributearrayentry& entry);

		ion_uint32 startface() const;
		ion_uint32 numFaces() const;

		void startface(const ion_uint32 face);
		void numFaces(const ion_uint32 numFaces);

	protected:
		ion_uint32 m_Startface,m_NumFaces;
	};



	class ION_API Attributearray
	{
	public:
		Attributearray();
		Attributearray(const Attributearray& array);
		~Attributearray();

		Attributearray& operator = (const Attributearray& array);
		void clone(const Attributearray& array);

		void faceattribute(const ion_uint32 attribute);
		ion_uint32 faceattribute() const;

		ion_uint32 numEntries() const;
		void addEntry(const Attributearrayentry& entry);
		const Attributearrayentry* entry(const ion_uint32 index) const;
	protected:
		struct Array;
		Array *m_pArray;

		ion_uint32 m_Faceattribute;
	};



	class ION_API Attributetable
	{
	public:
		Attributetable();
		Attributetable(const Attributetable& table);
		Attributetable(const Attributebuffer& buffer);
		~Attributetable();

		void clone(const Attributetable& table);

		void recalculate(const Attributebuffer& buffer);

		void dumpTable();

		ion_uint32 numArrays() const;
		const Attributearray* array(const ion_uint32 index) const;

	protected:
		Attributearray* m_pArrays;
		ion_uint32 m_NumArrays;
	};

}
}

#endif
