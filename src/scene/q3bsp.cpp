#include <string.h>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include "../base/datablock.hh"
#include "../base/physfsfile.hh"
#include "../base/log.hh"
#include "../math/quadraticbezier.hh"
#include "../video/indexiterator.hh"
#include "../video/picloader.hh"
#include "q3bsp.hh"
#include "renderjobqueue.hh"
#include "renderquery.hh"
#include "renderer.hh"

namespace ion {
namespace scene {

	///////////////////////////
	// BSPRenderable methods //
	///////////////////////////

	Q3BSP::BSPRenderable::BSPRenderable(Q3BSP& rBSP,Renderable &rRenderable,const ion_uint32 renderableidx):
	m_rQ3BSP(rBSP),m_rRenderable(rRenderable),m_Renderableindex(renderableidx)
	{
		m_pLeaves=new std::vector < Leaf* >;

		update();

		m_rRenderable.transformAlterationFunctor(this);
	}

	Q3BSP::BSPRenderable::~BSPRenderable()
	{
		clearLeafList();
		delete m_pLeaves;
	}
			
	const Renderable& Q3BSP::BSPRenderable::renderable() const
	{
		return m_rRenderable;
	}

	Renderable& Q3BSP::BSPRenderable::renderable()
	{
		return m_rRenderable;
	}

	void Q3BSP::BSPRenderable::transformAltered(const bool rotationAltered,const bool positionAltered)
	{
		update();
	}

	void Q3BSP::BSPRenderable::update()
	{
		updateLeafList();
	}

	void Q3BSP::BSPRenderable::updateLeafList()
	{
		// First, remove all leaf entries (and remove this renderable from all leaves)
		clearLeafList();

		// Now, find the leaves in range, add them to the list, and add this renderable to their lists
		addRenderableToLeaves(0);
	}

	void Q3BSP::BSPRenderable::clearLeafList()
	{
		for (std::vector < Leaf* >::iterator it=m_pLeaves->begin();it!=m_pLeaves->end();++it) {
			Leaf* pLeaf=(*it);
			std::set < ion_uint32 >::iterator itrenderable=pLeaf->m_pRenderables->find(m_Renderableindex);
			if (itrenderable!=pLeaf->m_pRenderables->end()) pLeaf->m_pRenderables->erase(itrenderable);
		}
		m_pLeaves->clear();
	}

	void Q3BSP::BSPRenderable::addRenderableToLeaves(const ion_int32 nodeNum)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_rQ3BSP.m_pLeaves-(nodeNum+1);

			math::Vector3f minV((float)(pLeaf->m_Mincoord[0]),(float)(pLeaf->m_Mincoord[1]),(float)(pLeaf->m_Mincoord[2]));
			math::Vector3f maxV((float)(pLeaf->m_Maxcoord[0]),(float)(pLeaf->m_Maxcoord[1]),(float)(pLeaf->m_Maxcoord[2]));

			minV-=m_rRenderable.transform().position();
			maxV-=m_rRenderable.transform().position();

			if (m_rRenderable.boundingSphere().intersectsWith(minV,maxV)) {
				m_pLeaves->push_back(pLeaf);
				pLeaf->m_pRenderables->insert(m_Renderableindex);
			}
		} else {
			Q3BSP::Node *pNode=&(m_rQ3BSP.m_pNodes[nodeNum]);

			math::Vector3f minV((float)(pNode->m_Mincoord[0]),(float)(pNode->m_Mincoord[1]),(float)(pNode->m_Mincoord[2]));
			math::Vector3f maxV((float)(pNode->m_Maxcoord[0]),(float)(pNode->m_Maxcoord[1]),(float)(pNode->m_Maxcoord[2]));

			minV-=m_rRenderable.transform().position();
			maxV-=m_rRenderable.transform().position();

			if (m_rRenderable.boundingSphere().intersectsWith(minV,maxV)) {
				addRenderableToLeaves(pNode->m_FrontChild);
				addRenderableToLeaves(pNode->m_BackChild);
			}
		}
	}

	//////////////////////
	// BSPLight methods //
	//////////////////////

	const ion_uint32 Q3BSP::BSPLight::s_IndexPadding=1024*3;

	Q3BSP::BSPLight::BSPLight(Q3BSP& rBSP,Light &rLight,const ion_uint32 lightidx):m_rQ3BSP(rBSP),m_pIndices(0),m_rLight(rLight),
	m_NumActiveJobs(0),m_Lightindex(lightidx)
	{
		m_pLeaves=new std::vector < Leaf* >;
		m_pBezierPatches=new std::vector < BezierPatch* >;

		m_NumJobs=(rBSP.m_NumSurfaces==0) ? 1 : rBSP.m_NumSurfaces;
		m_pJobs=new Renderjob*[m_NumJobs];
		m_pActiveJobs=new Renderjob*[m_NumJobs];
		for (ion_uint32 j=0;j<m_NumJobs;++j) {
			m_pJobs[j]=new Renderjob(m_rQ3BSP);
			m_pActiveJobs[j]=new Renderjob(m_rQ3BSP);
		}

		rLight.renderablelist().clear();
		rLight.renderableIndicesList().clear();
		rLight.renderablelist().addRenderable(m_rQ3BSP);
		rLight.renderableIndicesList().addIndex(0);

		update();

		m_rLight.transformAlterationFunctor(this);
	}

	Q3BSP::BSPLight::~BSPLight()
	{
		clearLeafList();

		for (ion_uint32 j=0;j<m_NumJobs;++j) {
			delete m_pActiveJobs[j];
			delete m_pJobs[j];
		}
		delete [] m_pActiveJobs;
		delete [] m_pJobs;

		delete m_pBezierPatches;
		delete m_pLeaves;
		if (m_pIndices) delete m_pIndices;
	}

	ion_uint32 Q3BSP::BSPLight::numLightingjobs() const
	{
		return m_NumActiveJobs+(ion_uint32)(m_pBezierPatches->size());
	}

	Renderjob* Q3BSP::BSPLight::lightingjob(const ion_uint32 jobnr)
	{
		if (jobnr<m_NumActiveJobs)
			return m_pActiveJobs[jobnr];
		else
			return (*m_pBezierPatches)[jobnr-m_NumActiveJobs]->m_pRenderjob;
	}

	const Light& Q3BSP::BSPLight::light() const
	{
		return m_rLight;
	}

	Light& Q3BSP::BSPLight::light()
	{
		return m_rLight;
	}

	void Q3BSP::BSPLight::transformAltered(const bool rotationAltered,const bool positionAltered)
	{
		update();
	}

	void Q3BSP::BSPLight::update()
	{
		//std::set<ion_uint32> renderableIndices;
		updateLeafList(/*renderableIndices*/);

		/*for (std::set<ion_uint32>::iterator it=renderableIndices.begin();it!=renderableIndices.end();++it) {
			ion_uint32 ridx=(*it);
			Renderable &rRenderable=(*(m_rQ3BSP.m_pRenderables))[ridx-1]->renderable();
			m_rLight.renderablelist().addRenderable(rRenderable);
			m_rLight.renderableIndicesList().addIndex(ridx);
		}*/
	}

	/*void Q3BSP::BSPLight::newLightindex(const ion_uint32 lightidx)
	{
		m_Lightindex=lightidx;
		addLightToLeaves(0);
	}*/

