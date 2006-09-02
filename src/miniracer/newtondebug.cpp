#include "../video/videodevice.hh"
#include "newtondebug.hh"

const ion::scene::Camera *pDebugCamera;
ion::video::VCache *pDebugVCache;

void debugShowGeometryCollision(const NewtonBody* body, int vertexCount, const dFloat* faceVertex, int id)
{
	int i;

	i = vertexCount - 1;
	ion::math::Vector3f p0 (faceVertex[i * 3 + 0], faceVertex[i * 3 + 1], faceVertex[i * 3 + 2]);
	for (i = 0; i < vertexCount; i ++) {
		ion::math::Vector3f p1 (faceVertex[i * 3 + 0], faceVertex[i * 3 + 1], faceVertex[i * 3 + 2]);

		pDebugVCache->position(p0); pDebugVCache->pushVertex();
		pDebugVCache->position(p1); pDebugVCache->pushVertex();
	
		p0 = p1;
	}
}


void  debugShowBodyCollision(const NewtonBody* body)
{
	pDebugVCache->begin(ion::video::Primitives_Lines);
	NewtonBodyForEachPolygonDo(body,debugShowGeometryCollision);
	pDebugVCache->end();
}

void newtonShowCollisionGeometry(NewtonWorld *pWorld,const ion::scene::Camera &rCamera,ion::video::VCache &rVCache)
{
	rVCache.videodevice().activateFFPipeline(ion::video::FixedfunctionVertex);
	rVCache.videodevice().activateFFPipeline(ion::video::FixedfunctionFragment);

	rVCache.videodevice().culling(false);

	rVCache.videodevice().fixedSetProjectionMatrix(rCamera.projectionmatrix());
	rVCache.videodevice().fixedSetViewMatrix(rCamera.viewmatrix());
	rVCache.videodevice().fixedSetWorldMatrix(ion::math::Matrix4f::identitymatrix());

	pDebugCamera=&rCamera;
	pDebugVCache=&rVCache;
	NewtonWorldForEachBodyDo(pWorld,debugShowBodyCollision);
}
