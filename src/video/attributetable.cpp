#include <map>
#include <vector>

#include "../base/log.hh"
#include "attributebuffer.hh"
#include "attributetable.hh"

namespace ion {
namespace video {



	Attributearrayentry::Attributearrayentry():m_Startface(0),m_NumFaces(0)
	{
	}

	Attributearrayentry::Attributearrayentry(const Attributearrayentry& entry)
	{
		m_Startface=entry.startface();
		m_NumFaces=entry.numFaces();
	}

	Attributearrayentry::~Attributearrayentry()
	{
	}

	Attributearrayentry& Attributearrayentry::operator = (const Attributearrayentry& entry)
	{
		m_Startface=entry.startface();
		m_NumFaces=entry.numFaces();

		return *this;
	}

	ion_uint32 Attributearrayentry::startface() const
	{
		return m_Startface;
	}

	ion_uint32 Attributearrayentry::numFaces() const
	{
		return m_NumFaces;
	}

	void Attributearrayentry::startface(const ion_uint32 face)
	{
		m_Startface=face;
	}

	void Attributearrayentry::numFaces(const ion_uint32 numFaces)
	{
		m_NumFaces=numFaces;
	}




	struct Attributearray::Array {
		std::vector < Attributearrayentry > m_Array;
	};

	Attributearray::Attributearray():m_pArray(new Array),m_Faceattribute(0)
	{
	}

	Attributearray::Attributearray(const Attributearray& array):m_pArray(new Array),m_Faceattribute(0)
	{
		clone(array);
	}

	Attributearray::~Attributearray()
	{
		delete m_pArray;
	}

	Attributearray& Attributearray::operator = (const Attributearray& array)
	{
		clone(array);
		return *this;
	}

	void Attributearray::clone(const Attributearray& array)
	{
		m_pArray->m_Array.clear();

		for (ion_uint32 i=0;i<numEntries();++i) {
			m_pArray->m_Array.push_back(*(array.entry(i)));
		}

		m_Faceattribute=array.faceattribute();
	}

	void Attributearray::faceattribute(const ion_uint32 attribute)
	{
		m_Faceattribute=attribute;
	}

	ion_uint32 Attributearray::faceattribute() const
	{
		return m_Faceattribute;
	}

	ion_uint32 Attributearray::numEntries() const
	{
		return (ion_uint32)(m_pArray->m_Array.size());
	}

	void Attributearray::addEntry(const Attributearrayentry& entry)
	{
		m_pArray->m_Array.push_back(entry);
	}

	const Attributearrayentry* Attributearray::entry(const ion_uint32 index) const
	{
		return (index<numEntries()) ? &(m_pArray->m_Array[index]) : 0;
	}






	Attributetable::Attributetable():m_pArrays(0),m_NumArrays(0)
	{
	}

	Attributetable::Attributetable(const Attributetable& table):m_pArrays(0),m_NumArrays(0)
	{
		clone(table);
	}

	Attributetable::Attributetable(const Attributebuffer& buffer):m_pArrays(0),m_NumArrays(0)
	{
		recalculate(buffer);
	}

	Attributetable::~Attributetable()
	{
		if (m_pArrays) delete [] m_pArrays;
	}

	void Attributetable::clone(const Attributetable& table)
	{
		if (m_pArrays) delete [] m_pArrays;
		m_NumArrays=table.numArrays();
		m_pArrays=new Attributearray[m_NumArrays];
		for (ion_uint32 i=0;i<m_NumArrays;++i) {
			m_pArrays[i]=*(table.array(i));
		}
	}

	void Attributetable::recalculate(const Attributebuffer& buffer)
	{
		std::map < ion_uint32, std::vector < Attributearrayentry > > arrays;

		Attributearrayentry entry;
		ion_uint32 attrib=buffer[0];
		ion_uint32 face=0;
		entry.startface(0);

		arrays[attrib]=std::vector < Attributearrayentry >();

		for (;face<buffer.attributeAmount();++face) {
			if (attrib!=buffer[face]) {
				entry.numFaces(face-entry.startface());

				if (arrays.find(attrib)==arrays.end())
					arrays[attrib]=std::vector < Attributearrayentry >();
				arrays[attrib].push_back(entry);

				entry.startface(face);
				entry.numFaces(0);
				attrib=buffer[face];
			}
		}

		entry.numFaces(face-entry.startface());
		if (arrays.find(attrib)==arrays.end())
			arrays[attrib]=std::vector < Attributearrayentry >();
		arrays[attrib].push_back(entry);

		if (m_pArrays) delete [] m_pArrays;
		m_NumArrays=(ion_uint32)arrays.size();
		m_pArrays=new Attributearray[m_NumArrays];

		Attributearray *pArray=&(m_pArrays[0]);
		std::map < ion_uint32, std::vector < Attributearrayentry > >::iterator ita=arrays.begin();
		for (;ita!=arrays.end();++ita,++pArray) {
			pArray->faceattribute((*ita).first);

			std::vector < Attributearrayentry >& srcarray=(*ita).second;

			std::vector < Attributearrayentry >::iterator ite=srcarray.begin();
			for (;ite!=srcarray.end();++ite) pArray->addEntry(*ite);
		}

	}

	void Attributetable::dumpTable()
	{
		/*std::vector <Attributetableentry>::const_iterator itentry=m_pEntries->begin();
		for (;itentry!=m_pEntries->end();++itentry) {
			const Attributetableentry& entry=*itentry;
			ion::base::logstream() << "[ " << entry.startface() << " " << entry.numFaces() << " " << entry.faceattribute() << " ]\n";
		}*/
	}

	const Attributearray* Attributetable::array(const ion_uint32 index) const
	{
		return (m_pArrays) ? ((index<m_NumArrays) ? &(m_pArrays[index]) : 0) : 0;
	}

	ion_uint32 Attributetable::numArrays() const
	{
		return m_NumArrays;
	}

}
}
