#ifndef MINIRACER_TERRAIN_HH_INCLUDED
#define MINIRACER_TERRAIN_HH_INCLUDED

#include "../base/streamable.hh"
#include "../scene/renderable.hh"
#include "../scene/renderer.hh"
#include "../scene/renderjob.hh"
#include "../scene/space.hh"
#include "../video/mesh.hh"
#include "../video/videodevice.hh"
#include "../math/spherevolume.hh"
#include "Newton.h"

#include <vector>

class TerrainPatch;

class Terrain:public ion::scene::Node
{
public:
	Terrain(const ion::base::String& identifier,NewtonWorld *pNewtonworld,ion::scene::Renderer& rRenderer,
		ion::base::Streamable& heightmap,const ion_uint32 width,const ion_uint32 depth,const ion_uint32 patchwidth,
		const ion_uint32 patchdepth,const ion::math::Vector3f& offset,const ion::math::Vector3f& size);
	~Terrain();

	void terrainTexture(ion::video::Texture2D *pTex);

	const char *type() const;
	void respawn(ion::base::Streamable& source);
	void serialize(ion::base::Streamable& dest);

	std::vector< TerrainPatch* >& patches();

	bool isValid() const;
protected:
	NewtonWorld *m_pNewtonworld;
	NewtonCollision *m_pTreeCollision;
	std::vector< TerrainPatch* > m_Patches;
};

class TerrainPatch:public ion::scene::Renderable
{
public:
	TerrainPatch(const ion::base::String& identifier,ion::scene::Renderer& rRenderer,const ion_uint16 *pHeightdata,
		const ion_uint32 width,const ion_uint32 pitch,const ion_uint32 depth,const bool lastrow,const ion::math::Vector3f& offset,
		const ion::math::Vector3f& size);
	~TerrainPatch();

	const ion::math::SphereVolume& boundingSphere() const;
	ion::math::SphereVolume& boundingSphere();

	void terrainTexture(ion::video::Texture2D *pTex);

	const char *type() const;
	void respawn(ion::base::Streamable& source);
	void serialize(ion::base::Streamable& dest);
	void visit(ion::scene::Renderjobqueue& queue) const;
	void restoreVideoResources();

	bool isValid() const;
protected:
	void rendererChanging(const bool videodeviceIsDifferent,ion::scene::Renderer *pNewRenderer);

	ion::video::Texture2D *m_pTex;
	ion::video::Vertexstream *m_pVertices;
	ion::video::Indexstream *m_pIndices;
	ion::math::SphereVolume m_BoundingSphere;
	ion::scene::Renderjob *m_pRenderjob;
	ion::video::Propertytable m_Properties;
};

#endif
