#ifndef ION_SCENE_ISCLOADER_HH_INCLUDED
#define ION_SCENE_ISCLOADER_HH_INCLUDED

#include "../base/localstreamable.hh"
#include "../base/refcountedsource.hh"
#include "group.hh"
#include "renderer.hh"
#include "renderablelist.hh"
#include "renderablemesh.hh"
#include "renderablemeshnode.hh"
#include "shader.hh"

namespace ion {
namespace scene {

	ION_API Group* loadISC(base::LocalStreamable& streamable,Renderer& renderer,Renderablelist* pRenderablelist,
		Node *pParent,
		base::Refcountedsource< video::Texture2D > *pTexturesource,
		base::Refcountedsource< Shader > *pShadersource
		);

}
}

#endif
