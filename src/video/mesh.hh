#ifndef ION_VIDEO_MESH_HH_INCLUDED
#define ION_VIDEO_MESH_HH_INCLUDED

#include "../base/refcounted.hh"
#include "../base/string.hh"
#include "../math/aabbvolume.hh"
#include "../math/spherevolume.hh"
#include "../video/attributebuffer.hh"
#include "../video/attributetable.hh"
#include "../video/memindexstream.hh"
#include "../video/memvertexstream.hh"
#include "../video/indexstream.hh"
#include "../video/vertexstream.hh"

namespace ion {
namespace video {

	class ION_API Mesh:public base::RefCounted
	{
	public:
		const math::SphereVolume& boundingSphere() const;
		math::SphereVolume& boundingSphere();

		const math::AABBVolume& AABB() const;
		math::AABBVolume& AABB();

		virtual const Attributebuffer& attributebuffer() const=0;
		virtual const Attributetable& attributetable() const=0;
		virtual const Indexstream& indexstream() const=0;
		virtual const Vertexstream& vertexstream() const=0;

		virtual Attributebuffer& attributebuffer()=0;
		virtual Attributetable& attributetable()=0;
		virtual Indexstream& indexstream()=0;
		virtual Vertexstream& vertexstream()=0;

		virtual void reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat)=0;

		virtual void transferFrom(const Mesh& src,const bool initMesh=true);

		~Mesh();
	protected:
		Mesh(const base::String& identifier);

		math::AABBVolume m_AABBVolume;
		math::SphereVolume m_BoundingSphere;
	};



	class ION_API Memmesh:public Mesh
	{
	public:
		Memmesh(const base::String& identifier);

		Memmesh(const base::String& identifier,const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat);

		~Memmesh();

		void calcBoundingSphere();

		const Attributebuffer& attributebuffer() const;
		const Attributetable& attributetable() const;
		const MemIndexstream& memIndexstream() const;
		const MemVertexstream& memVertexstream() const;
		const Indexstream& indexstream() const;
		const Vertexstream& vertexstream() const;

		Attributebuffer& attributebuffer();
		Attributetable& attributetable();
		MemIndexstream& memIndexstream();
		MemVertexstream& memVertexstream();
		Indexstream& indexstream();
		Vertexstream& vertexstream();

		void reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat);

		bool isValid() const;

	protected:

		Attributebuffer m_Attributebuffer;
		Attributetable m_Attributetable;
		MemIndexstream *m_pMemIndexstream;
		MemVertexstream *m_pMemVertexstream;
	};

	class Videodevice;

	class ION_API Videomesh:public Mesh
	{
	public:
		Videomesh(const base::String& identifier,Videodevice& rVideodevice);
		Videomesh(const base::String& identifier,Videodevice& rVideodevice,
			const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat);
		~Videomesh();

		Videodevice& videodevice() const;

		const Attributebuffer& attributebuffer() const;
		const Attributetable& attributetable() const;
		const Vertexstream& vertexstream() const;
		const Indexstream& indexstream() const;

		Attributebuffer& attributebuffer();
		Attributetable& attributetable();
		Vertexstream& vertexstream();
		Indexstream& indexstream();

		void reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat);

		bool isValid() const;

	protected:

		Attributebuffer m_Attributebuffer;
		Attributetable m_Attributetable;
		Videodevice &m_rVideodevice;
		Indexstream *m_pIndexstream;
		Vertexstream *m_pVertexstream;
	};


	ION_API void calculateTangentspace(Mesh& rMesh,const ion_uint32 uvstage);

	ION_API void calculateTorus(Mesh &mesh,const float innerradius,const float outerradius,const unsigned long tesselation,
		const bool calculateTangentspace);
	ION_API void calculateCube(Mesh &mesh,const float xlength,const float ylength,const float zlength,const bool calculateTangentspace,const bool invertedFaces=false);
	ION_API void calculateSkybox(Mesh &mesh,const float xlength,const float ylength,const float zlength);
	ION_API void calculateSphere(Mesh &mesh,const float radius,const unsigned int tesselation);
	ION_API void calculateCylinderX(Mesh &mesh,const float radius,const float length,const unsigned int tesselation);
	ION_API void calculateCylinderY(Mesh &mesh,const float radius,const float length,const unsigned int tesselation);
	ION_API void calculateCylinderZ(Mesh &mesh,const float radius,const float length,const unsigned int tesselation);

}
}

#endif
