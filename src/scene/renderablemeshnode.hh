#ifndef ION_SCENE_RENDERABLEMESHNODE_HH_INCLUDED
#define ION_SCENE_RENDERABLEMESHNODE_HH_INCLUDED

#include "../video/propertytable.hh"
#include "renderable.hh"
#include "renderablemesh.hh"
#include "renderjob.hh"
#include "shader.hh"

#include <vector>

namespace ion {
namespace scene {

	class Camera;
	class Renderer;

	class ION_API RenderableMeshNode:public Renderable
	{
	public:
		RenderableMeshNode(const base::String& identifier);
		RenderableMeshNode(const base::String& identifier,RenderableMesh& rRenderableMesh);
		~RenderableMeshNode();

		const math::SphereVolume& boundingSphere() const;

		ion_uint32 numMaterials() const;

		const video::Propertytable* materialProperties(const ion_uint32 index) const;
		video::Propertytable* materialProperties(const ion_uint32 index);
		void materialProperties(const ion_uint32 index,video::Propertytable *pProperties);
	
		Shader* shader(const ion_uint32 index) const;
		void shader(const ion_uint32 index,Shader *pShader);

		const char *type() const;
		void respawn(base::Streamable& source);
		void serialize(base::Streamable& dest);

		void update(const Camera& camera,const Updatemode mode);

		//void visit(Renderjobqueue& queue) const;
		ion_uint32 numJobs() const;
		Renderjob* job(const ion_uint32 jobnr);

		void renderableMesh(RenderableMesh& rRenderableMesh);
		const RenderableMesh* renderableMesh() const;

		void restoreVideoResources();

		bool isValid() const;

		Camera *m_pRTCamera;
	protected:
		void rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer);
		void clearJobs();

		std::vector < Renderjobqueue* > *m_pJobqueues;
		std::vector < Renderjob* > *m_pJobs;
		RenderableMesh* m_pRenderableMesh;
		std::vector < Shader* > *m_pShader;
		std::vector < video::Propertytable* > *m_pMaterialproperties;
	};

}
}

#endif
