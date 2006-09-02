#ifndef ION_VIDEO_LOADOBJ_HH_INCLUDED
#define ION_VIDEO_LOADOBJ_HH_INCLUDED

#include "../base/streamable.hh"
#include "mesh.hh"

namespace ion {
namespace video {

	/*class ION_API OBJLoader
	{
	public:
		OBJLoader(base::Streamable &streamable);
		~OBJLoader();

		ion_uint32 numMeshes() const;
		const Memmesh* mesh(const ion_uint32 index) const;

	protected:
		Memmesh **m_ppMeshes;
		ion_uint32 m_NumMeshes;
	};*/

	ION_API void loadOBJ(base::Streamable &streamable,Mesh &rMesh,const float scale=1.0f,const bool useTangentspace=false);

}
}

#endif
