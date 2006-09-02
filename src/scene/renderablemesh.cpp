#include "../video/mempool.hh"
#include "../video/mesh.hh"
#include "renderer.hh"
#include "renderablemesh.hh"

namespace ion {
namespace scene {

	RenderableMesh::RenderableMesh(const base::String& identifier,Renderer& rRenderer,video::Mesh *pSourceMesh):
	Mesh(identifier),m_rRenderer(rRenderer),m_pSourceMesh(pSourceMesh),m_pIndexstream(0),m_pVertexstream(0)
	{
		if (pSourceMesh!=0) {
			pSourceMesh->addRef();
			transferFromSourceMesh();
		}
	}
	
	RenderableMesh::~RenderableMesh()
	{
		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;
		if (m_pSourceMesh) m_pSourceMesh->release();
	}

	Renderer& RenderableMesh::renderer() const
	{
		return m_rRenderer;
	}

	video::Mesh* RenderableMesh::sourceMesh() const
	{
		return m_pSourceMesh;
	}

	void RenderableMesh::sourceMesh(video::Mesh* pMesh)
	{
		if (m_pSourceMesh==pMesh) return;

		if (pMesh!=0) pMesh->addRef();
		if (m_pSourceMesh!=0) m_pSourceMesh->release();
		m_pSourceMesh=pMesh;
		transferFromSourceMesh();
	}
	

	void RenderableMesh::transferFromSourceMesh()
	{
		if (m_pSourceMesh!=0) {
			m_pSourceMesh->vertexstream().map(video::Map_Readonly);
			m_pSourceMesh->indexstream().map(video::Map_Readonly);
			transferFrom(*m_pSourceMesh);
			m_pSourceMesh->indexstream().unmap();
			m_pSourceMesh->vertexstream().unmap();
		}
	}

	const video::Attributebuffer& RenderableMesh::attributebuffer() const
	{
		return m_Attributebuffer;
	}

	const video::Attributetable& RenderableMesh::attributetable() const
	{
		return m_Attributetable;
	}

	const video::Vertexstream& RenderableMesh::vertexstream() const
	{
		return *m_pVertexstream;
	}

	const video::Indexstream& RenderableMesh::indexstream() const
	{
		return *m_pIndexstream;
	}

	video::Attributebuffer& RenderableMesh::attributebuffer()
	{
		return m_Attributebuffer;
	}

	video::Attributetable& RenderableMesh::attributetable()
	{
		return m_Attributetable;
	}

	video::Vertexstream& RenderableMesh::vertexstream()
	{
		return *m_pVertexstream;
	}

	video::Indexstream& RenderableMesh::indexstream()
	{
		return *m_pIndexstream;
	}

	void RenderableMesh::reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const video::Vertexformat& vertexformat,const video::Indexformat indexformat)
	{
		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;

		m_pVertexstream=m_rRenderer.videodevice()->createVertexstream(
			numVertices,
			vertexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);
			
		m_pIndexstream=m_rRenderer.videodevice()->createIndexstream(
			numFaces*3,
			indexformat,
			video::Streamflags_Writeonly,
			video::Mempool_Managed
			);

		m_Attributebuffer.attributeAmount(numFaces);
	}

	bool RenderableMesh::isValid() const
	{
		return (m_pIndexstream!=0) && (m_pVertexstream!=0) && m_rRenderer.isValid() &&
			((m_pSourceMesh!=0) ? m_pSourceMesh->isValid() : true);
	}

}
}
