#include "../base/streamable.hh"
#include "vertexformat.hh"
#include "memvertexstream.hh"

namespace ion {
namespace video {

	MemVertexstream::MemVertexstream(const Vertexformat& format,
		const ion_uint32 _capacity,Vertexsource *pSource):
	Vertexstream(format,_capacity,pSource)
	{
		m_Vertices.resize(format.stride()*_capacity);
		memset(m_Vertices.data(),0,m_Vertices.size());

		m_Pointers.m_CurrentVtx=0;
		m_Pointers.m_pp1DTexcoords=m_pp1DTexcoords;
		m_Pointers.m_pp2DTexcoords=m_pp2DTexcoords;
		m_Pointers.m_pp3DTexcoords=m_pp3DTexcoords;
		calculatePointers(m_Pointers);
	}

	void MemVertexstream::calculatePointers(VertexiteratorPointers& vitp)
	{
		if (vitp.m_CurrentVtx>=capacity()) return;

		ion_uint8* pPtr=((ion_uint8*)(m_Vertices.data()))+m_Format.stride()*vitp.m_CurrentVtx;
		int numtexcoords[3]={0,0,0};
		for (unsigned long entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			VertexFormatEntry entry=m_Format.entry(entrynr);
			switch (entry) {
				case VertexFormatEntry_Position:vitp.m_pPosition=(math::Vector3f*)pPtr; break;
				case VertexFormatEntry_Normal:vitp.m_pNormal=(math::Vector3f*)pPtr; break;
				case VertexFormatEntry_Texcoord1D:vitp.m_pp1DTexcoords[numtexcoords[0]++]=(float*)pPtr; break;
				case VertexFormatEntry_Texcoord2D:vitp.m_pp2DTexcoords[numtexcoords[1]++]=(math::Vector2f*)pPtr; break;
				case VertexFormatEntry_Texcoord3D:vitp.m_pp3DTexcoords[numtexcoords[2]++]=(math::Vector3f*)pPtr; break;
				case VertexFormatEntry_Diffuse:vitp.m_pDiffuseColor=(ion_uint32*)pPtr; break;
				case VertexFormatEntry_Specular:vitp.m_pSpecularColor=(ion_uint32*)pPtr; break;
				case VertexFormatEntry_Boneweight:break; // TODO: solve this
			}
			pPtr+=vertexFormatEntrySizeLookup(entry);
		}
	}

	void MemVertexstream::calcMinmax(math::Vector3f& minxyz,math::Vector3f& maxxyz)
	{
		Vertexiterator viterator(*this);

		minxyz=maxxyz=(viterator.position());

		for (ion_uint32 v=0;v<capacity();++v) {
			const math::Vector3f &vec=viterator.position();

			if (minxyz.x()>vec.x()) minxyz.x()=vec.x();
			if (minxyz.y()>vec.y()) minxyz.y()=vec.y();
			if (minxyz.z()>vec.z()) minxyz.z()=vec.z();

			if (maxxyz.x()<vec.x()) maxxyz.x()=vec.x();
			if (maxxyz.x()<vec.y()) maxxyz.y()=vec.y();
			if (maxxyz.x()<vec.z()) maxxyz.z()=vec.z();
		
			++viterator;
		}
	}

	bool MemVertexstream::isValid() const
	{
		return true;
	}

	bool MemVertexstream::isDataOK() const
	{
		return true;
	}

	const math::Vector3f& MemVertexstream::position(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	const math::Vector3f& MemVertexstream::normal(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	ion_uint32 MemVertexstream::diffuseColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	ion_uint32 MemVertexstream::specularColor(const ion_uint32 vtxindex) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		return *((ion_uint32*)pPtr);
	}

	float MemVertexstream::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((float*)pPtr);
	}

	const math::Vector2f& MemVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector2f*)pPtr);
	}

	const math::Vector3f& MemVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	void MemVertexstream::position(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void MemVertexstream::position(const ion_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newPosition.x();
		((math::Vector3f*)pPtr)->y()=newPosition.y();
		((math::Vector3f*)pPtr)->z()=newPosition.z();
	}

	void MemVertexstream::normal(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void MemVertexstream::normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newNormal.x();
		((math::Vector3f*)pPtr)->y()=newNormal.y();
		((math::Vector3f*)pPtr)->z()=newNormal.z();
	}

	void MemVertexstream::diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)r)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)b);
	}

	void MemVertexstream::diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=color;
	}

	void MemVertexstream::specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=(((ion_uint32)a)<<24)|(((ion_uint32)r)<<16)|(((ion_uint32)g)<<8)|((ion_uint32)b);
	}

	void MemVertexstream::specularColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ion_uint32*)pPtr)=color;
	}

	void MemVertexstream::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		*((float*)pPtr)=newTexcoord1D;
	}

	void MemVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
	}

	void MemVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord2D.x();
		((float*)pPtr)[1]=newTexcoord2D.y();
	}

	void MemVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
		((float*)pPtr)[2]=w;
	}

	void MemVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		ion_uint8 *pPtr=(ion_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord3D.x();
		((float*)pPtr)[1]=newTexcoord3D.y();
		((float*)pPtr)[2]=newTexcoord3D.z();
	}

}
}
