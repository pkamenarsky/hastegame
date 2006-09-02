#include <algorithm>
#include <assert.h>
#include "renderer.hh"
#include "renderjob.hh"
#include "renderjobqueue.hh"
#include "renderablemeshnode.hh"

namespace ion {
namespace scene {

	RenderableMeshNode::RenderableMeshNode(const base::String& identifier):Renderable(identifier),
		m_pJobqueues(new std::vector < Renderjobqueue* >),
		m_pJobs(new std::vector < Renderjob* >),m_pRenderableMesh(0)
	{
		m_pRTCamera=0;

		m_pShader=new std::vector < Shader* >;
		m_pMaterialproperties=new std::vector < video::Propertytable* >;
	}

	RenderableMeshNode::RenderableMeshNode(const base::String& identifier,RenderableMesh& rRenderableMesh):
	Renderable(identifier),m_pJobqueues(new std::vector < Renderjobqueue* >),
	m_pJobs(new std::vector < Renderjob* >),m_pRenderableMesh(0)
	{
		m_pRTCamera=0;

		m_pShader=new std::vector < Shader* >;
		m_pMaterialproperties=new std::vector < video::Propertytable* >;

		renderableMesh(rRenderableMesh);
	}
	
	RenderableMeshNode::~RenderableMeshNode()
	{
		if (m_pRenderableMesh) m_pRenderableMesh->release();
		
		std::vector < video::Propertytable* >::iterator itp=m_pMaterialproperties->begin();
		for (;itp!=m_pMaterialproperties->end();++itp) {
			video::Propertytable* pProps=(*itp);
			if (pProps!=0)
				pProps->release();
		}
		delete m_pMaterialproperties;

		clearJobs();
		delete m_pShader;
		delete m_pJobs;
		delete m_pJobqueues;
	}

	const math::SphereVolume& RenderableMeshNode::boundingSphere() const
	{
		assert(m_pRenderableMesh!=0);
		return m_pRenderableMesh->boundingSphere();
	}

	ion_uint32 RenderableMeshNode::numMaterials() const
	{
		if (m_pRenderableMesh==0) return 0;
		return m_pRenderableMesh->attributetable().numArrays();
	}

	const video::Propertytable* RenderableMeshNode::materialProperties(const ion_uint32 index) const
	{
		return (index<numMaterials()) ? (*m_pMaterialproperties)[index] : 0;
	}

	video::Propertytable* RenderableMeshNode::materialProperties(const ion_uint32 index)
	{
		return (index<numMaterials()) ? (*m_pMaterialproperties)[index] : 0;
	}

	void RenderableMeshNode::materialProperties(const ion_uint32 index,video::Propertytable *pProperties)
	{
		if ((index<numMaterials())) {
			video::Propertytable *pOldProps=(*m_pMaterialproperties)[index];
			(*m_pMaterialproperties)[index]=pProperties;
			if (pOldProps!=pProperties) {
				if (pProperties) pProperties->addRef();
				if (pOldProps) pOldProps->release();
			}

		}
	}

	Shader* RenderableMeshNode::shader(const ion_uint32 index) const
	{
		return (index<numMaterials()) ? (*m_pShader)[index] : 0;
	}

	void RenderableMeshNode::shader(const ion_uint32 index,Shader *pShader)
	{
		if (index<numMaterials()) (*m_pShader)[index]=pShader;
	}

	const char* RenderableMeshNode::type() const
	{
		return "RenderableMeshNode";
	}

	void RenderableMeshNode::respawn(base::Streamable& source)
	{
	}

	void RenderableMeshNode::serialize(base::Streamable& dest)
	{
	}

	void RenderableMeshNode::update(const Camera& camera,const Updatemode mode)
	{
		if (m_pRenderableMesh==0) return;

		ion_uint32 nr=0;
		std::vector < Renderjobqueue* >::iterator itp=m_pJobqueues->begin();
		for (;itp!=m_pJobqueues->end();++itp,++nr) {
			Renderjobqueue* pQueue=(*itp);
			video::Propertytable* pMaterialprops=(*m_pMaterialproperties)[nr];
			Shader* pShader=(*m_pShader)[nr];

			for (std::vector < Renderjob * > ::iterator it=pQueue->jobs().begin();it!=pQueue->jobs().end();++it) {
				(*it)->propertytable(pMaterialprops);
				(*it)->shader(pShader);
				(*it)->worldtransform()=transform();
				(*it)->rendertargetCamera(m_pRTCamera);
			}
		}
	}

