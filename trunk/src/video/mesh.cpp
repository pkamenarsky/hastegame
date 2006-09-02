#include "../math/plane.hh"
#include "../math/vectors.hh"
#include "../video/mempool.hh"
#include "mesh.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	Mesh::Mesh(const base::String& identifier):RefCounted(identifier)
	{
	}

	Mesh::~Mesh()
	{
	}

	const math::SphereVolume& Mesh::boundingSphere() const
	{
		return m_BoundingSphere;
	}

	math::SphereVolume& Mesh::boundingSphere()
	{
		return m_BoundingSphere;
	}

	const math::AABBVolume& Mesh::AABB() const
	{
		return m_AABBVolume;
	}

	math::AABBVolume& Mesh::AABB()
	{
		return m_AABBVolume;
	}

	void Mesh::transferFrom(const Mesh& src,const bool initMesh)
	{
		if (initMesh)
			reinit(src.attributebuffer().attributeAmount(),src.vertexstream().capacity(),
				src.vertexstream().vertexformat(),src.indexstream().indexformat());

		vertexstream().map(Map_Writeonly);
		vertexstream().transferFrom(src.vertexstream());
		vertexstream().unmap();

		indexstream().map(Map_Writeonly);
		indexstream().transferFrom(src.indexstream());
		indexstream().unmap();

		m_BoundingSphere.center(src.boundingSphere().center());
		m_BoundingSphere.radius(src.boundingSphere().radius());

		m_AABBVolume.center(src.AABB().center());
		m_AABBVolume.extents(src.AABB().extents());

		attributebuffer().clone(src.attributebuffer());
		attributetable().recalculate(attributebuffer());
	}



	Memmesh::Memmesh(const base::String& identifier):Mesh(identifier),m_pMemIndexstream(0),m_pMemVertexstream(0)
	{
	}

	Memmesh::Memmesh(const base::String& identifier,const ion_uint32 numFaces,const ion_uint32 numVertices,
	const Vertexformat& vertexformat):Mesh(identifier),m_pMemIndexstream(0),m_pMemVertexstream(0)
	{
		reinit(numFaces,numVertices,vertexformat,Indexformat_32bit);
	}

	Memmesh::~Memmesh()
	{
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;
	}

	void Memmesh::calcBoundingSphere()
	{
		math::Vector3f min,max;

		for (ion_uint32 i=0;i<m_pMemVertexstream->capacity();++i) {
			if (i==0) {
				min=max=m_pMemVertexstream->position(i);
			} else {
				const math::Vector3f &v=m_pMemVertexstream->position(i);

				if (min.x()>v.x()) min.x()=v.x();
				if (min.y()>v.y()) min.y()=v.y();
				if (min.z()>v.z()) min.z()=v.z();

				if (max.x()<v.x()) max.x()=v.x();
				if (max.y()<v.y()) max.y()=v.y();
				if (max.z()<v.z()) max.z()=v.z();
			}
		}

		m_BoundingSphere.center((max-min)*0.5f);
		m_BoundingSphere.radius((max-min).length()*0.5f);
	}

	const Attributebuffer& Memmesh::attributebuffer() const
	{
		return m_Attributebuffer;
	}

	const Attributetable& Memmesh::attributetable() const
	{
		return m_Attributetable;
	}

	const MemIndexstream& Memmesh::memIndexstream() const
	{
		return *m_pMemIndexstream;
	}

	const MemVertexstream& Memmesh::memVertexstream() const
	{
		return *m_pMemVertexstream;
	}

	const Indexstream& Memmesh::indexstream() const
	{
		return *m_pMemIndexstream;
	}

	const Vertexstream& Memmesh::vertexstream() const
	{
		return *m_pMemVertexstream;
	}


	Attributebuffer& Memmesh::attributebuffer()
	{
		return m_Attributebuffer;
	}

	Attributetable& Memmesh::attributetable()
	{
		return m_Attributetable;
	}

	MemIndexstream& Memmesh::memIndexstream()
	{
		return *m_pMemIndexstream;
	}

	MemVertexstream& Memmesh::memVertexstream()
	{
		return *m_pMemVertexstream;
	}

	Indexstream& Memmesh::indexstream()
	{
		return *m_pMemIndexstream;
	}

	Vertexstream& Memmesh::vertexstream()
	{
		return *m_pMemVertexstream;
	}

	void Memmesh::reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat)
	{
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;


		if ((m_pMemVertexstream!=0) && (m_pMemIndexstream!=0)) {
			bool same=	(numFaces==m_Attributebuffer.attributeAmount())&&
						(vertexformat==m_pMemVertexstream->vertexformat());

			if (same) return;
		}

		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;

		m_pMemVertexstream=new MemVertexstream(vertexformat,numVertices);
		m_pMemIndexstream=new MemIndexstream(numFaces*3);

		m_Attributebuffer.attributeAmount(numFaces);
	}

	bool Memmesh::isValid() const
	{
		return ((m_pMemVertexstream!=0) && (m_pMemIndexstream!=0));
	}



	Videomesh::Videomesh(const base::String& identifier,Videodevice& rVideodevice):Mesh(identifier),
		m_rVideodevice(rVideodevice),m_pIndexstream(0),m_pVertexstream(0)
	{
	}

	Videomesh::Videomesh(const base::String& identifier,Videodevice& rVideodevice,
		const ion_uint32 numFaces,const ion_uint32 numVertices,const Vertexformat& vertexformat,const Indexformat indexformat):
		Mesh(identifier),m_rVideodevice(rVideodevice),m_pIndexstream(0),m_pVertexstream(0)
	{
		m_pVertexstream=rVideodevice.createVertexstream(
			numVertices,
			vertexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);

		m_pIndexstream=rVideodevice.createIndexstream(
			numFaces*3,
			indexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);

		m_Attributebuffer.attributeAmount(numFaces);
	}

	Videomesh::~Videomesh()
	{
		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;
	}

	Videodevice& Videomesh::videodevice() const
	{
		return m_rVideodevice;
	}

	const Attributebuffer& Videomesh::attributebuffer() const
	{
		return m_Attributebuffer;
	}

	const Attributetable& Videomesh::attributetable() const
	{
		return m_Attributetable;
	}

	const Vertexstream& Videomesh::vertexstream() const
	{
		return *m_pVertexstream;
	}

	const Indexstream& Videomesh::indexstream() const
	{
		return *m_pIndexstream;
	}

	Attributebuffer& Videomesh::attributebuffer()
	{
		return m_Attributebuffer;
	}

	Attributetable& Videomesh::attributetable()
	{
		return m_Attributetable;
	}

	Vertexstream& Videomesh::vertexstream()
	{
		return *m_pVertexstream;
	}

	Indexstream& Videomesh::indexstream()
	{
		return *m_pIndexstream;
	}

	void Videomesh::reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const Vertexformat& vertexformat,const Indexformat indexformat)
	{
		if ((m_pVertexstream!=0) && (m_pIndexstream!=0)) {
			bool same=	(numFaces==m_Attributebuffer.attributeAmount())&&
						(vertexformat==m_pVertexstream->vertexformat())&&
						(indexformat==m_pIndexstream->indexformat());

			if (same) return;
		}

		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;

		m_pVertexstream=m_rVideodevice.createVertexstream(
			numVertices,
			vertexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);

		m_pIndexstream=m_rVideodevice.createIndexstream(
			numFaces*3,
			indexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);

		m_Attributebuffer.attributeAmount(numFaces);
	}

	bool Videomesh::isValid() const
	{
		return (m_pIndexstream!=0) && (m_pVertexstream!=0) && m_rVideodevice.isValid();
	}











	//////////// UTILITY FUNCTIONS

	void calculateTangentspace(Mesh& rMesh,const ion_uint32 uvstage)
	{
		Vertexstream &vtxstream=rMesh.vertexstream();
		Indexstream &idxstream=rMesh.indexstream();

		ion_uint32 numfaces=idxstream.capacity()/3;

		ion_uint32 tangentstage=vtxstream.vertexformat().find(VertexFormatEntry_Texcoord3D,0,VertexFormatSemantic_Tangent,0);
		ion_uint32 binormalstage=vtxstream.vertexformat().find(VertexFormatEntry_Texcoord3D,0,VertexFormatSemantic_Binormal,0);

		if (!rMesh.vertexstream().vertexformat().contains(VertexFormatEntry_Texcoord2D,0,uvstage)) return;
		if ((tangentstage==0xffffffff) && (binormalstage==0xffffffff)) return; // Nothing to calculate

		math::Vector3f *pTangents=new math::Vector3f[vtxstream.capacity()];
		math::Vector3f *pBinormal=new math::Vector3f[vtxstream.capacity()];

		Indexiterator iiterator(idxstream);
		Vertexiterator viterator(vtxstream);

		for (ion_uint32 face=0;face<numfaces;++face) {
			ion_uint32 indices[3];
			ion_uint32 i;
			math::Vector3f pVecXYZ[3];
			math::Vector2f pVecUV[3];

			for (i=0;i<3;++i) {
				iiterator.positionInStream(face*3+i);
				indices[i]=iiterator.index();

				viterator.positionInStream(indices[i]);
				pVecXYZ[i]=viterator.position();
				pVecUV[i]=viterator.texcoord2D(uvstage);
			}

			math::Plane planes[3];

			for (i=0;i<3;++i) {
				math::Vector3f nvec[2];
				const math::Vector3f &v0=pVecXYZ[0];
				const math::Vector3f &v1=pVecXYZ[1];
				const math::Vector3f &v2=pVecXYZ[2];
				nvec[0]=math::Vector3f(
					(v0[i])-(v1[i]),
					pVecUV[0].x()-pVecUV[1].x(),
					pVecUV[0].y()-pVecUV[1].y()
					);
				nvec[1]=math::Vector3f(
					v0[i]-v2[i],
					pVecUV[0].x()-pVecUV[2].x(),
					pVecUV[0].y()-pVecUV[2].y()
					);

				const float *pXYZ=pVecXYZ[0];
				
				planes[i].normal(nvec[0]^nvec[1]);
				planes[i].d(
					(const math::Vector3f &)(planes[i].normal())
					*
					(const math::Vector3f &)(math::Vector3f(pXYZ [i],pVecUV[0].x(),pVecUV[0].y()))
					);
			}

			// tangent = ( -B0/A0,-B1/A1,-B2/A2 )
			// binormal = ( -C0/A0,-C1/A1,-C2/A2 )
			math::Vector3f tangent,binormal;
			for (i=0;i<3;++i) {
				if (planes[i].a()!=0.0f) {
					tangent[i]=-planes[i].b()/planes[i].a();
					binormal[i]=-planes[i].c()/planes[i].a();
				}
			}

			for (i=0;i<3;++i) {
				pTangents[indices[i]]+=tangent;
				pBinormal[indices[i]]+=binormal;
			}
		}

		viterator.positionInStream(0);
		for (ion_uint32 vtx=0;vtx<vtxstream.capacity();++vtx) {
			if (tangentstage!=0xffffffff) viterator.texcoord3D(tangentstage,pTangents[vtx]);
			if (binormalstage!=0xffffffff) viterator.texcoord3D(binormalstage, viterator.normal() ^ pTangents[vtx]  );

			viterator++;
		}

		delete [] pBinormal;
		delete [] pTangents;
	}

	void calculateTorus(Mesh &mesh,const float innerradius,const float outerradius,const unsigned long tesselation,
		const bool calculateTangentspace)
	{
		unsigned long numVertices=(tesselation+1)*(tesselation+1);
		unsigned long numFaces=2*tesselation*tesselation;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		if (calculateTangentspace) {
			vf.addEntry(VertexFormatEntry_Texcoord3D,VertexFormatSemantic_Tangent);
			vf.addEntry(VertexFormatEntry_Texcoord3D,VertexFormatSemantic_Binormal);
		}

		mesh.reinit(numFaces,numVertices,vf,(numVertices<65536) ? Indexformat_16bit : Indexformat_32bit);
		memset(mesh.attributebuffer().data(),0,mesh.attributebuffer().size());

		mesh.vertexstream().map(Map_Writeonly);
		Vertexiterator v(mesh.vertexstream());

		unsigned long ring;

		for (ring=0;ring<(tesselation+1);++ring) {
			float tpos=(float(ring)/float(tesselation));
			float t=2.0f*3.1415926535f*tpos;
			float sinT=sinf(t);
			float cosT=cosf(t);
			for (unsigned long i=0;i<(tesselation+1);++i) {
				float ppos=(float(i)/float(tesselation));
				float p=2.0f*3.1415926535f*ppos;
				float sinP=sinf(p);
				float cosP=cosf(p);

				math::Vector3f pos(
					(innerradius+outerradius*cosP)*cosT,
					(innerradius+outerradius*cosP)*sinT,
					outerradius*sinP
				);
				math::Vector3f nn(cosT*cosP,sinT*cosP,sinP);

				v.position(pos);
				v.texcoord2D(0,4.0f*tpos,2.0f*ppos);
				v.normal(nn);

				if (calculateTangentspace) {
					math::Vector3f tangent;
					tangent.x()=-2.0f*3.1415926535f*pos.y();
					tangent.y()=2.0f*3.1415926535f*pos.x();
					tangent.z()=0;
					tangent.normalize();

					v.texcoord3D(0,tangent);
					v.texcoord3D(1,nn^tangent);
				}

				++v;
			}
		}

		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		Indexiterator idx(mesh.indexstream());

		for (ring=0;ring<tesselation;++ring) {
			for (unsigned long i=0;i<tesselation;++i) {
				idx.positionInStream(((ring*tesselation+i)*2)*3+0); idx=ring*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2)*3+1); idx=(ring+1)*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2)*3+2); idx=ring*(tesselation+1)+i+1;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+0); idx=ring*(tesselation+1)+i+1;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+1); idx=(ring+1)*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+2); idx=(ring+1)*(tesselation+1)+i+1;
			}
		}


		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius((innerradius+outerradius)*0.5f);
	}


	void calculateCube(Mesh &mesh,const float xlength,const float ylength,const float zlength,const bool calculateTangentspace,const bool invertedFaces)
	{
		struct vtxentry {
			float x,y,z;
			float nx,ny,nz;
			float tx,ty,tz;
			float u,v;
		};

		const vtxentry vtxentries[4*6]={
			// Top
			{	-1,	-1,	-1,		0,	-1,	0,	1,0,0,	0,0	},
			{	 1,	-1,	-1,		0,	-1,	0,	1,0,0,	1,0	},
			{	 1,	-1,	 1,		0,	-1,	0,	1,0,0,	1,1	},
			{	-1,	-1,	 1,		0,	-1,	0,	1,0,0,	0,1	},

			// Bottom
			{	 1,	 1,	-1,		0,	 1,	0,	1,0,0,	1,1	},
			{	-1,	 1,	-1,		0,	 1,	0,	1,0,0,	0,1	},
			{	-1,	 1,	 1,		0,	 1,	0,	1,0,0,	0,0	},
			{	 1,	 1,	 1,		0,	 1,	0,	1,0,0,	1,0	},

			// Left
			{	-1,	-1,	-1,		-1,	0,	0,	0,-1,0,	0,0	},
			{	-1,	-1,	 1,		-1,	0,	0,	0,-1,0,	0,1	},
			{	-1,	 1,	 1,		-1,	0,	0,	0,-1,0,	1,1	},
			{	-1,	 1,	-1,		-1,	0,	0,	0,-1,0,	1,0	},

			// Right
			{	 1,	-1,	-1,		1,	0,	0,	0,1,0,	0,0	},
			{	 1,	 1,	-1,		1,	0,	0,	0,1,0,	1,0	},
			{	 1,	 1,	 1,		1,	0,	0,	0,1,0,	1,1	},
			{	 1,	-1,	 1,		1,	0,	0,	0,1,0,	0,1	},

			// Front
			{	-1,	-1,	-1,		0,	0,	-1,	1,0,0,	0,1	},
			{	-1,	 1,	-1,		0,	0,	-1,	1,0,0,	0,0	},
			{	 1,	 1,	-1,		0,	0,	-1,	1,0,0,	1,0	},
			{	 1,	-1,	-1,		0,	0,	-1,	1,0,0,	1,1	},

			// Back
			{	-1,	-1,	1,		0,	0,	1,	1,0,0,	0,0	},
			{	 1,	-1,	1,		0,	0,	1,	1,0,0,	1,0	},
			{	 1,	 1,	1,		0,	0,	1,	1,0,0,	1,1	},
			{	-1,	 1,	1,		0,	0,	1,	1,0,0,	0,1	}
		};



		unsigned long numVertices=4*6;
		unsigned long numFaces=2*6;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Diffuse,VertexFormatSemantic_None);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		if (calculateTangentspace) {
			vf.addEntry(VertexFormatEntry_Texcoord3D,VertexFormatSemantic_Tangent);
			vf.addEntry(VertexFormatEntry_Texcoord3D,VertexFormatSemantic_Binormal);
		}

		mesh.reinit(numFaces,numVertices,vf,Indexformat_16bit);
		memset(mesh.attributebuffer().data(),0,mesh.attributebuffer().size());

		const float xl=xlength*0.5f;
		const float yl=ylength*0.5f;
		const float zl=zlength*0.5f;

		mesh.vertexstream().map(Map_Writeonly);
		Vertexiterator v(mesh.vertexstream());

		for (ion_uint32 vtx=0;vtx<numVertices;++vtx) {
			math::Vector3f nn(vtxentries[vtx].nx,vtxentries[vtx].ny,vtxentries[vtx].nz);

			v.position(vtxentries[vtx].x*xl,vtxentries[vtx].y*yl,vtxentries[vtx].z*zl);
			v.diffuseColor(0,255,255,255);
			v.normal(invertedFaces ? -nn : nn);
			v.texcoord2D(0,vtxentries[vtx].u,vtxentries[vtx].v);

			if (calculateTangentspace) {
				math::Vector3f tangent(vtxentries[vtx].tx,vtxentries[vtx].ty,vtxentries[vtx].tz);

				v.texcoord3D(0,tangent);
				v.texcoord3D(1,nn^tangent);
			}

			++v;
		}

		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		Indexiterator idx(mesh.indexstream());

		for (ion_uint32 face=0;face<numFaces;++face) {
			ion_uint32 i=face*4;

			if (invertedFaces) {
				idx=i+0; ++idx;
				idx=i+2; ++idx;
				idx=i+1; ++idx;

				idx=i+0; ++idx;
				idx=i+3; ++idx;
				idx=i+2; ++idx;
			} else {
				idx=i+0; ++idx;
				idx=i+1; ++idx;
				idx=i+2; ++idx;

				idx=i+0; ++idx;
				idx=i+2; ++idx;
				idx=i+3; ++idx;
			}

		}

		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius(math::Vector3f(xl,yl,zl).length());
	}

	void calculateSkybox(Mesh &mesh,const float xlength,const float ylength,const float zlength)
	{
		struct vtxentry {
			float x,y,z;
			float nx,ny,nz;
			float u,v;
		};

		const vtxentry vtxentries[4*6]={
			// Bottom
			{	-1,	-1,	-1,		0,	-1,	0,	0,0	},
			{	 1,	-1,	-1,		0,	-1,	0,	1,0	},
			{	 1,	-1,	 1,		0,	-1,	0,	1,1	},
			{	-1,	-1,	 1,		0,	-1,	0,	0,1	},

			// Top
			{	 1,	 1,	-1,		0,	 1,	0,	1,0	},
			{	-1,	 1,	-1,		0,	 1,	0,	0,0	},
			{	-1,	 1,	 1,		0,	 1,	0,	0,1	},
			{	 1,	 1,	 1,		0,	 1,	0,	1,1	},

			// Left
			{	-1,	-1,	-1,		-1,	0,	0,	0,1	},
			{	-1,	-1,	 1,		-1,	0,	0,	1,1	},
			{	-1,	 1,	 1,		-1,	0,	0,	1,0	},
			{	-1,	 1,	-1,		-1,	0,	0,	0,0	},

			// Right
			{	 1,	-1,	-1,		1,	0,	0,	1,1	},
			{	 1,	 1,	-1,		1,	0,	0,	1,0	},
			{	 1,	 1,	 1,		1,	0,	0,	0,0	},
			{	 1,	-1,	 1,		1,	0,	0,	0,1	},

			// Back
			{	-1,	-1,	-1,		0,	0,	-1,	1,1	},
			{	-1,	 1,	-1,		0,	0,	-1,	1,0	},
			{	 1,	 1,	-1,		0,	0,	-1,	0,0	},
			{	 1,	-1,	-1,		0,	0,	-1,	0,1	},

			// Front
			{	-1,	-1,	1,		0,	0,	1,	0,1	},
			{	 1,	-1,	1,		0,	0,	1,	1,1	},
			{	 1,	 1,	1,		0,	0,	1,	1,0	},
			{	-1,	 1,	1,		0,	0,	1,	0,0	}
		};



		unsigned long numVertices=4*6;
		unsigned long numFaces=2*6;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);
		vf.addEntry(VertexFormatEntry_Diffuse,VertexFormatSemantic_None);

		mesh.reinit(numFaces,numVertices,vf,Indexformat_16bit);

		const float xl=xlength*0.5f;
		const float yl=ylength*0.5f;
		const float zl=zlength*0.5f;

		mesh.vertexstream().map(Map_Writeonly);
		Vertexiterator v(mesh.vertexstream());

		for (ion_uint32 vtx=0;vtx<numVertices;++vtx) {
			v.position(vtxentries[vtx].x*xl,vtxentries[vtx].y*yl,vtxentries[vtx].z*zl);
			v.normal(vtxentries[vtx].nx,vtxentries[vtx].ny,vtxentries[vtx].nz);
			v.texcoord2D(0,vtxentries[vtx].u,vtxentries[vtx].v);
			v.diffuseColor(0xffffffff);
			++v;
		}

		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		Indexiterator idx(mesh.indexstream());

		for (ion_uint32 face=0;face<numFaces;++face) {
			ion_uint32 i=face*4;

			idx=i+0; ++idx;
			idx=i+2; ++idx;
			idx=i+1; ++idx;

			idx=i+2; ++idx;
			idx=i+0; ++idx;
			idx=i+3; ++idx;

			mesh.attributebuffer()[face]=face/2; // 0,0,1,1,2,2,3,3....
		}

		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius(math::Vector3f(xl,yl,zl).length());
	}

	void calculateSphere(Mesh &mesh,const float radius,const unsigned int tesselation)
	{
	}

	void calculateCylinderX(Mesh &mesh,const float radius,const float length,const unsigned int tesselation)
	{
		const unsigned int numEndFaces=tesselation+1;
		const unsigned int numFaces=numEndFaces*2+(tesselation+3)*2;
		const unsigned int numEndVertices=tesselation+3;
		const unsigned int numVertices=numEndVertices*2;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		mesh.reinit(numFaces,numVertices,vf,(numVertices<65536) ? Indexformat_16bit : Indexformat_32bit);
		memset(mesh.attributebuffer().data(),0,mesh.attributebuffer().size());

		mesh.vertexstream().map(Map_Writeonly);
		const float upperend=length*0.5f,lowerend=length*(-0.5f);
		{
			Vertexiterator vtx1(mesh.vertexstream(),0);
			Vertexiterator vtx2(mesh.vertexstream(),numEndVertices);

			for (unsigned int v=0;v<numEndVertices;++v) {
				float r=((float)v)/((float)numEndVertices)*2.0f*3.1415936535f;
				const float a=radius*cosf(r);
				const float b=radius*sinf(r);
				vtx1.position(lowerend,a,b);
				vtx2.position(upperend,a,b);
				vtx1.normal(math::Vector3f(lowerend,a,b).normalizedVector());
				vtx2.normal(math::Vector3f(upperend,a,b).normalizedVector());

				++vtx1; ++vtx2;
			}
		}
		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		{
			Indexiterator idx1(mesh.indexstream(),0);
			Indexiterator idx2(mesh.indexstream(),numEndFaces*3);

			unsigned int face;
			for (face=0;face<numEndFaces;++face) {
				idx1=0; ++idx1;
				idx1=face+2; ++idx1;
				idx1=face+1; ++idx1;

				idx2=numEndVertices; ++idx2;
				idx2=numEndVertices+face+1; ++idx2;
				idx2=numEndVertices+face+2; ++idx2;
			}

			idx1.positionInStream(numEndFaces*3*2);

			for (face=0;face<(tesselation+3);++face) {
				unsigned int f2=((face+1)%numEndVertices);

				idx1=face; ++idx1;
				idx1=f2; ++idx1;
				idx1=face+numEndVertices; ++idx1;

				idx1=face+numEndVertices; ++idx1;
				idx1=f2; ++idx1;
				idx1=numEndVertices+f2; ++idx1;
			}
		}
		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius(math::Vector3f(length*0.5f,radius,0).length());
	}





	void calculateCylinderY(Mesh &mesh,const float radius,const float length,const unsigned int tesselation)
	{
		const unsigned int numEndFaces=tesselation+1;
		const unsigned int numFaces=numEndFaces*2+(tesselation+3)*2;
		const unsigned int numEndVertices=tesselation+3;
		const unsigned int numVertices=numEndVertices*2;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		mesh.reinit(numFaces,numVertices,vf,(numVertices<65536) ? Indexformat_16bit : Indexformat_32bit);
		memset(mesh.attributebuffer().data(),0,mesh.attributebuffer().size());

		mesh.vertexstream().map(Map_Writeonly);
		const float upperend=length*0.5f,lowerend=length*(-0.5f);
		{
			Vertexiterator vtx1(mesh.vertexstream(),0);
			Vertexiterator vtx2(mesh.vertexstream(),numEndVertices);

			for (unsigned int v=0;v<numEndVertices;++v) {
				float r=((float)v)/((float)numEndVertices)*2.0f*3.1415936535f;
				const float a=radius*cosf(r);
				const float b=radius*sinf(r);
				vtx1.position(a,lowerend,b);
				vtx2.position(a,upperend,b);
				vtx1.normal(math::Vector3f(a,lowerend,b).normalizedVector());
				vtx2.normal(math::Vector3f(a,upperend,b).normalizedVector());

				++vtx1; ++vtx2;
			}
		}
		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		{
			Indexiterator idx1(mesh.indexstream(),0);
			Indexiterator idx2(mesh.indexstream(),numEndFaces*3);

			unsigned int face;
			for (face=0;face<numEndFaces;++face) {
				idx1=0; ++idx1;
				idx1=face+2; ++idx1;
				idx1=face+1; ++idx1;

				idx2=numEndVertices; ++idx2;
				idx2=numEndVertices+face+1; ++idx2;
				idx2=numEndVertices+face+2; ++idx2;
			}

			idx1.positionInStream(numEndFaces*3*2);

			for (face=0;face<(tesselation+3);++face) {
				unsigned int f2=((face+1)%numEndVertices);

				idx1=face; ++idx1;
				idx1=f2; ++idx1;
				idx1=face+numEndVertices; ++idx1;

				idx1=face+numEndVertices; ++idx1;
				idx1=f2; ++idx1;
				idx1=numEndVertices+f2; ++idx1;
			}
		}
		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius(math::Vector3f(length*0.5f,radius,0).length());
	}





	void calculateCylinderZ(Mesh &mesh,const float radius,const float length,const unsigned int tesselation)
	{
		const unsigned int numEndFaces=tesselation+1;
		const unsigned int numFaces=numEndFaces*2+(tesselation+3)*2;
		const unsigned int numEndVertices=tesselation+3;
		const unsigned int numVertices=numEndVertices*2;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		mesh.reinit(numFaces,numVertices,vf,(numVertices<65536) ? Indexformat_16bit : Indexformat_32bit);
		memset(mesh.attributebuffer().data(),0,mesh.attributebuffer().size());

		mesh.vertexstream().map(Map_Writeonly);
		const float upperend=length*0.5f,lowerend=length*(-0.5f);
		{
			Vertexiterator vtx1(mesh.vertexstream(),0);
			Vertexiterator vtx2(mesh.vertexstream(),numEndVertices);

			for (unsigned int v=0;v<numEndVertices;++v) {
				float r=((float)v)/((float)numEndVertices)*2.0f*3.1415936535f;
				const float a=radius*cosf(r);
				const float b=radius*sinf(r);
				vtx1.position(a,b,lowerend);
				vtx2.position(a,b,upperend);
				vtx1.normal(math::Vector3f(a,b,lowerend).normalizedVector());
				vtx2.normal(math::Vector3f(a,b,upperend).normalizedVector());

				++vtx1; ++vtx2;
			}
		}
		mesh.vertexstream().unmap();

		mesh.indexstream().map(Map_Writeonly);
		{
			Indexiterator idx1(mesh.indexstream(),0);
			Indexiterator idx2(mesh.indexstream(),numEndFaces*3);

			unsigned int face;
			for (face=0;face<numEndFaces;++face) {
				idx1=0; ++idx1;
				idx1=face+2; ++idx1;
				idx1=face+1; ++idx1;

				idx2=numEndVertices; ++idx2;
				idx2=numEndVertices+face+1; ++idx2;
				idx2=numEndVertices+face+2; ++idx2;
			}

			idx1.positionInStream(numEndFaces*3*2);

			for (face=0;face<(tesselation+3);++face) {
				unsigned int f2=((face+1)%numEndVertices);

				idx1=face; ++idx1;
				idx1=f2; ++idx1;
				idx1=face+numEndVertices; ++idx1;

				idx1=face+numEndVertices; ++idx1;
				idx1=f2; ++idx1;
				idx1=numEndVertices+f2; ++idx1;
			}
		}
		mesh.indexstream().unmap();

		mesh.attributetable().recalculate(mesh.attributebuffer());

		mesh.boundingSphere().center(math::Vector3f(0,0,0));
		mesh.boundingSphere().radius(math::Vector3f(length*0.5f,radius,0).length());
	}



}
}
