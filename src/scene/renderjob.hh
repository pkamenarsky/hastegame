#ifndef ION_SCENE_RENDERJOB_HH_INCLUDED
#define ION_SCENE_RENDERJOB_HH_INCLUDED

#include "../math/transform.hh"
#include "../base/basememobj.hh"
#include "../video/propertytable.hh"
#include "../video/videodevice.hh"

namespace ion {
namespace scene {

	class Camera;
	class Renderable;
	class Shader;

	class ION_API Renderjob:public base::BaseMemObj
	{
	public:
		Renderjob(const Renderable& rRenderable);
		Renderjob(const Renderjob& src);

		void clear();

		void clone(const Renderjob& src);
		Renderjob& operator =(const Renderjob& src);

		const math::Transform& worldtransform() const;
		math::Transform& worldtransform();

		void primitivestype(const video::PrimitivesType type);
		video::PrimitivesType primitivestype() const;

		void firstElement(const unsigned long first);
		unsigned long firstElement() const;

		void numElements(const unsigned long num);
		unsigned long numElements() const;

		void indexOffset(const unsigned long offset);
		unsigned long indexOffset() const;

		void vertexstream(video::Vertexstream& rVertexstream);
		video::Vertexstream& vertexstream() const;

		void indexstream(video::Indexstream* pIndexstream);
		video::Indexstream* indexstream() const;

		void shader(Shader* pShader);
		Shader* shader() const;

		void rendertargetCamera(Camera *pCamera);
		Camera* rendertargetCamera() const;

		bool isValid() const;

		void renderable(const Renderable& rRenderable);
		const Renderable& renderable() const;

		void propertytable(video::Propertytable *pPropertytable);
		video::Propertytable *propertytable() const;

		void viewerDistance(const float num);
		float viewerDistance() const;

		virtual void drawIndexed(video::Videodevice &rVideodevice);
		virtual void draw(video::Videodevice &rVideodevice);

	protected:
		math::Transform m_Worldtransform;
		video::PrimitivesType m_PrimitivesType;
		unsigned long m_FirstElement,m_NumElements,m_IndexOffset;
		video::Vertexstream *m_pVertexstream;
		video::Indexstream *m_pIndexstream;
		Shader *m_pShader;
		const Renderable *m_pRenderable;
		Camera *m_pCamera;
		video::Propertytable *m_pPropertytable;
		float m_ViewerDistance;
	};

}
}

#endif