//#define DEBUG_CHECK_BSPLIGHT_INDEXSTREAM_BOUNDARIES

	void Q3BSP::BSPLight::updateLeafList(/*std::set<ion_uint32> &renderableIndices*/)
	{
		// First, remove all leaf entries (and remove this light from all leaves)
		clearLeafList();

		// Now, find the leaves in range, add them to the list, and add this light to their lists
		ion_uint32 numIndices=0;	// This counts the total amount of indices of all leaffaces
									// This is necessary for eventual IB resize

		std::vector < bool > alreadyVisible;
		alreadyVisible.resize(m_rQ3BSP.m_NumFaces);
		addLightToLeaves(0,numIndices,alreadyVisible/*,renderableIndices*/);

		ion_uint32 currentNumIndices=0;

		if (m_pIndices)
			currentNumIndices=(m_pIndices->capacity()==0) ? s_IndexPadding : ((m_pIndices->capacity()-1)/s_IndexPadding+1)*s_IndexPadding;

#ifdef DEBUG_CHECK_BSPLIGHT_INDEXSTREAM_BOUNDARIES
		base::logstream() << numIndices << " /// " << currentNumIndices << "\n";
#endif

		numIndices=(numIndices==0) ? s_IndexPadding : (numIndices/s_IndexPadding+1)*s_IndexPadding;
		if (numIndices>currentNumIndices) { // TODO: Should this reallocate the indexstream only when the amount of indices increases?
#ifdef DEBUG_CHECK_BSPLIGHT_INDEXSTREAM_BOUNDARIES
			base::logstream() << "m_rQ3BSP.renderer()->videodevice()->createIndexstream()" << currentNumIndices << " -> " << numIndices << "\n";
#endif
			if (m_pIndices) delete m_pIndices;
			m_pIndices=m_rQ3BSP.renderer()->videodevice()->createIndexstream(
				numIndices,
				(m_rQ3BSP.m_NumBSPVertices>65535) ? video::Indexformat_32bit : video::Indexformat_16bit,
				video::Streamflags_Dynamic|video::Streamflags_Writeonly,
				video::Mempool_Default
			);
		}

		std::fill(alreadyVisible.begin(),alreadyVisible.end(),false);

		m_NumActiveJobs=0;

		std::vector < std::vector < Q3BSP::Face* > > faces;
		faces.resize(m_NumJobs);

		for (std::vector < Leaf* >::iterator itleaf=m_pLeaves->begin();itleaf!=m_pLeaves->end();++itleaf) {
			Leaf *pLeaf=(*itleaf);

			for (ion_int32 f=0;f<pLeaf->m_NumLeaffaces;++f) {
				int faceidx=m_rQ3BSP.m_pLeaffaces[f+pLeaf->m_Leafface];

				if ((faceidx<0) || (faceidx>=(int)(m_rQ3BSP.m_NumFaces))) continue;
				if (alreadyVisible[faceidx]) continue;
				alreadyVisible[faceidx]=true;
				Q3BSP::Face &rFace=m_rQ3BSP.m_pFaces[faceidx];
				if ((rFace.m_Surfaceindex<0) || (rFace.m_Surfaceindex>=(ion_int32)m_NumJobs)) continue;
				if (m_rQ3BSP.m_pSurfaces[rFace.m_Surfaceindex].m_IsSky) continue;

				bool fullAABB=false;
				math::Vector3f minV((float)(pLeaf->m_Mincoord[0]),(float)(pLeaf->m_Mincoord[1]),(float)(pLeaf->m_Mincoord[2]));
				math::Vector3f maxV((float)(pLeaf->m_Maxcoord[0]),(float)(pLeaf->m_Maxcoord[1]),(float)(pLeaf->m_Maxcoord[2]));
				float rr=m_rLight.range()*m_rLight.range();
				fullAABB=((m_rLight.transform().position()-minV).lengthSquared()<rr) &&
					((m_rLight.transform().position()-maxV).lengthSquared()<rr); // If the entire AABB is in range, add all lights

				if ((rFace.m_Type==Q3BSP::Facetype_Polygon) || (rFace.m_Type==Q3BSP::Facetype_Mesh)) {
					bool inLight=fullAABB;

					// TODO: Uncomment this and include a WORKING sphere-polygon test
					// (One that also checks for plane-polygonarea intersections etc.)
					if (!fullAABB) {
						/*for (ion_int32 j=0;j<rFace.m_NumMeshverts;++j) {
							ion_uint32 idx=m_rQ3BSP.m_pMeshverts[rFace.m_FirstMeshvert+j]+rFace.m_FirstVertex;
							const math::Vector3f &v=m_rQ3BSP.m_pBSPVertices[idx].m_Position;
							float d=(m_rLight.transform().position()-v).lengthSquared();
							if (d<m_rLight.range()*m_rLight.range()) {
								inLight=true;
								break;
							}
						}*/

						for (ion_int32 j=0;j<rFace.m_NumMeshverts/3;++j) {
							const math::Vector3f* vv[3];
							for (int iij=0;iij<3;++iij) {
								ion_uint32 idx=m_rQ3BSP.m_pMeshverts[rFace.m_FirstMeshvert+j*3+iij]+rFace.m_FirstVertex;
								//const math::Vector3f &v1=m_rQ3BSP.m_pBSPVertices[idx].m_Position;
								vv[iij]=&(m_rQ3BSP.m_pBSPVertices[idx].m_Position);
							}
						
							if (sphereTriangleIntersection(m_rLight.range(),m_rLight.transform().position(),
								*(vv[0]),*(vv[1]),*(vv[2]))) {
								inLight=true;
								break;
							}
						}
					}

					if (inLight)
						faces[rFace.m_Surfaceindex].push_back(&rFace);
				} else if (rFace.m_Type==Q3BSP::Facetype_Patch) {
					minV=rFace.m_pBezierPatch->m_Mincoord-m_rLight.transform().position();
					maxV=rFace.m_pBezierPatch->m_Maxcoord-m_rLight.transform().position();

					if (m_rLight.boundingSphere().intersectsWith(minV,maxV))
						m_pBezierPatches->push_back(rFace.m_pBezierPatch);
				}
				
			}
		}

		{
			ion_uint32 numTriangles=0,indexOffset=0,currentIdx=0;

			m_pIndices->map(video::Map_Writeonly|video::Map_Discard);
			std::vector < std::vector < Q3BSP::Face* > >::iterator itflist=faces.begin();
			for (;itflist!=faces.end();++itflist) {
				std::vector < Q3BSP::Face* >& rFList=(*itflist);
				if (rFList.size()==0) continue;

				//ion_uint32 matidx=(ion_uint32)(itflist-faces.begin());
				ion_uint32 matidx=(*rFList.begin())->m_Surfaceindex;
				Renderjob *pJob=m_pActiveJobs[m_NumActiveJobs];
				++m_NumActiveJobs;

				numTriangles=0;

				for (std::vector < Q3BSP::Face* >::iterator itface=rFList.begin();itface!=rFList.end();++itface) {
					Q3BSP::Face *pFace=(*itface);
					for (ion_int32 j=0;j<pFace->m_NumMeshverts;++j) {
						assert(currentIdx<m_pIndices->capacity());
						m_pIndices->index(currentIdx,
							m_rQ3BSP.m_pMeshverts[pFace->m_FirstMeshvert+j]+pFace->m_FirstVertex);
						++currentIdx;
					}
					numTriangles+=pFace->m_NumMeshverts/3;
				}

				pJob->primitivestype(video::Primitives_Triangles);
				pJob->indexstream(m_pIndices);
				pJob->vertexstream(*(m_rQ3BSP.m_pVertices));
				pJob->shader(0);
				pJob->rendertargetCamera(0);
				pJob->propertytable(0);
				pJob->viewerDistance(0);
				pJob->firstElement(0);
				pJob->indexOffset(indexOffset);
				pJob->numElements(numTriangles);
				pJob->propertytable(&(m_rQ3BSP.m_pSurfaces[matidx].m_Material));
				indexOffset+=numTriangles*3;
			}
			m_pIndices->unmap();
		}
	}

	inline float angleBetweenVectors(const math::Vector3f& c,const math::Vector3f& p1,const math::Vector3f& p2) {
		math::Vector3f v1=p1-c,v2=p2-c;
		return acosf((v1*v2)/(v1.length()*v2.length()));
	}

	bool Q3BSP::BSPLight::sphereTriangleIntersection(const float radius,const math::Vector3f& center,
		const math::Vector3f& p1,const math::Vector3f& p2,const math::Vector3f& p3)
	{
		const math::Vector3f* points[3]={&p1,&p2,&p3};

		// First test if any point is within the radius
		{
			float rr=radius*radius;
			for (int i=0;i<3;++i) {
				float dist=(center-*(points[i])).lengthSquared();
				if (dist<rr) return true;
			}
		}


		// Note: n and D are plane coefficients and could be precalculated
		math::Vector3f v1=p3-p1;
		math::Vector3f v2=p2-p1;

		/*math::Vector3f n(
			v1.y*v2.z-v1.z*v2.y,
			v1.z*v2.x-v1.x*v2.z,
			v1.x*v2.y-v1.y*v2.x
			);*/
		math::Vector3f n=v1^v2;
		n.normalize();

		float D=p1*n;
		

		float d=n*center-D;
		if (d>radius) return false;
		math::Vector3f cp=center-n*d;

		float theta=angleBetweenVectors(cp,p1,p2)+angleBetweenVectors(cp,p2,p3)+angleBetweenVectors(cp,p3,p1);

		// if its 2*pi, then the projected center is in the triangle
		// -> polygon is in sphere
		return (theta-2*3.1415926535897932384626433832795<=0.0001);
	}

	void Q3BSP::BSPLight::clearLeafList()
	{
		for (std::vector < Leaf* >::iterator it=m_pLeaves->begin();it!=m_pLeaves->end();++it) {
			Leaf* pLeaf=(*it);
			std::set < ion_uint32 >::iterator itlight=pLeaf->m_pLights->find(m_Lightindex);
			if (itlight!=pLeaf->m_pLights->end()) pLeaf->m_pLights->erase(itlight);
		}
		m_pLeaves->clear();
		m_pBezierPatches->clear();
	}

	void Q3BSP::BSPLight::addLightToLeaves(const ion_int32 nodeNum,ion_uint32& indicesCounter,std::vector < bool >& alreadyVisible/*,
		std::set<ion_uint32> &renderableIndices*/)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_rQ3BSP.m_pLeaves-(nodeNum+1);

			math::Vector3f minV((float)(pLeaf->m_Mincoord[0]),(float)(pLeaf->m_Mincoord[1]),(float)(pLeaf->m_Mincoord[2]));
			math::Vector3f maxV((float)(pLeaf->m_Maxcoord[0]),(float)(pLeaf->m_Maxcoord[1]),(float)(pLeaf->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			/*for (std::set<ion_uint32>::iterator itridx=pLeaf->m_pRenderables->begin();itridx!=pLeaf->m_pRenderables->end();++itridx) {
				ion_uint32 ridx=(*itridx);
				Renderable &rRenderable=(*m_rQ3BSP.m_pRenderables)[ridx-1]->renderable();
				const math::SphereVolume &sphere=rRenderable.boundingSphere();

				float distSq1=(m_rLight.transform().position()-(rRenderable.transform().position()+sphere.center())).lengthSquared();
				float distSq2=m_rLight.range()+sphere.radius(); distSq2*=distSq2;

				if (distSq1<distSq2)
					renderableIndices.insert(ridx);
			}*/

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				m_pLeaves->push_back(pLeaf);
				pLeaf->m_pLights->insert(m_Lightindex);

				for (ion_int32 f=0;f<pLeaf->m_NumLeaffaces;++f) {
					int faceidx=m_rQ3BSP.m_pLeaffaces[f+pLeaf->m_Leafface];

					if ((faceidx<0) || (faceidx>=(int)(m_rQ3BSP.m_NumFaces))) continue;
					if (alreadyVisible[faceidx]) continue;
					alreadyVisible[faceidx]=true;

					Q3BSP::Face &rFace=m_rQ3BSP.m_pFaces[faceidx];

					// TODO: Support patches & billboards
					if ((rFace.m_Type==Q3BSP::Facetype_Polygon) || (rFace.m_Type==Q3BSP::Facetype_Mesh))
						indicesCounter+=rFace.m_NumMeshverts;
				}
			}
		} else {
			Q3BSP::Node *pNode=&(m_rQ3BSP.m_pNodes[nodeNum]);

			math::Vector3f minV((float)(pNode->m_Mincoord[0]),(float)(pNode->m_Mincoord[1]),(float)(pNode->m_Mincoord[2]));
			math::Vector3f maxV((float)(pNode->m_Maxcoord[0]),(float)(pNode->m_Maxcoord[1]),(float)(pNode->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				addLightToLeaves(pNode->m_FrontChild,indicesCounter,alreadyVisible/*,renderableIndices*/);
				addLightToLeaves(pNode->m_BackChild,indicesCounter,alreadyVisible/*,renderableIndices*/);
			}
		}
	}

	/*void Q3BSP::BSPLight::addLightToLeaves(const ion_int32 nodeNum)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_rQ3BSP.m_pLeaves-(nodeNum+1);

			math::Vector3f minV((float)(pLeaf->m_Mincoord[0]),(float)(pLeaf->m_Mincoord[1]),(float)(pLeaf->m_Mincoord[2]));
			math::Vector3f maxV((float)(pLeaf->m_Maxcoord[0]),(float)(pLeaf->m_Maxcoord[1]),(float)(pLeaf->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				m_pLeaves->push_back(pLeaf);
				pLeaf->m_pLights->insert(m_Lightindex);
			}
		} else {
			Q3BSP::Node *pNode=&(m_rQ3BSP.m_pNodes[nodeNum]);

			math::Vector3f minV((float)(pNode->m_Mincoord[0]),(float)(pNode->m_Mincoord[1]),(float)(pNode->m_Mincoord[2]));
			math::Vector3f maxV((float)(pNode->m_Maxcoord[0]),(float)(pNode->m_Maxcoord[1]),(float)(pNode->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				addLightToLeaves(pNode->m_FrontChild);
				addLightToLeaves(pNode->m_BackChild);
			}
		}
	}*/

	//////////////////////
	// Progress methods //
	//////////////////////

	Q3BSP::Progress::Progress()
	{
	}

	Q3BSP::Progress::~Progress()
	{
	}

	//////////////////////////////////
	// BSPVertex operator overloads //
	//////////////////////////////////

	Q3BSP::BSPVertex operator *(const Q3BSP::BSPVertex& a,const float f)
	{
		Q3BSP::BSPVertex v;
		v.m_Position=a.m_Position*f;
		v.m_Texcoord[0]=a.m_Texcoord[0]*f;
		v.m_Texcoord[1]=a.m_Texcoord[1]*f;
		v.m_Normal=a.m_Normal*f;
		v.m_Tangent=a.m_Tangent*f;
		v.m_Binormal=a.m_Binormal*f;
		for (int i=0;i<4;++i) v.m_ColorRGBA[i]=(int)( ((float)(a.m_ColorRGBA[i]))*f );

		return v;
	}

	Q3BSP::BSPVertex operator +(const Q3BSP::BSPVertex& a,const Q3BSP::BSPVertex& b)
	{
		Q3BSP::BSPVertex v;
		v.m_Position=a.m_Position+b.m_Position;
		v.m_Texcoord[0]=a.m_Texcoord[0]+b.m_Texcoord[0];
		v.m_Texcoord[1]=a.m_Texcoord[1]+b.m_Texcoord[1];
		v.m_Normal=a.m_Normal+b.m_Normal;
		v.m_Tangent=a.m_Tangent+b.m_Tangent;
		v.m_Binormal=a.m_Binormal+b.m_Binormal;
		for (int i=0;i<4;++i) v.m_ColorRGBA[i]=a.m_ColorRGBA[i]+b.m_ColorRGBA[i];

		return v;
	}



	//////////////////
	// Leaf methods //
	//////////////////

	Q3BSP::Leaf::Leaf()
	{
		m_pLights=new std::set < ion_uint32 >;
		m_pRenderables=new std::set < ion_uint32 >;
	}

	Q3BSP::Leaf::~Leaf()
	{
		delete m_pRenderables;
		delete m_pLights;
	}





	///////////////////
	// Model methods //
	///////////////////

	bool Q3BSP::BSPModel::isValid() const
	{
		return m_pIndices!=0;
	}

	const char* Q3BSP::BSPModel::type() const
	{
		return "BSPModel";
	}

	void Q3BSP::BSPModel::respawn(base::Streamable& source)
	{
	}

	void Q3BSP::BSPModel::serialize(base::Streamable& dest)
	{
	}

	ion_uint32 Q3BSP::BSPModel::numJobs() const
	{
		return m_NumRenderjobs;
	}

	Renderjob* Q3BSP::BSPModel::job(const ion_uint32 jobnr)
	{
		return m_pRenderjobs[jobnr];
	}

	void Q3BSP::BSPModel::restoreVideoResources()
	{
	}

	Q3BSP::BSPModel::BSPModel():Renderable("BSPModel"),m_pQ3BSP(0),m_pIndices(0),m_pRenderjobs(0),m_NumRenderjobs(0)
	{
	}

	Q3BSP::BSPModel::~BSPModel()
	{
		if (m_pIndices) delete m_pIndices;
		if (m_pRenderjobs) {
			for (ion_uint32 i=0;i<m_NumRenderjobs;++i) {
				delete m_pRenderjobs[i];
			}
			delete [] m_pRenderjobs;
		}
	}

	const math::SphereVolume& Q3BSP::BSPModel::boundingSphere() const
	{
		return m_BoundingSphere;
	}

	void Q3BSP::BSPModel::init(Q3BSP &rQ3BSP)
	{
		m_pQ3BSP=&rQ3BSP;
		renderer(*(rQ3BSP.renderer()));

		std::vector<std::vector< const Q3BSP::Face* > > faces;
		faces.resize(rQ3BSP.m_NumSurfaces);

		m_NumRenderjobs=0;
		ion_uint32 numtris=0;
		{
			std::set <ion_uint32> tmpsss;
			for (ion_int32 i=0;i<m_NumFaces;++i) {
				const Q3BSP::Face &rFace=rQ3BSP.faces()[i+m_FirstFace];
				if ((rFace.m_Type!=Q3BSP::Facetype_Polygon) && (rFace.m_Type!=Q3BSP::Facetype_Mesh)) continue;

				numtris+=rFace.m_NumMeshverts/3;
				if ((rFace.m_Surfaceindex>=0) && (rFace.m_Surfaceindex<(ion_int32)(rQ3BSP.m_NumSurfaces))) {
					faces[rFace.m_Surfaceindex].push_back(&rFace);
					tmpsss.insert(rFace.m_Surfaceindex);
				}
			}

			m_NumRenderjobs=(ion_uint32)(tmpsss.size());
			m_pRenderjobs=new Renderjob*[m_NumRenderjobs];

			for (ion_uint32 r=0;r<m_NumRenderjobs;++r) {
				m_pRenderjobs[r]=new Renderjob(*this);
			}
		}

		//(const ion_uint32 numIndices,const Indexformat format,const ion_uint32 flags,const Memorypool pool)=0;
		m_pIndices=renderer()->videodevice()->createIndexstream(numtris*3,
			(rQ3BSP.numBSPVertices()<65536) ? video::Indexformat_16bit : video::Indexformat_32bit,
			video::Streamflags_Writeonly,video::Mempool_Managed);

		/*m_pIndices->map(video::Map_Writeonly);
		video::Indexiterator idx(*m_pIndices);
		for (ion_int32 i=0;i<m_NumFaces;++i) {
			const Q3BSP::Face &rFace=rQ3BSP.faces()[i+m_FirstFace];
			for (ion_int32 vtx=0;vtx<rFace.m_NumMeshverts;++vtx) {
				idx=rQ3BSP.meshverts()[vtx+rFace.m_FirstMeshvert];
				++idx;
			}
		}
		m_pIndices->unmap();*/

		math::Vector3f minc,maxc;

		{
			ion_uint32 numTriangles=0,indexOffset=0,currentIdx=0;
			bool firstvertex=true;

			Renderjob **pJobs=m_pRenderjobs;

			m_pIndices->map(video::Map_Writeonly);
			std::vector < std::vector < const Q3BSP::Face* > >::iterator itflist=faces.begin();
			for (;itflist!=faces.end();++itflist) {
				std::vector < const Q3BSP::Face* >& rFList=(*itflist);
				if (rFList.size()==0) continue;

				//ion_uint32 matidx=(ion_uint32)(itflist-faces.begin());
				ion_uint32 matidx=(*rFList.begin())->m_Surfaceindex;
				Renderjob *pJob=*pJobs;
				++pJobs;

				numTriangles=0;

				for (std::vector < const Q3BSP::Face* >::iterator itface=rFList.begin();itface!=rFList.end();++itface) {
					const Q3BSP::Face *pFace=(*itface);
					for (ion_int32 j=0;j<pFace->m_NumMeshverts;++j) {
						assert(currentIdx<m_pIndices->capacity());
						ion_uint32 meshvertidx=rQ3BSP.m_pMeshverts[pFace->m_FirstMeshvert+j]+pFace->m_FirstVertex;
						m_pIndices->index(currentIdx,meshvertidx);
						++currentIdx;

						math::Vector3f &vv=rQ3BSP.m_pBSPVertices[meshvertidx].m_Position;

						if (firstvertex) {
							minc=maxc=vv;
							firstvertex=false;
						} else {
							if (minc.x()>vv.x()) minc.x()=vv.x();
							if (minc.y()>vv.y()) minc.y()=vv.y();
							if (minc.z()>vv.z()) minc.z()=vv.z();

							if (maxc.x()<vv.x()) maxc.x()=vv.x();
							if (maxc.y()<vv.y()) maxc.y()=vv.y();
							if (maxc.z()<vv.z()) maxc.z()=vv.z();
						}
					}
					numTriangles+=pFace->m_NumMeshverts/3;
				}

				pJob->primitivestype(video::Primitives_Triangles);
				pJob->indexstream(m_pIndices);
				pJob->vertexstream(*(rQ3BSP.m_pVertices));
				pJob->shader(0);
				pJob->rendertargetCamera(0);
				pJob->propertytable(0);
				pJob->viewerDistance(0);
				pJob->firstElement(0);
				pJob->indexOffset(indexOffset);
				pJob->numElements(numTriangles);
				pJob->propertytable(&(rQ3BSP.m_pSurfaces[matidx].m_Material));
				indexOffset+=numTriangles*3;
			}
			m_pIndices->unmap();
		}

		m_BoundingSphere.center((maxc+minc)*0.5f);
		m_BoundingSphere.radius((maxc-minc).length()*0.5f);

		// TODO: remove this when model transformations work properly
		//m_pBSPRenderable=rQ3BSP.addRenderable(*this);
	}

	void Q3BSP::BSPModel::rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer)
	{
	}







	struct Q3BSP::BezierList {
		std::vector< BezierPatch* > m_Patches;
	};




	///////////////////////////////
	// MAIN Q3BSP IMPLEMENTATION //
	///////////////////////////////


	Q3BSP::Q3BSP(const base::String& bspname,Renderer& rRenderer,Progress* pProgress,const float mapscale):Renderable(bspname),
		m_pSurfaces(0),m_NumSurfaces(0),m_pPlanes(0),m_pNodes(0),m_pLeaves(0),m_pLeaffaces(0),m_pLeafbrushes(0),
		m_pBSPModels(0),m_pBrushes(0),m_pBrushsides(0),m_pBSPVertices(0),m_pMeshverts(0),m_pEffects(0),m_pFaces(0),
		m_pVisdata(0),m_Mapscale(mapscale),m_IsValid(false),m_UsesFrustumCulling(true),m_pBezierList(0),m_pIndices(0),
		m_pVertices(0),m_pOpaqueRenderjobs(0)
	{
		m_AmbientColor.r(0.9f);
		m_AmbientColor.g(0.9f);
		m_AmbientColor.b(0.9f);
		m_AmbientColor.a(1);

		m_pBezierList=new BezierList;
		m_pLights=new std::vector < BSPLight* >;
		m_pRenderables=new std::vector < BSPRenderable* >;

		renderer(rRenderer);

		load(bspname,pProgress);
	}

	Q3BSP::~Q3BSP()
	{
		cleanup();
		delete m_pBezierList;
		delete m_pRenderables;
		delete m_pLights;
	}

	void Q3BSP::ambientColor(const video::Color& color)
	{
		m_AmbientColor=color;

		for (ion_uint32 surface=0;surface<m_NumSurfaces;++surface) {
			m_pSurfaces[surface].m_Material.addColorRGB("ambientColor",m_AmbientColor.r(),m_AmbientColor.g(),m_AmbientColor.b());
		}
	}

	const video::Color& Q3BSP::ambientColor() const
	{
		return m_AmbientColor;
	}

	void Q3BSP::cleanup()
	{
		{
			for (std::vector < BSPLight* >::iterator it=m_pLights->begin();it!=m_pLights->end();++it)
				delete (*it);
			m_pLights->clear();
		}

		{
			for (std::vector < BSPRenderable* >::iterator it=m_pRenderables->begin();it!=m_pRenderables->end();++it)
				delete (*it);
			m_pRenderables->clear();
		}

		if (m_pOpaqueRenderjobs!=0) {
			for (ion_uint32 i=0;i<m_NumSurfaces;++i) {
				delete m_pOpaqueRenderjobs[i];
			}
			delete [] m_pOpaqueRenderjobs;
			m_pOpaqueRenderjobs=0;
		}

		if (m_pSurfaces!=0) {
			for (ion_uint32 i=0;i<m_NumSurfaces;++i) {
				video::Texture2D *pTex=0;
				m_pSurfaces[i].m_Material.fetch2DTexture("diffuseTexture",pTex);
				if (pTex!=0) delete pTex; pTex=0;
				m_pSurfaces[i].m_Material.fetch2DTexture("normalMap",pTex);
				if (pTex!=0) delete pTex; pTex=0;
			}
		}

		if (m_pFaces!=0) {
			for (ion_uint32 i=0;i<m_NumFaces;++i) {
				if (m_pFaces[i].m_pBezierPatch!=0) {
					delete m_pFaces[i].m_pBezierPatch->m_pIndexstream;
					delete m_pFaces[i].m_pBezierPatch->m_pVertexstream;
					delete m_pFaces[i].m_pBezierPatch->m_pRenderjob;
					delete m_pFaces[i].m_pBezierPatch;
				}
			}
		}

#define CLEANUPARRAY(arrayvar,countvar) { if (arrayvar!=0) delete [] (arrayvar); (arrayvar)=0; (countvar)=0; }

		CLEANUPARRAY(m_pSurfaces,m_NumSurfaces);
		CLEANUPARRAY(m_pPlanes,m_NumPlanes);
		CLEANUPARRAY(m_pNodes,m_NumNodes);
		CLEANUPARRAY(m_pLeaves,m_NumLeaves);
		CLEANUPARRAY(m_pLeaffaces,m_NumLeaffaces);
		CLEANUPARRAY(m_pLeafbrushes,m_NumLeafbrushes);
		CLEANUPARRAY(m_pBSPModels,m_NumBSPModels);
		CLEANUPARRAY(m_pBrushes,m_NumBrushes);
		CLEANUPARRAY(m_pBrushsides,m_NumBrushsides);
		CLEANUPARRAY(m_pBSPVertices,m_NumBSPVertices);
		CLEANUPARRAY(m_pMeshverts,m_NumMeshverts);
		CLEANUPARRAY(m_pEffects,m_NumEffects);
		CLEANUPARRAY(m_pFaces,m_NumFaces);
		CLEANUPARRAY(m_pVisdata,m_NumVisVectors);

#undef CLEANUPARRAY

		if (m_pVertices!=0) delete m_pVertices; m_pVertices=0;
		if (m_pIndices!=0) delete m_pIndices; m_pIndices=0;

		m_IsValid=false;
	}

	void Q3BSP::useFrustumCulling(const bool use)
	{
		m_UsesFrustumCulling=use;
	}

	bool Q3BSP::usesFrustumCulling() const
	{
		return m_UsesFrustumCulling;
	}

	Q3BSP::BSPLight* Q3BSP::addLight(Light &rLight)
	{
		if (rLight.lighttype()!=video::Lighttype_Point) return 0; // TODO: support spotlights & dirlights

		BSPLight *pBSPLight=new BSPLight(*this,rLight,(ion_uint32)(m_pLights->size()));
		m_pLights->push_back(pBSPLight);
		return pBSPLight;
	}

	void Q3BSP::removeLight(BSPLight &rLight)
	{
		std::vector<BSPLight*>::iterator itlight=std::find(m_pLights->begin(),m_pLights->end(),&rLight);
		if (itlight!=m_pLights->end()) {
			ion_uint32 lightindex=(ion_uint32)(itlight-m_pLights->begin());
			removeLightindexFromLeaves(0,lightindex);

			std::vector<BSPLight*>::iterator itlight2=m_pLights->begin();
			for (;itlight2!=m_pLights->end();++itlight2) {
				if (itlight2==itlight) continue;

				BSPLight* pLight2=(*itlight2);
				if (pLight2->m_Lightindex>lightindex)
					pLight2->m_Lightindex--;
			}

			delete (*itlight);
			m_pLights->erase(itlight);

			rLight.light().transformAlterationFunctor(0);
		}
	}

	void Q3BSP::removeLight(Light &rLight)
	{
		std::vector<BSPLight*>::iterator itlight=m_pLights->begin();
		for (;itlight!=m_pLights->end();++itlight) {
			Light *pLight=&((*itlight)->light());
			if (pLight==&rLight) break;
		}

		if (itlight!=m_pLights->end()) {
			ion_uint32 lightindex=(ion_uint32)(itlight-m_pLights->begin());
			removeLightindexFromLeaves(0,lightindex);

			std::vector<BSPLight*>::iterator itlight2=m_pLights->begin();
			for (;itlight2!=m_pLights->end();++itlight2) {
				if (itlight2==itlight) continue;

				BSPLight* pLight2=(*itlight2);
				if (pLight2->m_Lightindex>lightindex)
					pLight2->m_Lightindex--;
			}

			delete (*itlight);
			m_pLights->erase(itlight);

			rLight.transformAlterationFunctor(0);
		}
	}

	void Q3BSP::updateLightGeometry()
	{
		std::vector<BSPLight*>::iterator itlight=m_pLights->begin();
		for (;itlight!=m_pLights->end();++itlight) {
			BSPLight *pBSPLight=(*itlight);
			pBSPLight->update();
		}
	}

	Q3BSP::BSPRenderable* Q3BSP::addRenderable(Renderable &rRenderable)
	{
		BSPRenderable *pBSPRenderable=new BSPRenderable(*this,rRenderable,(ion_uint32)(m_pRenderables->size()+1)); // +1 because #0 is the BSP itself
		m_pRenderables->push_back(pBSPRenderable);

		return 0;
	}

	void Q3BSP::removeRenderable(BSPRenderable &rRenderable)
	{
		std::vector<BSPRenderable*>::iterator itrenderable=std::find(m_pRenderables->begin(),m_pRenderables->end(),&rRenderable);
		if (itrenderable!=m_pRenderables->end()) {
			ion_uint32 renderableindex=(ion_uint32)(itrenderable-m_pRenderables->begin())+1; // +1 because #0 is the BSP itself
			removeRenderableindexFromLeaves(0,renderableindex);

			std::vector<BSPRenderable*>::iterator itrenderable2=m_pRenderables->begin();
			for (;itrenderable2!=m_pRenderables->end();++itrenderable2) {
				if (itrenderable2==itrenderable) continue;

				BSPRenderable* pRenderable2=(*itrenderable2);
				if (pRenderable2->m_Renderableindex>renderableindex)
					pRenderable2->m_Renderableindex--;
			}

			delete (*itrenderable);
			m_pRenderables->erase(itrenderable);

			rRenderable.renderable().transformAlterationFunctor(0);
		}
	}

	void Q3BSP::removeRenderable(Renderable &rRenderable)
	{
		std::vector<BSPRenderable*>::iterator itrenderable=m_pRenderables->begin();
		for (;itrenderable!=m_pRenderables->end();++itrenderable) {
			Renderable *pRenderable=&((*itrenderable)->renderable());
			if (pRenderable==&rRenderable) break;
		}

		if (itrenderable!=m_pRenderables->end()) {
			ion_uint32 renderableindex=(ion_uint32)(itrenderable-m_pRenderables->begin())+1; // +1 because #0 is the BSP itself
			removeRenderableindexFromLeaves(0,renderableindex);

			std::vector<BSPRenderable*>::iterator itrenderable2=m_pRenderables->begin();
			for (;itrenderable2!=m_pRenderables->end();++itrenderable2) {
				if (itrenderable2==itrenderable) continue;

				BSPRenderable* pRenderable2=(*itrenderable2);
				if (pRenderable2->m_Renderableindex>renderableindex)
					pRenderable2->m_Renderableindex--;
			}

			delete (*itrenderable);
			m_pRenderables->erase(itrenderable);

			rRenderable.transformAlterationFunctor(0);
		}
	}

	void Q3BSP::getLeavesForNode(int nodenum, std::vector <int> &leaves)
	{
		const Node *node = m_pNodes + nodenum;

		if(nodenum < 0)
		{
			leaves.push_back(-(nodenum + 1));
			return;
		}

		getLeavesForNode(node->m_FrontChild, leaves);
		getLeavesForNode(node->m_BackChild, leaves);
	}

	void Q3BSP::determineVisibity(Renderquery& query)
	{
		if (!m_IsValid) return;

		query.lightlist().clear();
		query.lightIndicesList().clear();
		query.renderablelist().clear();
		//Renderable::lightlist().clear();

		query.renderablelist().addRenderable(*this);
		query.renderableIndicesList().addIndex(0);

		/*{
			ion_uint32 l=0;
			for (std::vector < BSPLight* >::iterator it=m_pLights->begin();it!=m_pLights->end();++it) {
				BSPLight *pBSPLight=(*it);
				Light &rLight=(pBSPLight->m_rLight);

				bool add=(rLight.lighttype()==video::Lighttype_Directional);
				if (!add) add=(query.camera()->frustum().checkSphere(rLight.transform().position(),rLight.range())!=math::Frustum::NotInFrustum);

				if (add)
				{
					query.lightlist().addLight(rLight);
					query.lightIndicesList().addIndex(l);
					//rLight.renderablelist().addRenderable(*this);
				}

				++l;
			}
		}*/


		std::vector < std::vector < const Face* > > faces;
		std::vector < bool > alreadyVisible;
		std::vector < bool > lightAdded,renderableAdded;
		faces.resize(m_NumSurfaces);
		alreadyVisible.resize(m_NumFaces);
		lightAdded.resize(m_pLights->size());
		renderableAdded.resize(m_pRenderables->size());

		m_pBezierList->m_Patches.clear();

		ion_uint32 i=0;

		const Camera &camera=*(query.camera());

		const Leaf& rVisLeaf=m_pLeaves[findLeaf(camera.transform().position())];

		std::vector <int> r_leaves;
		getLeavesForNode(0, r_leaves);

		int li;

		for(li = 0; li < (int)(r_leaves.size()); li++)
		{
			i = r_leaves[li];
//		for (i=0;i<m_NumLeaves;++i) {
			const Leaf& rLeaf=m_pLeaves[i];

			// Check for visibility
			if ((rLeaf.m_ClusterNr<0) || (rLeaf.m_Leafface<0)) continue;
			if ((m_pVisdata!=0) && (rVisLeaf.m_ClusterNr!=rLeaf.m_ClusterNr))
				if (!isClusterVisible(rVisLeaf.m_ClusterNr,rLeaf.m_ClusterNr)) continue;

			if (m_UsesFrustumCulling) {
				if (math::Frustum::NotInFrustum==camera.frustum().checkAABB(
					math::Vector3f((float)(rLeaf.m_Mincoord[0]),(float)(rLeaf.m_Mincoord[1]),(float)(rLeaf.m_Mincoord[2]) ),
					math::Vector3f((float)(rLeaf.m_Maxcoord[0]),(float)(rLeaf.m_Maxcoord[1]),(float)(rLeaf.m_Maxcoord[2]) ))) continue;
			}

			// Add lights
			{
				for (std::set<ion_uint32>::iterator itlidx=rLeaf.m_pLights->begin();itlidx!=rLeaf.m_pLights->end();++itlidx) {
					ion_uint32 lidx=(*itlidx);
					if (!lightAdded[lidx]) {
						lightAdded[lidx]=true;
						Light &rLight=(*m_pLights)[lidx]->m_rLight;

						bool add=(rLight.lighttype()==video::Lighttype_Directional) || !m_UsesFrustumCulling;
						if (!add)
							add=(query.camera()->frustum().checkSphere(rLight.transform().position(),rLight.range())!=math::Frustum::NotInFrustum);

						if (add) {
							query.lightIndicesList().addIndex(lidx);
							query.lightlist().addLight(rLight);
						}

					}
				}
			}

			// Add renderables
			{
				for (std::set<ion_uint32>::iterator itridx=rLeaf.m_pRenderables->begin();itridx!=rLeaf.m_pRenderables->end();++itridx) {
					ion_uint32 ridx=(*itridx);
					if (!renderableAdded[ridx]) {
						renderableAdded[ridx]=true;
						Renderable &rRenderable=(*m_pRenderables)[ridx-1]->m_rRenderable; // -1 because #0 is the BSP itself
/*
					const math::SphereVolume &sphere=rRenderable.boundingSphere();
					math::Vector3f pos=rRenderable.transform().position()+sphere.center();
					if (query.camera()->frustum().checkSphere(pos,sphere.radius()))
*/
						const math::SphereVolume &sphere=rRenderable.boundingSphere();
						math::Vector3f pos=rRenderable.transform().position()+sphere.center();

						bool add=!m_UsesFrustumCulling;
						if (!add)
							add=(query.camera()->frustum().checkSphere(pos,sphere.radius())!=math::Frustum::NotInFrustum);

						if (add) {
							query.renderableIndicesList().addIndex(ridx);
							query.renderablelist().addRenderable(rRenderable);
						}

					}
				}
			}

			// Add leaf faces
			for (ion_int32 j=0;j<rLeaf.m_NumLeaffaces;++j) {
				if ((j+rLeaf.m_Leafface)>=(ion_int32)m_NumLeaffaces) continue;
				int faceidx=m_pLeaffaces[j+rLeaf.m_Leafface];
				if ((faceidx<0) || (faceidx>=(int)m_NumFaces)) continue;
				if (alreadyVisible[faceidx]) continue;
				alreadyVisible[faceidx]=true;

				const Face& rFace=m_pFaces[faceidx];
				if ((rFace.m_Surfaceindex<0) || (rFace.m_Surfaceindex>=(ion_int32)m_NumSurfaces)) continue;
				if (m_pSurfaces[rFace.m_Surfaceindex].m_IsSky) continue;

				switch (rFace.m_Type) {
					case Facetype_Polygon:
					case Facetype_Mesh:
						faces[rFace.m_Surfaceindex].push_back(&rFace);
						break;
					case Facetype_Patch: {
						if (!m_UsesFrustumCulling || math::Frustum::NotInFrustum!=camera.frustum().checkAABB(rFace.m_pBezierPatch->m_Mincoord,rFace.m_pBezierPatch->m_Maxcoord))
							m_pBezierList->m_Patches.push_back(rFace.m_pBezierPatch);
					} break;
					default:break;
				}
			}
		}


		// Add renderables to lights
		// This should be done in the PVS, but as a temporary measure, it is done here
		{
			for (ion_uint32 lightnr=0;lightnr<query.lightlist().numLights();++lightnr) {
				Light *pLight=query.lightlist().light(lightnr);
				ion_uint32 lidx=query.lightIndicesList().index(lightnr);

				pLight->renderablelist().clear();
				pLight->renderableIndicesList().clear();
				pLight->renderablelist().addRenderable(*this);
				pLight->renderableIndicesList().addIndex(0);


				for (ion_uint32 renderablenr=0;renderablenr<query.renderablelist().numRenderables();++renderablenr) {
					Renderable *pRenderable=query.renderablelist().renderable(renderablenr);

					float distSq1=(pLight->transform().position()-(pRenderable->transform().position()+pRenderable->boundingSphere().center())).lengthSquared();
					float distSq2=pLight->range()+pRenderable->boundingSphere().radius(); distSq2*=distSq2;

					if (distSq1<distSq2) {
						ion_uint32 ridx=query.renderableIndicesList().index(renderablenr);

						pLight->renderablelist().addRenderable(*pRenderable);
						pLight->renderableIndicesList().addIndex(ridx);
					}

				}
			}
		}



		// Prepare the render jobs

		m_pIndices->map(video::Map_Writeonly|video::Map_Discard);
		video::Indexiterator idx(*m_pIndices);
		ion_uint32 indexOffset=0;
		m_NumVisibleTriangles=0;
		m_NumOpaqueRenderjobs=0;

		/*for (i=0;i<m_NumSurfaces;++i) {
			ion_uint32 numTriangles=0;
			const std::vector < const Face* >& f=faces[i];

			for (std::vector < const Face* >::const_iterator it=f.begin();it!=f.end();++it) {
				const Face& rFace=*(*it);

				switch (rFace.m_Type) {
					case Facetype_Polygon: {
						for (ion_int32 j=0;j<rFace.m_NumMeshverts;++j) {
							idx=m_pMeshverts[rFace.m_FirstMeshvert+j]+rFace.m_FirstVertex; ++idx;
						}
						numTriangles+=rFace.m_NumMeshverts/3;
					} break;
					case Facetype_Mesh: {
						for (ion_int32 j=0;j<rFace.m_NumMeshverts;++j) {
							idx=m_pMeshverts[rFace.m_FirstMeshvert+j]+rFace.m_FirstVertex; ++idx;
						}
						numTriangles+=rFace.m_NumMeshverts/3;
					} break;
					default:break;
				}
			}

			m_pOpaqueRenderjobs[i]->primitivestype(video::Primitives_Triangles);
			m_pOpaqueRenderjobs[i]->indexstream(m_pIndices);
			m_pOpaqueRenderjobs[i]->vertexstream(*m_pVertices);
			m_pOpaqueRenderjobs[i]->shader(0);
			m_pOpaqueRenderjobs[i]->rendertargetCamera(0);
			m_pOpaqueRenderjobs[i]->propertytable(0);
			m_pOpaqueRenderjobs[i]->viewerDistance(0);
			m_pOpaqueRenderjobs[i]->firstElement(0);
			m_pOpaqueRenderjobs[i]->indexOffset(indexOffset);
			m_pOpaqueRenderjobs[i]->numElements(numTriangles);
			m_pOpaqueRenderjobs[i]->propertytable(&(m_pSurfaces[i].m_Material));
			indexOffset+=numTriangles*3;
			m_NumVisibleTriangles+=numTriangles;
		}*/

		{
			ion_uint32 numTriangles=0,indexOffset=0,currentIdx=0;
			Renderjob **pJobs=m_pOpaqueRenderjobs;

			std::vector < std::vector < const Q3BSP::Face* > >::iterator itflist=faces.begin();
			for (;itflist!=faces.end();++itflist) {
				std::vector < const Q3BSP::Face* >& rFList=(*itflist);
				if (rFList.size()==0) continue;
				ion_uint32 matidx=(*rFList.begin())->m_Surfaceindex;

				Renderjob *pJob=*pJobs;
				++pJobs;

				numTriangles=0;

				for (std::vector < const Q3BSP::Face* >::iterator itface=rFList.begin();itface!=rFList.end();++itface) {
					const Q3BSP::Face *pFace=(*itface);
					for (ion_int32 j=0;j<pFace->m_NumMeshverts;++j) {
						assert(currentIdx<m_pIndices->capacity());
						m_pIndices->index(currentIdx,m_pMeshverts[pFace->m_FirstMeshvert+j]+pFace->m_FirstVertex);
						++currentIdx;
					}
					numTriangles+=pFace->m_NumMeshverts/3;
				}

				pJob->primitivestype(video::Primitives_Triangles);
				pJob->indexstream(m_pIndices);
				pJob->vertexstream(*m_pVertices);
				pJob->shader(0);
				pJob->rendertargetCamera(0);
				pJob->propertytable(0);
				pJob->viewerDistance(0);
				pJob->firstElement(0);
				pJob->indexOffset(indexOffset);
				pJob->numElements(numTriangles);
				pJob->propertytable(&(m_pSurfaces[matidx].m_Material));
				indexOffset+=numTriangles*3;

				++m_NumOpaqueRenderjobs;
			}
		}

		m_pIndices->unmap();

	}

	void Q3BSP::findInRadius(Renderquery& query,const math::Vector3f point,const float radius)
	{
	}

	void Q3BSP::findNearest(Renderquery& query,const math::Vector3f point,const ion_uint32 maxNodes)
	{
	}

	ion_uint32 Q3BSP::numJobs() const
	{
		return m_NumOpaqueRenderjobs+(ion_uint32)(m_pBezierList->m_Patches.size());
	}

	Renderjob* Q3BSP::job(const ion_uint32 jobnr)
	{
		/*if (jobnr<m_NumSurfaces)
			return m_pOpaqueRenderjobs[jobnr];
		else {
			BezierPatch *pPatch=m_pBezierList->m_Patches[jobnr-m_NumSurfaces];
			return pPatch->m_pRenderjob;
		}*/
		if (jobnr<m_NumOpaqueRenderjobs)
			return m_pOpaqueRenderjobs[jobnr];
		else {
			BezierPatch *pPatch=m_pBezierList->m_Patches[jobnr-m_NumOpaqueRenderjobs];
			return pPatch->m_pRenderjob;
		}
	}

	// BUG: These methods should return BSPLight->job values

	ion_uint32 Q3BSP::numLightingJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return (lightnr<m_pLights->size()) ? (*m_pLights)[lightnr]->numLightingjobs() : 0;
	}

	Renderjob* Q3BSP::lightingjob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return (lightnr<m_pLights->size()) ? (*m_pLights)[lightnr]->lightingjob(jobnr) : 0;
	}

	ion_uint32 Q3BSP::numPointlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return numLightingJobs(lightnr,rLight);
	}

	Renderjob* Q3BSP::pointlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return lightingjob(lightnr,rLight,jobnr);
	}

	ion_uint32 Q3BSP::numDirlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return 0;
	}

	Renderjob* Q3BSP::dirlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return 0;
	}

	ion_uint32 Q3BSP::numSpecularlightJobs(const ion_uint32 lightnr,const Light& rLight) const
	{
		return 0;
	}

	Renderjob* Q3BSP::specularlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr)
	{
		return 0;
	}

	void Q3BSP::update(const Camera& camera,const Updatemode mode)
	{
		if (!m_IsValid) return;

		// TODO: Update lights here if necessary
	}

	void Q3BSP::restoreVideoResources()
	{
	}

	const char* Q3BSP::type() const
	{
		return "Q3BSP";
	}

	void Q3BSP::respawn(base::Streamable& source)
	{
	}

	void Q3BSP::serialize(base::Streamable& dest)
	{
	}

	bool Q3BSP::isValid() const
	{
		return m_IsValid;
	}




	////// Q3BSP world data accessors

	const base::String& Q3BSP::entities() const { return m_Entities; }

	const Q3BSP::Surface* Q3BSP::surfaces() const { return m_pSurfaces; }
	ion_uint32 Q3BSP::numSurfaces() const { return m_NumSurfaces; }

	const math::Plane* Q3BSP::planes() const { return m_pPlanes; }
	ion_uint32 Q3BSP::numPlanes() const { return m_NumPlanes; }

	const Q3BSP::Node* Q3BSP::nodes() const { return m_pNodes; }
	ion_uint32 Q3BSP::numNodes() const { return m_NumNodes; }

	const Q3BSP::Leaf* Q3BSP::leaves() const { return m_pLeaves; }
	ion_uint32 Q3BSP::numLeaves() const { return m_NumLeaves; }

	const ion_int32* Q3BSP::leaffaces() const { return m_pLeaffaces; }
	ion_uint32 Q3BSP::numLeaffaces() const { return m_NumLeaffaces; }

	const ion_int32* Q3BSP::leafbrushes() const { return m_pLeafbrushes; }
	ion_uint32 Q3BSP::numLeafbrushes() const { return m_NumLeafbrushes; }

	const Q3BSP::BSPModel* Q3BSP::BSPModels() const { return m_pBSPModels; }
	ion_uint32 Q3BSP::numBSPModels() const { return m_NumBSPModels; }

	const Q3BSP::Brush* Q3BSP::brushes() const { return m_pBrushes; }
	ion_uint32 Q3BSP::numBrushes() const { return m_NumBrushes; }

	const Q3BSP::Brushside* Q3BSP::brushsides() const { return m_pBrushsides; }
	ion_uint32 Q3BSP::numBrushsides() const { return m_NumBrushsides; }

	const Q3BSP::BSPVertex* Q3BSP::BSPVertices() const { return m_pBSPVertices; }
	ion_uint32 Q3BSP::numBSPVertices() const { return m_NumBSPVertices; }

	const ion_int32* Q3BSP::meshverts() const { return m_pMeshverts; }
	ion_uint32 Q3BSP::numMeshverts() const { return m_NumMeshverts; }

	const Q3BSP::Effect* Q3BSP::effects() const { return m_pEffects; }
	ion_uint32 Q3BSP::numEffects() const { return m_NumEffects; }

	const Q3BSP::Face* Q3BSP::faces() const { return m_pFaces; }
	ion_uint32 Q3BSP::numFaces() const { return m_NumFaces; }

	const ion_uint8* Q3BSP::visdata() const { return m_pVisdata; }
	ion_uint32 Q3BSP::numVisVectors() const { return m_NumVisVectors; }
	ion_uint32 Q3BSP::visvectorSize() const { return m_VisvectorSize; }






	/*
	void Q3BSP::BSPLight::addLightToLeaves(const ion_int32 nodeNum)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_rQ3BSP.m_pLeaves-(nodeNum+1);

			math::Vector3f minV((float)(pLeaf->m_Mincoord[0]),(float)(pLeaf->m_Mincoord[1]),(float)(pLeaf->m_Mincoord[2]));
			math::Vector3f maxV((float)(pLeaf->m_Maxcoord[0]),(float)(pLeaf->m_Maxcoord[1]),(float)(pLeaf->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				m_pLeaves->push_back(pLeaf);
				pLeaf->m_pLights->insert(m_Lightindex);
			}
		} else {
			Q3BSP::Node *pNode=&(m_rQ3BSP.m_pNodes[nodeNum]);

			math::Vector3f minV((float)(pNode->m_Mincoord[0]),(float)(pNode->m_Mincoord[1]),(float)(pNode->m_Mincoord[2]));
			math::Vector3f maxV((float)(pNode->m_Maxcoord[0]),(float)(pNode->m_Maxcoord[1]),(float)(pNode->m_Maxcoord[2]));

			minV-=m_rLight.transform().position();
			maxV-=m_rLight.transform().position();

			if (m_rLight.boundingSphere().intersectsWith(minV,maxV)) {
				addLightToLeaves(pNode->m_FrontChild);
				addLightToLeaves(pNode->m_BackChild);
			}
		}
	}	
	*/


	void Q3BSP::removeLightindexFromLeaves(const ion_int32 nodeNum,const ion_uint32 lightindex)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_pLeaves-(nodeNum+1);

			std::set<ion_uint32>::iterator itlidx=pLeaf->m_pLights->begin(),itlidxmatch=pLeaf->m_pLights->end();
			while (itlidx!=pLeaf->m_pLights->end()) {
				if ((*itlidx)==lightindex) itlidxmatch=itlidx;
				else if ((*itlidx)>lightindex) (*itlidx)--;

				itlidx++;
			}

			if (itlidxmatch!=pLeaf->m_pLights->end())
				pLeaf->m_pLights->erase(itlidxmatch);
		} else {
			Q3BSP::Node *pNode=&(m_pNodes[nodeNum]);
			removeLightindexFromLeaves(pNode->m_FrontChild,lightindex);
			removeLightindexFromLeaves(pNode->m_BackChild,lightindex);
		}
	}

	void Q3BSP::removeRenderableindexFromLeaves(const ion_int32 nodeNum,const ion_uint32 renderableindex)
	{
		if (nodeNum<0) {
			Q3BSP::Leaf *pLeaf=m_pLeaves-(nodeNum+1);

			std::set<ion_uint32>::iterator itridx=pLeaf->m_pRenderables->begin(),itridxmatch=pLeaf->m_pRenderables->end();
			while (itridx!=pLeaf->m_pRenderables->end()) {
				if ((*itridx)==renderableindex) itridxmatch=itridx;
				else if ((*itridx)>renderableindex) (*itridx)--;

				itridx++;
			}

			if (itridxmatch!=pLeaf->m_pRenderables->end())
				pLeaf->m_pRenderables->erase(itridxmatch);
		} else {
			Q3BSP::Node *pNode=&(m_pNodes[nodeNum]);
			removeRenderableindexFromLeaves(pNode->m_FrontChild,renderableindex);
			removeRenderableindexFromLeaves(pNode->m_BackChild,renderableindex);
		}
	}

	int Q3BSP::findLeaf(const math::Vector3f& camPos) const
	{
		int index=0;

		while (index>=0) {
			const Node& node=m_pNodes[index];
			const math::Plane& plane=m_pPlanes[node.m_Planeindex];

			// Distance from point to a plane
			const float distance=(plane.normal()*camPos)-plane.distance();

			index=(distance>=0) ? node.m_FrontChild : node.m_BackChild;
		}

		return -index - 1;
	}

	bool Q3BSP::isClusterVisible(const int visCluster,const int testCluster) const
	{
		if ((m_pVisdata==0) || (visCluster<0) || (visCluster==testCluster)) return true;

		int i=(visCluster*m_VisvectorSize)+(testCluster>>3);
		ion_uint8 visSet=m_pVisdata[i];

		return (visSet&(1<<(testCluster&7)))!=0;
	}

	void Q3BSP::rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer)
	{
	}

	void Q3BSP::swizzle(math::Vector3f& v)
	{
		float temp=v.y();
		v.y()=v.z();
		v.z()=-temp;
		v.x()*=-1;
	}

	void Q3BSP::swizzle(ion_int32 *pIntcoords)
	{
		ion_int32 temp=pIntcoords[1];
		pIntcoords[1]=pIntcoords[2];
		pIntcoords[2]=-temp;
		pIntcoords[0]*=-1;
	}




	////// MAIN BSP LOAD METHOD

	void Q3BSP::load(const base::String& bspname,Progress* pProgress)
	{
		base::PhysFSFile bspfile(bspname,base::PhysFSFile::OpenRead);

		if (!bspfile.streamIsOk()) {
			base::log("Q3BSP::load()",base::Error) << "File " << bspname << " could not be opened\n";
			return;
		}

		char magicID[5]; magicID[4]=0;
		bspfile.read(magicID,4);

		if (strcmp(magicID,"IBSP")!=0) { // file is not an IBSP file
			base::log("Q3BSP::load()",base::Error) << "File " << bspname << " is not a valid IBSP file\n";
			return;
		}
		ion_int32 version; bspfile >> version;
		if (version!=0x2e) { // 0x2e = Quake 3 BSP
			base::log("Q3BSP::load()",base::Error) << "File " << bspname << " is not a valid Quake 3 BSP file\n";
			return;
		}

		cleanup();


		// Macro for conditioned progress report
#define REPORTPROGRESS(Stage) if (pProgress) (*pProgress)(Stage,progressStatus);

		float progressStatus=0.0f;

		REPORTPROGRESS(Progress::LoadingWorldData);

		int i ,ii;

		ion_uint32 numTriangles=0;

		// Read the direntries
		for (i=0;i<17;++i) {
			bspfile >> m_Direntries[i].m_Offset >> m_Direntries[i].m_Length;
		}

		static const float entitiesProgress=0.02f;
		static const float texturesProgress=0.15f; // 17
		static const float planesProgress=0.04f; // 21
		static const float nodesProgress=0.10f; // 31
		static const float leafsProgress=0.10f; // 41
		static const float leaffacesProgress=0.05f; // 46
		static const float leafbrushesProgress=0.05f; // 51
		static const float modelsProgress=0.05f; // 56
		static const float brushesProgress=0.05f; // 61
		static const float brushsidesProgress=0.02f; // 63
		static const float verticesProgress=0.10f; // 73
		static const float meshvertsProgress=0.05f; // 78
		static const float effectsProgress=0.02f; // 80
		static const float facesProgress=0.10f; // 90
		static const float lightmapsProgress=0.00f; // Not used for now
		static const float lightvolsProgress=0.00f; // Not used for now
		static const float visdataProgress=0.05f; // 95
		// The remaining percentage goes to patch tesselation

		ion_uint32 numTransparentSurfaces=0;

		// Read the lump contents
		std::vector < Face* > patchfaces; // Temp var for patches
		for (i=0;i<17;++i) {
			ion_uint32 lumplen=m_Direntries[i].m_Length;

			bspfile.seek(m_Direntries[i].m_Offset,base::LocalStreamable::SeekAbsolute);

			switch (i) {
				case Lumptype_Entities: {
					char *pStr=new char[lumplen+1]; pStr[lumplen]=0;
					bspfile.read(pStr,lumplen);
					m_Entities=pStr;
					delete [] pStr;
					progressStatus+=entitiesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Textures: {
					m_NumSurfaces=lumplen/(64+2*4);
					m_pSurfaces=new Surface[m_NumSurfaces];

					char surfname[65]; surfname[64]=0;
					for (ion_uint32 j=0;j<m_NumSurfaces;++j) {
						memset(surfname,0,64);
						bspfile.read(surfname,64);
						m_pSurfaces[j].m_Shadername=surfname;
						bspfile >> m_pSurfaces[j].m_Flags >> m_pSurfaces[j].m_Contents;

						m_pSurfaces[j].m_IsTransparent=false;
						m_pSurfaces[j].m_IsSky=(m_pSurfaces[j].m_Shadername.STLstring().find("sky")!=std::string::npos);

						//m_pSurfaces[j].m_pTexture=0;

						// First try if the given file is a shader
						// Then try .jpg, then .tga
						// TODO: shader

						bool foundShader=false;
						if (!m_pSurfaces[j].m_IsSky) {
							const char *ext[2]={".jpg",".tga"};
							video::Texture2D *pTex=0,*pNormalmap=0;
							for (ii=0;ii<2;++ii) {

								base::String fn(surfname); fn+=ext[ii];
								base::PhysFSFile texfile(fn,base::PhysFSFile::OpenRead);

								pTex=0;
								if (texfile.streamIsOk()) {
									pTex=video::load2DTex(
										*(renderer()->videodevice()),
										fn,
										texfile,
										//video::Pixelformat_ARGB8888,
										video::Pixelformat_Unknown,
										0,0,0);
								}

								if (pTex!=0) {
									m_pSurfaces[j].m_IsTransparent=video::pixelformatHasAlphachannel(pTex->pixelformat());
									foundShader=true; break;
								}
							}

							// Try to find normal- & gloss-maps (*_local.* & *_s.*)
							if (foundShader) {
								base::String nmapname=base::String(surfname)+"_local"+ext[ii];
								base::String glossname=base::String(surfname)+"_s"+ext[ii];
								base::PhysFSFile nmapfile(nmapname,base::PhysFSFile::OpenRead);
								base::PhysFSFile glossfile(glossname,base::PhysFSFile::OpenRead);

								video::SimplePicbuffer *pNMapPicbuf=video::load2DPic(nmapfile,video::Pixelformat_RGBA8888);
								video::SimplePicbuffer *pGlossPicbuf=video::load2DPic(glossfile,video::Pixelformat_RGB888);
/*
								if (!pNMapPicbuf) {
									base::PhysFSFile nmapfile2("textures/def_local.jpg",base::PhysFSFile::OpenRead);
									base::PhysFSFile glossfile2("textures/def_s.jpg",base::PhysFSFile::OpenRead);

									pNMapPicbuf=video::load2DPic(nmapfile2,video::Pixelformat_RGBA8888);
									pGlossPicbuf=video::load2DPic(glossfile2,video::Pixelformat_RGB888);

									nmapfile2.close();
									glossfile2.close();
								}
*/
								if (pNMapPicbuf) {

									if (pGlossPicbuf) {
										if ((pNMapPicbuf->width()!=pGlossPicbuf->width()) || (pNMapPicbuf->height()!=pGlossPicbuf->height())) {
											video::SimplePicbuffer *pTempbuf=new video::SimplePicbuffer(pNMapPicbuf->width(),pNMapPicbuf->height(),video::Pixelformat_RGB888);

											pTempbuf->transfer(*pGlossPicbuf);
											delete pGlossPicbuf;
											pGlossPicbuf=pTempbuf;
										}
									}

									for (ion_uint32 y=0;y<pNMapPicbuf->height();++y) {
										for (ion_uint32 x=0;x<pNMapPicbuf->width();++x) {
											ion_uint8 rgba[2][4];
											pNMapPicbuf->getpixelUint8(x,y,rgba[0][0],rgba[0][1],rgba[0][2],rgba[0][3]);

											if (pGlossPicbuf)
												pGlossPicbuf->getpixelUint8(x,y,rgba[1][0],rgba[1][1],rgba[1][2],rgba[1][3]);
											else rgba[1][0]=0xff;

											pNMapPicbuf->putpixelUint8(x,y,rgba[0][0],rgba[0][1],rgba[0][2],rgba[1][0]);
										}
									}

									pNormalmap=renderer()->videodevice()->create2DTexture(base::String(surfname)+"_nmap",pNMapPicbuf->width(),
										pNMapPicbuf->height(),0,0,video::Pixelformat_RGBA8888,video::Mempool_Managed);
									pNormalmap->generateMipmaps(*pNMapPicbuf);

									delete pNMapPicbuf;
								}

								if (pGlossPicbuf) delete pGlossPicbuf;
							}

							m_pSurfaces[j].m_Material.addColorRGB("ambientColor",
								m_AmbientColor.r(),m_AmbientColor.g(),m_AmbientColor.b());

							if (pTex!=0)
								m_pSurfaces[j].m_Material.add2DTexture("diffuseTexture",pTex);
							if (pNormalmap!=0)
								m_pSurfaces[j].m_Material.add2DTexture("normalMap",pNormalmap);
						}

						//if (!foundShader)
						//	base::log("Q3BSP::load()",base::Warning) << "could not find shader " << surfname << "\n";

						progressStatus+=texturesProgress*(float)(j)/(float)(m_NumSurfaces-1);
						REPORTPROGRESS(Progress::LoadingWorldData);
					}

					base::logstream() << m_NumSurfaces << " surfaces\n";
				} break;
				case Lumptype_Planes: {
					m_NumPlanes=lumplen/(4*4);
					m_pPlanes=new math::Plane[m_NumPlanes];

					for (ion_uint32 j=0;j<m_NumPlanes;++j) {
						float f[4];
						bspfile >> f[0] >> f[1] >> f[2] >> f[3];
						m_pPlanes[j].a(f[0]);
						m_pPlanes[j].b(f[1]);
						m_pPlanes[j].c(f[2]);
						m_pPlanes[j].d(f[3]*m_Mapscale);

						swizzle(m_pPlanes[j].normal());
					}

					progressStatus+=planesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Nodes: {
					m_NumNodes=lumplen/(4+4*2+4*3+4*3);
					m_pNodes=new Node[m_NumNodes];

					for (ion_uint32 j=0;j<m_NumNodes;++j) {
						bspfile >> m_pNodes[j].m_Planeindex;
						bspfile >> m_pNodes[j].m_FrontChild >> m_pNodes[j].m_BackChild;
						bspfile >> m_pNodes[j].m_Mincoord[0] >> m_pNodes[j].m_Mincoord[1] >> m_pNodes[j].m_Mincoord[2];
						bspfile >> m_pNodes[j].m_Maxcoord[0] >> m_pNodes[j].m_Maxcoord[1] >> m_pNodes[j].m_Maxcoord[2];

						swizzle(m_pNodes[j].m_Mincoord);
						swizzle(m_pNodes[j].m_Maxcoord);

						{
							ion_int32 ii;
							ii=m_pNodes[j].m_Mincoord[0]; m_pNodes[j].m_Mincoord[0]=m_pNodes[j].m_Maxcoord[0]; m_pNodes[j].m_Maxcoord[0]=ii;
							ii=m_pNodes[j].m_Mincoord[2]; m_pNodes[j].m_Mincoord[2]=m_pNodes[j].m_Maxcoord[2]; m_pNodes[j].m_Maxcoord[2]=ii;
						}

						m_pNodes[j].m_Mincoord[0]=(ion_int32)(m_pNodes[j].m_Mincoord[0]*m_Mapscale);
						m_pNodes[j].m_Mincoord[1]=(ion_int32)(m_pNodes[j].m_Mincoord[1]*m_Mapscale);
						m_pNodes[j].m_Mincoord[2]=(ion_int32)(m_pNodes[j].m_Mincoord[2]*m_Mapscale);

						m_pNodes[j].m_Maxcoord[0]=(ion_int32)(m_pNodes[j].m_Maxcoord[0]*m_Mapscale);
						m_pNodes[j].m_Maxcoord[1]=(ion_int32)(m_pNodes[j].m_Maxcoord[1]*m_Mapscale);
						m_pNodes[j].m_Maxcoord[2]=(ion_int32)(m_pNodes[j].m_Maxcoord[2]*m_Mapscale);

						progressStatus+=nodesProgress*(float)(j)/(float)(m_NumNodes-1);
						REPORTPROGRESS(Progress::LoadingWorldData);
					}
				} break;
				case Lumptype_Leafs: {
					m_NumLeaves=lumplen/(4+4*2+4*3+4*3);
					m_pLeaves=new Leaf[m_NumLeaves];

					for (ion_uint32 j=0;j<m_NumLeaves;++j) {
						bspfile >> m_pLeaves[j].m_ClusterNr >> m_pLeaves[j].m_AreaportalArea;
						bspfile >> m_pLeaves[j].m_Mincoord[0] >> m_pLeaves[j].m_Mincoord[1] >> m_pLeaves[j].m_Mincoord[2];
						bspfile >> m_pLeaves[j].m_Maxcoord[0] >> m_pLeaves[j].m_Maxcoord[1] >> m_pLeaves[j].m_Maxcoord[2];
						bspfile >> m_pLeaves[j].m_Leafface >> m_pLeaves[j].m_NumLeaffaces;
						bspfile >> m_pLeaves[j].m_Leafbrush >> m_pLeaves[j].m_NumLeafbrushes;

						swizzle(m_pLeaves[j].m_Mincoord);
						swizzle(m_pLeaves[j].m_Maxcoord);

						{
							ion_int32 ii;
							ii=m_pLeaves[j].m_Mincoord[0]; m_pLeaves[j].m_Mincoord[0]=m_pLeaves[j].m_Maxcoord[0]; m_pLeaves[j].m_Maxcoord[0]=ii;
							ii=m_pLeaves[j].m_Mincoord[2]; m_pLeaves[j].m_Mincoord[2]=m_pLeaves[j].m_Maxcoord[2]; m_pLeaves[j].m_Maxcoord[2]=ii;
						}

						m_pLeaves[j].m_Mincoord[0]=(ion_int32)((float)(m_pLeaves[j].m_Mincoord[0])*m_Mapscale);
						m_pLeaves[j].m_Mincoord[1]=(ion_int32)((float)(m_pLeaves[j].m_Mincoord[1])*m_Mapscale);
						m_pLeaves[j].m_Mincoord[2]=(ion_int32)((float)(m_pLeaves[j].m_Mincoord[2])*m_Mapscale);

						m_pLeaves[j].m_Maxcoord[0]=(ion_int32)((float)(m_pLeaves[j].m_Maxcoord[0])*m_Mapscale);
						m_pLeaves[j].m_Maxcoord[1]=(ion_int32)((float)(m_pLeaves[j].m_Maxcoord[1])*m_Mapscale);
						m_pLeaves[j].m_Maxcoord[2]=(ion_int32)((float)(m_pLeaves[j].m_Maxcoord[2])*m_Mapscale);

						progressStatus+=leafsProgress*(float)(j)/(float)(m_NumLeaves-1);
						REPORTPROGRESS(Progress::LoadingWorldData);
					}
				} break;
				case Lumptype_Leaffaces: {
					m_NumLeaffaces=lumplen/4;
					m_pLeaffaces=new ion_int32[m_NumLeaffaces];

					bspfile.read(m_pLeaffaces,m_NumLeaffaces*4);

					progressStatus+=leaffacesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Leafbrushes: {
					m_NumLeafbrushes=lumplen/4;
					m_pLeafbrushes=new ion_int32[m_NumLeafbrushes];

					bspfile.read(m_pLeafbrushes,m_NumLeafbrushes*4);

					progressStatus+=leafbrushesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Models: {
					m_NumBSPModels=lumplen/(3*4+3*4+4*4);
					m_pBSPModels=new BSPModel[m_NumBSPModels];

					for (ion_uint32 j=0;j<m_NumBSPModels;++j) {
						bspfile >> m_pBSPModels[j].m_Mincoord[0] >> m_pBSPModels[j].m_Mincoord[1] >> m_pBSPModels[j].m_Mincoord[2];
						bspfile >> m_pBSPModels[j].m_Maxcoord[0] >> m_pBSPModels[j].m_Maxcoord[1] >> m_pBSPModels[j].m_Maxcoord[2];
						bspfile >> m_pBSPModels[j].m_FirstFace >> m_pBSPModels[j].m_NumFaces;
						bspfile >> m_pBSPModels[j].m_FirstBrush >> m_pBSPModels[j].m_NumBrushes;

						swizzle(m_pBSPModels[j].m_Mincoord);
						swizzle(m_pBSPModels[j].m_Maxcoord);

						{
							float ii;
							ii=m_pBSPModels[j].m_Mincoord[0]; m_pBSPModels[j].m_Mincoord[0]=m_pBSPModels[j].m_Maxcoord[0]; m_pBSPModels[j].m_Maxcoord[0]=ii;
							ii=m_pBSPModels[j].m_Mincoord[2]; m_pBSPModels[j].m_Mincoord[2]=m_pBSPModels[j].m_Maxcoord[2]; m_pBSPModels[j].m_Maxcoord[2]=ii;
						}

						m_pBSPModels[j].m_Mincoord*=m_Mapscale;
						m_pBSPModels[j].m_Maxcoord*=m_Mapscale;

						progressStatus+=modelsProgress*(float)(j)/(float)(m_NumBSPModels-1);
						REPORTPROGRESS(Progress::LoadingWorldData);
					}
				} break;
				case Lumptype_Brushes: {
					m_NumBrushes=lumplen/(3*4);
					m_pBrushes=new Brush[m_NumBrushes];

					for (ion_uint32 j=0;j<m_NumBrushes;++j) {
						bspfile >> m_pBrushes[j].m_FirstBrushside >> m_pBrushes[j].m_NumBrushsides;
						bspfile >> m_pBrushes[j].m_Surfaceindex;

						progressStatus+=brushesProgress*(float)(j)/(float)(m_NumBrushes-1);
						REPORTPROGRESS(Progress::LoadingWorldData);
					}
				} break;
				case Lumptype_Brushsides: {
					m_NumBrushsides=lumplen/(2*4);
					m_pBrushsides=new Brushside[m_NumBrushsides];

					for (ion_uint32 j=0;j<m_NumBrushsides;++j) {
						bspfile >> m_pBrushsides[j].m_Planeindex >> m_pBrushsides[j].m_Surfaceindex;
					}

					progressStatus+=brushsidesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Vertices: {
					m_NumBSPVertices=lumplen/(3*4+2*2*4+3*4+1*4);
					m_pBSPVertices=new BSPVertex[m_NumBSPVertices];

					for (ion_uint32 j=0;j<m_NumBSPVertices;++j) {
						bspfile >> m_pBSPVertices[j].m_Position[0] >> m_pBSPVertices[j].m_Position[1] >> m_pBSPVertices[j].m_Position[2];
						bspfile >> m_pBSPVertices[j].m_Texcoord[0][0] >> m_pBSPVertices[j].m_Texcoord[0][1];
						bspfile >> m_pBSPVertices[j].m_Texcoord[1][0] >> m_pBSPVertices[j].m_Texcoord[1][1];
						bspfile >> m_pBSPVertices[j].m_Normal[0] >> m_pBSPVertices[j].m_Normal[1] >> m_pBSPVertices[j].m_Normal[2];

						ion_uint8 color[4];
						bspfile.read(color,4);
						m_pBSPVertices[j].m_ColorRGBA[0]=color[0];
						m_pBSPVertices[j].m_ColorRGBA[1]=color[1];
						m_pBSPVertices[j].m_ColorRGBA[2]=color[2];
						m_pBSPVertices[j].m_ColorRGBA[3]=color[3];

						swizzle(m_pBSPVertices[j].m_Position);
						swizzle(m_pBSPVertices[j].m_Normal);

						m_pBSPVertices[j].m_Position*=m_Mapscale;
					}

					progressStatus+=verticesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Meshverts: {
					m_NumMeshverts=lumplen/4;
					m_pMeshverts=new ion_int32[m_NumMeshverts];

					bspfile.read(m_pMeshverts,m_NumMeshverts*4);

					progressStatus+=meshvertsProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Effects: {
					m_NumEffects=lumplen/(64+4+4);
					m_pEffects=new Effect[m_NumEffects];

					ion_uint32 dwtemp;
					char effectname[65]; effectname[64]=0;
					for (ion_uint32 j=0;j<m_NumEffects;++j) {
						memset(effectname,0,64);
						bspfile.read(effectname,64);
						m_pEffects[j].m_Name=effectname;
						bspfile >> m_pEffects[j].m_Brushindex >> dwtemp;
					}

					progressStatus+=effectsProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Faces: {
					m_NumFaces=lumplen/(8*4+2*4+2*4+3*4+2*3*4+3*4+2*4);
					m_pFaces=new Face[m_NumFaces];

					for (ion_uint32 j=0;j<m_NumFaces;++j) {
						bspfile >> m_pFaces[j].m_Surfaceindex >> m_pFaces[j].m_Effectindex;
						bspfile >> m_pFaces[j].m_Type >> m_pFaces[j].m_FirstVertex;
						bspfile >> m_pFaces[j].m_NumVertices >> m_pFaces[j].m_FirstMeshvert >> m_pFaces[j].m_NumMeshverts;
						bspfile >> m_pFaces[j].m_Lightmapindex >> m_pFaces[j].m_LightmapStart[0] >> m_pFaces[j].m_LightmapStart[1];
						bspfile >> m_pFaces[j].m_LightmapSize[0] >> m_pFaces[j].m_LightmapSize[1];
						bspfile >> m_pFaces[j].m_LightmapOrigin[0] >> m_pFaces[j].m_LightmapOrigin[1] >> m_pFaces[j].m_LightmapOrigin[2];
						bspfile >> m_pFaces[j].m_SVector[0] >> m_pFaces[j].m_SVector[1] >> m_pFaces[j].m_SVector[2];
						bspfile >> m_pFaces[j].m_TVector[0] >> m_pFaces[j].m_TVector[1] >> m_pFaces[j].m_TVector[2];
						bspfile >> m_pFaces[j].m_Normal[0] >> m_pFaces[j].m_Normal[1] >> m_pFaces[j].m_Normal[2];
						bspfile >> m_pFaces[j].m_Patchsize[0] >> m_pFaces[j].m_Patchsize[1];
						m_pFaces[j].m_pBezierPatch=0;

						if ((m_pFaces[j].m_Surfaceindex>=0) && (m_pFaces[j].m_Surfaceindex<(ion_int32)m_NumSurfaces)) {
							if (m_pSurfaces[m_pFaces[j].m_Surfaceindex].m_IsTransparent)
								++numTransparentSurfaces;
						}

						swizzle(m_pFaces[j].m_SVector);
						swizzle(m_pFaces[j].m_TVector);

						switch (m_pFaces[j].m_Type) {
							case Facetype_Polygon:
								numTriangles+=m_pFaces[j].m_NumMeshverts/3;
								break;
							case Facetype_Mesh:
								numTriangles+=m_pFaces[j].m_NumMeshverts/3;
								break;
							case Facetype_Patch: patchfaces.push_back(&m_pFaces[j]); break;
							default:break;
						}
					}

					base::logstream() << numTransparentSurfaces << " transparent faces\n";

					progressStatus+=facesProgress; REPORTPROGRESS(Progress::LoadingWorldData);
				} break;
				case Lumptype_Visdata:
					if (lumplen>0) {
					bspfile >> m_NumVisVectors >> m_VisvectorSize;
					m_pVisdata=new ion_uint8[m_NumVisVectors*m_VisvectorSize];
					bspfile.read(m_pVisdata,m_NumVisVectors*m_VisvectorSize);

					progressStatus+=visdataProgress; REPORTPROGRESS(Progress::LoadingVisibilityData);
				} break;
				default:break;
			}
		}

		video::Vertexformat vf;
		vf.addEntry(video::VertexFormatEntry_Position,video::VertexFormatSemantic_Position);
		vf.addEntry(video::VertexFormatEntry_Normal,video::VertexFormatSemantic_Normal);
		vf.addEntry(video::VertexFormatEntry_Diffuse,video::VertexFormatSemantic_None);
		vf.addEntry(video::VertexFormatEntry_Texcoord2D,video::VertexFormatSemantic_Texcoord);
		//vf.addEntry(video::VertexFormatEntry_Texcoord2D,video::VertexFormatSemantic_Texcoord);
		vf.addEntry(video::VertexFormatEntry_Texcoord3D,video::VertexFormatSemantic_Tangent);
		vf.addEntry(video::VertexFormatEntry_Texcoord3D,video::VertexFormatSemantic_Binormal);

		base::logstream() << "Tesselating " << (unsigned long)(patchfaces.size()) << " patches...\n";

		// Tesselate the bezier patches
		{
			const int level=1;

			for (std::vector< Face* >::iterator it=patchfaces.begin();it!=patchfaces.end();++it) {
				Face* pFace=(*it);
				BSPVertex *pControlPoints=&m_pBSPVertices[pFace->m_FirstVertex];

				// Calculate tangent space vectors on control points
				{
					int u,v;

					for (v=0;v<pFace->m_Patchsize[1];++v) {
						int v2=(v+1)%pFace->m_Patchsize[1];
						for (u=0;u<pFace->m_Patchsize[0];++u) {
							int u2=(u+1)%pFace->m_Patchsize[0];
							int ii;
							math::Vector3f pVecXYZ[3];
							math::Vector2f pVecUV[3];

							BSPVertex *pPoints[3]={
								&pControlPoints[u+v*pFace->m_Patchsize[0]],
								&pControlPoints[u2+v*pFace->m_Patchsize[0]],
								&pControlPoints[u+v2*pFace->m_Patchsize[0]]
							};

							for (ii=0;ii<3;++ii) {
								pVecXYZ[ii]=pPoints[ii]->m_Position;
								pVecUV[ii]=pPoints[ii]->m_Texcoord[0];
							}

							math::Plane planes[3];

							for (ii=0;ii<3;++ii) {
								math::Vector3f nvec[2];
								const math::Vector3f &v0=pVecXYZ[0];
								const math::Vector3f &v1=pVecXYZ[1];
								const math::Vector3f &v2=pVecXYZ[2];
								nvec[0]=math::Vector3f(
									(v0[ii])-(v1[ii]),
									pVecUV[0].x()-pVecUV[1].x(),
									pVecUV[0].y()-pVecUV[1].y()
									);
								nvec[1]=math::Vector3f(
									v0[ii]-v2[ii],
									pVecUV[0].x()-pVecUV[2].x(),
									pVecUV[0].y()-pVecUV[2].y()
									);

								const float *pXYZ=pVecXYZ[0];
							
								planes[ii].normal(nvec[0]^nvec[1]);
								planes[ii].d(
									(const math::Vector3f &)(planes[ii].normal())
									*
									(const math::Vector3f &)(math::Vector3f(pXYZ[ii],pVecUV[0].x(),pVecUV[0].y()))
									);
							}
							
							math::Vector3f tangent,binormal;
							for (ii=0;ii<3;++ii) {
								tangent[ii]=-planes[ii].b()/planes[ii].a();
								binormal[ii]=-planes[ii].c()/planes[ii].a();
							}

							for (ii=0;ii<3;++ii) {
								pPoints[ii]->m_Tangent+=tangent;
								pPoints[ii]->m_Binormal+=binormal;
							}

						}
					}
				}

				std::vector < std::vector < BSPVertex > > vrows,urows;

				int u,v;

				for (v=0;v<pFace->m_Patchsize[1];++v) {
					std::vector < BSPVertex > row=math::calculateQuadraticBezier(
						&pControlPoints[v*pFace->m_Patchsize[0]],pFace->m_Patchsize[0],level);

					vrows.push_back(row);
				}

				BSPVertex* pCPoints=new BSPVertex[pFace->m_Patchsize[1]];
				for (u=0;u<(int)(vrows[0].size());++u) {
					for (v=0;v<pFace->m_Patchsize[1];++v)
						pCPoints[v]=vrows[v][u];

					std::vector < BSPVertex > row=math::calculateQuadraticBezier(
						pCPoints,pFace->m_Patchsize[1],level);

					urows.push_back(row);
				}
				delete [] pCPoints;

				BezierPatch *pBezierPatch=new BezierPatch;
				pFace->m_pBezierPatch=pBezierPatch;

				int usize=(int)(urows.size());
				int vsize=(int)(urows[0].size());

				bool firstPoint=true;
				pBezierPatch->m_pVertexstream=renderer()->videodevice()->createVertexstream(
					usize*vsize,vf,video::Streamflags_Writeonly,video::Mempool_Managed);
				pBezierPatch->m_pVertexstream->map(video::Map_Writeonly);
				video::Vertexiterator viterator(*(pBezierPatch->m_pVertexstream));
				for (std::vector < std::vector < BSPVertex > >::iterator itrow=urows.begin();itrow!=urows.end();++itrow) {
					std::vector < BSPVertex >& row=(*itrow);
					for (std::vector < BSPVertex >::iterator itpoint=row.begin();itpoint!=row.end();++itpoint) {
						BSPVertex& point=(*itpoint);

						if (firstPoint) {
							pBezierPatch->m_Mincoord=pBezierPatch->m_Maxcoord=point.m_Position;
							firstPoint=false;
						} else {
							if (pBezierPatch->m_Mincoord.x()>point.m_Position.x()) pBezierPatch->m_Mincoord.x()=point.m_Position.x();
							if (pBezierPatch->m_Mincoord.y()>point.m_Position.y()) pBezierPatch->m_Mincoord.y()=point.m_Position.y();
							if (pBezierPatch->m_Mincoord.z()>point.m_Position.z()) pBezierPatch->m_Mincoord.z()=point.m_Position.z();

							if (pBezierPatch->m_Maxcoord.x()<point.m_Position.x()) pBezierPatch->m_Maxcoord.x()=point.m_Position.x();
							if (pBezierPatch->m_Maxcoord.y()<point.m_Position.y()) pBezierPatch->m_Maxcoord.y()=point.m_Position.y();
							if (pBezierPatch->m_Maxcoord.z()<point.m_Position.z()) pBezierPatch->m_Maxcoord.z()=point.m_Position.z();
						}

						point.m_Normal.normalize();
						point.m_Tangent.normalize();
						point.m_Binormal=point.m_Normal ^ point.m_Tangent;

						viterator.position(point.m_Position);
						viterator.normal(point.m_Normal);
						viterator.diffuseColor(point.m_ColorRGBA[3],point.m_ColorRGBA[0],point.m_ColorRGBA[1],point.m_ColorRGBA[2]);
						viterator.texcoord2D(0,point.m_Texcoord[0].x(),point.m_Texcoord[0].y());
						viterator.texcoord3D(0,point.m_Tangent.x(),point.m_Tangent.y(),point.m_Tangent.z());
						viterator.texcoord3D(1,point.m_Binormal.x(),point.m_Binormal.y(),point.m_Binormal.z());
						//viterator.texcoord2D(1,point.m_Texcoord[1].x(),point.m_Texcoord[1].y());

						++viterator;
					}
				}
				pBezierPatch->m_pVertexstream->unmap();

				int numtriangles=(usize-1)*(vsize-1)*2;
				pBezierPatch->m_pIndexstream=renderer()->videodevice()->createIndexstream(
					numtriangles*3,
					((usize*vsize)>65535) ? video::Indexformat_32bit : video::Indexformat_16bit,
					video::Streamflags_Writeonly,
					video::Mempool_Managed
					);
				pBezierPatch->m_pIndexstream->map(video::Map_Writeonly);
				video::Indexiterator iiterator(*(pBezierPatch->m_pIndexstream));
				for (u=0;u<usize-1;++u) {
					for (v=0;v<vsize-1;++v) {
						ion_uint32 idx11=(ion_uint32)(v+u*vsize);
						ion_uint32 idx21=(ion_uint32)((v+1)+u*vsize);
						ion_uint32 idx12=(ion_uint32)(v+(u+1)*vsize);
						ion_uint32 idx22=(ion_uint32)((v+1)+(u+1)*vsize);

						iiterator=idx11; ++iiterator;
						iiterator=idx21; ++iiterator;
						iiterator=idx12; ++iiterator;

						iiterator=idx12; ++iiterator;
						iiterator=idx21; ++iiterator;
						iiterator=idx22; ++iiterator;

					}
				}
				pBezierPatch->m_pIndexstream->unmap();

				pBezierPatch->m_pRenderjob=new Renderjob(*this);
				pBezierPatch->m_pRenderjob->primitivestype(video::Primitives_Triangles);
				pBezierPatch->m_pRenderjob->indexstream(pBezierPatch->m_pIndexstream);
				pBezierPatch->m_pRenderjob->vertexstream(*(pBezierPatch->m_pVertexstream));
				pBezierPatch->m_pRenderjob->shader(0);
				pBezierPatch->m_pRenderjob->rendertargetCamera(0);
				pBezierPatch->m_pRenderjob->propertytable(0);
				pBezierPatch->m_pRenderjob->viewerDistance(0);
				pBezierPatch->m_pRenderjob->firstElement(0);
				pBezierPatch->m_pRenderjob->indexOffset(0);
				pBezierPatch->m_pRenderjob->numElements(numtriangles);
				pBezierPatch->m_pRenderjob->propertytable(&(m_pSurfaces[pFace->m_Surfaceindex].m_Material));

			}
		}

		base::logstream() << "Done.\n";

		progressStatus+=0.05f; REPORTPROGRESS(Progress::LoadingWorldData);

		base::logstream() << "Allocating space for " << m_NumBSPVertices << " vertices...\n";

		// Prepare the vertex buffer

		m_pVertices=renderer()->videodevice()->createVertexstream(
			m_NumBSPVertices,vf,video::Streamflags_Writeonly,video::Mempool_Managed);

		base::logstream() << "Done.\n";

		{
			math::Vector3f *pTangents=new math::Vector3f[m_pVertices->capacity()];
			math::Vector3f *pBinormal=new math::Vector3f[m_pVertices->capacity()];

			for (ion_uint32 facenr=0;facenr<m_NumFaces;++facenr) {
				Face &rFace=m_pFaces[facenr];

				if ((rFace.m_Type!=Facetype_Polygon) && (rFace.m_Type!=Facetype_Mesh)) continue;

				for (ion_int32 j=0;j<rFace.m_NumMeshverts/3;++j) {
					ion_uint32 indices[3];
					ion_uint32 ii;
					math::Vector3f pVecXYZ[3];
					math::Vector2f pVecUV[3];

					for (ii=0;ii<3;++ii) {
						indices[ii]=m_pMeshverts[rFace.m_FirstMeshvert+j*3+ii]+rFace.m_FirstVertex;
						pVecXYZ[ii]=m_pBSPVertices[indices[ii]].m_Position;
						pVecUV[ii]=m_pBSPVertices[indices[ii]].m_Texcoord[0];
					}

					math::Plane planes[3];

					for (ii=0;ii<3;++ii) {
						math::Vector3f nvec[2];
						const math::Vector3f &v0=pVecXYZ[0];
						const math::Vector3f &v1=pVecXYZ[1];
						const math::Vector3f &v2=pVecXYZ[2];
						nvec[0]=math::Vector3f(
							(v0[ii])-(v1[ii]),
							pVecUV[0].x()-pVecUV[1].x(),
							pVecUV[0].y()-pVecUV[1].y()
							);
						nvec[1]=math::Vector3f(
							v0[ii]-v2[ii],
							pVecUV[0].x()-pVecUV[2].x(),
							pVecUV[0].y()-pVecUV[2].y()
							);

						const float *pXYZ=pVecXYZ[0];
						
						planes[ii].normal(nvec[0]^nvec[1]);
						planes[ii].d(
							(const math::Vector3f &)(planes[ii].normal())
							*
							(const math::Vector3f &)(math::Vector3f(pXYZ[ii],pVecUV[0].x(),pVecUV[0].y()))
							);
					}

					// tangent = ( -B0/A0,-B1/A1,-B2/A2 )
					// binormal = ( -C0/A0,-C1/A1,-C2/A2 )
					math::Vector3f tangent,binormal;
					for (ii=0;ii<3;++ii) {
						if (planes[ii].a()!=0.0f) {
							tangent[ii]=-planes[ii].b()/planes[ii].a();
							binormal[ii]=-planes[ii].c()/planes[ii].a();
						}
					}

					for (ii=0;ii<3;++ii) {
						pTangents[indices[ii]]+=tangent;
						pBinormal[indices[ii]]+=binormal;
					}
				}

				//phil
				// Does not work correctly with haste0402.bsp
				/*BSPVertex		*vert = m_pBSPVertices + rFace.m_FirstVertex;
				math::Vector3f	t, b;
				calculateTangents(vert[0].m_Normal, t, b, vert[0].m_Position, vert[1].m_Position, vert[2].m_Position, vert[0].m_Texcoord[0], vert[1].m_Texcoord[0], vert[2].m_Texcoord[0]);
				for(int ii = 0; ii < rFace.m_NumVertices; ii++)
				{
					pTangents[rFace.m_FirstVertex + ii] = t;
					pBinormal[rFace.m_FirstVertex + ii] = b;
				}*/
			}

			m_pVertices->map(video::Map_Writeonly);
			for (ion_uint32 vtx=0;vtx<m_pVertices->capacity();++vtx) {
				const BSPVertex &rBSPVtx=m_pBSPVertices[vtx];
				m_pVertices->position(vtx,rBSPVtx.m_Position);
				m_pVertices->normal(vtx,rBSPVtx.m_Normal.normalizedVector());
				m_pVertices->diffuseColor(vtx,
					rBSPVtx.m_ColorRGBA[3],rBSPVtx.m_ColorRGBA[0],rBSPVtx.m_ColorRGBA[1],rBSPVtx.m_ColorRGBA[2]);
				m_pVertices->texcoord2D(vtx,0,rBSPVtx.m_Texcoord[0]);
				//m_pVertices->texcoord2D(vtx,1,rBSPVtx.m_Texcoord[1]);
				m_pVertices->texcoord3D(vtx,0,pTangents[vtx].normalizedVector());
				m_pVertices->texcoord3D(vtx,1,/*pBinormal[vtx]*/rBSPVtx.m_Normal.normalizedVector() ^ pTangents[vtx].normalizedVector());
			}

			m_pVertices->unmap();

			delete [] pBinormal;
			delete [] pTangents;
		}

		// Prepare the renderjobs

		{
			m_pOpaqueRenderjobs=new Renderjob* [m_NumSurfaces];
			for (ion_uint32 i=0;i<m_NumSurfaces;++i) {
				m_pOpaqueRenderjobs[i]=new Renderjob(*this);
			}
		}

		base::logstream() << "Min map coord: " << m_pBSPModels[0].m_Mincoord.x() << " " << m_pBSPModels[0].m_Mincoord.y() << " " << m_pBSPModels[0].m_Mincoord.z() << "\n";
		base::logstream() << "Max map coord: " << m_pBSPModels[0].m_Maxcoord.x() << " " << m_pBSPModels[0].m_Maxcoord.y() << " " << m_pBSPModels[0].m_Maxcoord.z() << "\n";

		base::logstream() << "Allocating space for " << numTriangles << " triangles...\n";

		// Allocate the index buffer

		m_pIndices=renderer()->videodevice()->createIndexstream(
			numTriangles*3,
			(m_NumBSPVertices>65535) ? video::Indexformat_32bit : video::Indexformat_16bit,
			video::Streamflags_Dynamic|video::Streamflags_Writeonly,
			video::Mempool_Default);

		/*for (ion_uint32 j=0;j<m_NumBSPModels;++j) {
			if ((j>=1) && (m_pBSPModels[j].m_NumFaces>0)) { // j>=1 because #0 is the map itself
				m_pBSPModels[j].init(*this);
			}
		}*/

		base::logstream() << "Done.\n";

		m_IsValid=true;

	}

	void Q3BSP::calculateTangents(
		math::Vector3f& normal,
		math::Vector3f& tangent, 
		math::Vector3f& binormal, 
		math::Vector3f& vt1, math::Vector3f& vt2, math::Vector3f& vt3, // vertices
		math::Vector2f& tc1, math::Vector2f& tc2, math::Vector2f& tc3) // texture coords
	{
		math::Vector3f v1 = vt1 - vt2;
		math::Vector3f v2 = vt3 - vt1;
		normal = v2 ^ v1;
		normal.normalize();

		// binormal 

		float deltaX1 = tc1[0] - tc2[0];
		float deltaX2 = tc3[0] - tc1[0];
		binormal = (v1 * deltaX2) - (v2 * deltaX1);
		binormal.normalize();

		// tangent

		float deltaY1 = tc1[1] - tc2[1];
		float deltaY2 = tc3[1] - tc1[1];
		tangent = (v1 * deltaY2) - (v2 * deltaY1);
		tangent.normalize();

		// adjust

		math::Vector3f txb = tangent ^ binormal;
		if (txb * normal < 0.0f)
		{
			tangent *= -1.0f;
			binormal *= -1.0f;
		}
	}






}
}