	/*void RenderableMeshNode::visit(Renderjobqueue& queue) const
	{
		if (m_pRenderableMesh==0) return;

		ion_uint32 nr=0;
		std::vector < Renderjobqueue* >::iterator itp=m_pJobqueues->begin();
		for (;itp!=m_pJobqueues->end();++itp,++nr) {
			Renderjobqueue* pQueue=(*itp);
			video::Propertytable* pMaterialprops=(*m_pMaterialproperties)[nr];
			Shader* pShader=(*m_pShader)[nr];

			for (std::vector < Renderjob * > ::iterator it=pQueue->jobs().begin();it!=pQueue->jobs().end();++it) {
				(*it)->propertytable(pMaterialprops);
				(*it)->shader(pShader);
				(*it)->worldtransform()=transform();
				(*it)->rendertargetCamera(m_pRTCamera);
			}

			queue.appendQueue(*pQueue);
		}
	}*/

	ion_uint32 RenderableMeshNode::numJobs() const
	{
		return (ion_uint32)(m_pJobs->size());
	}

	Renderjob* RenderableMeshNode::job(const ion_uint32 jobnr)
	{
		return (jobnr<numJobs()) ? *(m_pJobs->begin()+jobnr) : 0;
	}

	void RenderableMeshNode::renderableMesh(RenderableMesh& rRenderableMesh)
	{
		if ( (&rRenderableMesh)==m_pRenderableMesh) return;
		
		rRenderableMesh.addRef();
		if (m_pRenderableMesh!=0) m_pRenderableMesh->release();
		m_pRenderableMesh=&rRenderableMesh;

		m_pMaterialproperties->resize(numMaterials());
		std::fill(m_pMaterialproperties->begin(),m_pMaterialproperties->end(),(video::Propertytable *)0);

		m_pShader->resize(numMaterials());
		std::fill(m_pShader->begin(),m_pShader->end(),(Shader*)0);

		clearJobs();

		{
			for (ion_uint32 arraynr=0;arraynr<m_pRenderableMesh->attributetable().numArrays();++arraynr) {
				Renderjobqueue *pQueue=new Renderjobqueue;

				const video::Attributearray* pArray=m_pRenderableMesh->attributetable().array(arraynr);

				video::Propertytable* pMaterialprops=(*m_pMaterialproperties)[arraynr];
				Shader* pShader=(*m_pShader)[arraynr];

				ion_uint32 numentries=pArray->numEntries();

				for (ion_uint32 entrynr=0;entrynr<numentries;++entrynr) {
					const video::Attributearrayentry *pEntry=pArray->entry(entrynr);

					Renderjob *rjob=new Renderjob(*this);
					rjob->propertytable(pMaterialprops);
					rjob->worldtransform()=transform();
					if (pShader) rjob->shader(pShader);
					rjob->firstElement(0);
					rjob->indexOffset(pEntry->startface()*3);
					rjob->numElements(pEntry->numFaces());
					rjob->primitivestype(video::Primitives_Triangles);
					rjob->indexstream(&(m_pRenderableMesh->indexstream()));
					rjob->vertexstream(m_pRenderableMesh->vertexstream());
					rjob->rendertargetCamera(m_pRTCamera);

					pQueue->addJob(*rjob);
					m_pJobs->push_back(rjob);
				}

				m_pJobqueues->push_back(pQueue);
			}
		}

	}

	const RenderableMesh* RenderableMeshNode::renderableMesh() const
	{
		return m_pRenderableMesh;
	}

	void RenderableMeshNode::restoreVideoResources()
	{
		if (m_pRenderableMesh->sourceMesh()) {
			m_pRenderableMesh->transferFromSourceMesh();
		}
	}

	void RenderableMeshNode::rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer)
	{
	}

	void RenderableMeshNode::clearJobs()
	{
		if (m_pJobqueues==0) return;

		std::vector < Renderjobqueue* >::iterator itp=m_pJobqueues->begin();
		for (;itp!=m_pJobqueues->end();++itp) {
			(*itp)->deleteJobs();
			delete (*itp);
		}
		m_pJobqueues->clear();
		m_pJobs->clear();
	}

	bool RenderableMeshNode::isValid() const
	{
		return (m_pRenderableMesh!=0) ? m_pRenderableMesh->isValid() : false;
	}

}
}
