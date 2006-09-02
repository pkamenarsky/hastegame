#ifndef ION_SCENE_RENDERABLEMESH_HH_INCLUDED
#define ION_SCENE_RENDERABLEMESH_HH_INCLUDED

#include "../base/refcounted.hh"
#include "../base/string.hh"
#include "../video/mesh.hh"

namespace ion {
namespace scene {


	class ION_API RenderableMesh:public video::Mesh
	{
	public:
		RenderableMesh(const base::String& identifier,Renderer& rRenderer,video::Mesh *pSourceMesh=0);
		~RenderableMesh();
		
		Renderer& renderer() const;

		video::Mesh* sourceMesh() const;
		void sourceMesh(video::Mesh* pMesh);
		void transferFromSourceMesh();

		const video::Attributebuffer& attributebuffer() const;
		const video::Attributetable& attributetable() const;
		const video::Vertexstream& vertexstream() const;
		const video::Indexstream& indexstream() const;

		video::Attributebuffer& attributebuffer();
		video::Attributetable& attributetable();
		video::Vertexstream& vertexstream();
		video::Indexstream& indexstream();
		
		void reinit(const ion_uint32 numFaces,const ion_uint32 numVertices,
			const video::Vertexformat& vertexformat,const video::Indexformat indexformat);

		bool isValid() const;
	protected:

		video::Attributebuffer m_Attributebuffer;
		video::Attributetable m_Attributetable;
		Renderer &m_rRenderer;
		video::Mesh *m_pSourceMesh;
		video::Indexstream *m_pIndexstream;
		video::Vertexstream *m_pVertexstream;
	};

}
}

#endif
