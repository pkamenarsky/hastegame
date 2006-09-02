#ifndef ION_SCENE_Q3BSP_HH_INCLUDED
#define ION_SCENE_Q3BSP_HH_INCLUDED

#include "../math/plane.hh"
#include "../video/indexstream.hh"
#include "../video/vertexstream.hh"
#include "light.hh"
#include "renderable.hh"
#include "renderjob.hh"
#include "space.hh"
#include "../video/color.hh"

#include <set>
#include <vector>

namespace ion {
namespace scene {

	class ION_API Q3BSP:public Space,public Renderable
	{
	protected:
		struct BezierPatch;
	public:
		class BSPLight;
		class BSPRenderable;
		friend class BSPLight;
		friend class BSPRenderable;


		enum Facetype {
			Facetype_Polygon=1,
			Facetype_Patch=2,
			Facetype_Mesh=3,
			Facetype_Billboard=4
		};

		struct Direntry {
			ion_uint32 m_Offset,m_Length;
		};

		struct Surface {
			base::String m_Shadername;
			ion_uint32 m_Flags,m_Contents;
			video::Propertytable m_Material;
			bool m_IsTransparent,m_IsSky;
		};

		struct Node {
			ion_int32 m_Planeindex;
			ion_int32 m_FrontChild,m_BackChild;
			ion_int32 m_Mincoord[3],m_Maxcoord[3];
		};

		struct Leaf {
		friend class Q3BSP;
		friend class BSPLight;
		friend class BSPRenderable;
		public:
			ion_int32 m_ClusterNr,m_AreaportalArea;
			ion_int32 m_Mincoord[3],m_Maxcoord[3];
			ion_int32 m_Leafface,m_NumLeaffaces;
			ion_int32 m_Leafbrush,m_NumLeafbrushes;

			// The following members are not part of Q3 BSP leaves

			Leaf();
			~Leaf();

		protected:
			std::set < ion_uint32 > *m_pLights,*m_pRenderables;
		};

		class ION_API BSPRenderable:public scene::Node::TransformAlterationFunctor
		{
		friend class Q3BSP;
		public:
			BSPRenderable(Q3BSP& rBSP,Renderable &rRenderable,const ion_uint32 renderableidx);
			~BSPRenderable();
			
			const Renderable& renderable() const;
			Renderable& renderable();

			void transformAltered(const bool rotationAltered,const bool positionAltered);

			void update();
		protected:
			void updateLeafList();
			void clearLeafList();
			void addRenderableToLeaves(const ion_int32 nodeNum);

			Q3BSP &m_rQ3BSP;
			Renderable &m_rRenderable;
			std::vector < Leaf* > *m_pLeaves;
			ion_uint32 m_Renderableindex;
		};

		class BSPModel:public Renderable {
		friend class Q3BSP;
		public:
			math::Vector3f m_Mincoord,m_Maxcoord;
			ion_int32 m_FirstFace,m_NumFaces;
			ion_int32 m_FirstBrush,m_NumBrushes;

			bool isValid() const;

			const math::SphereVolume& boundingSphere() const;

			const char *type() const;
			void respawn(base::Streamable& source);
			void serialize(base::Streamable& dest);

			ion_uint32 numJobs() const;
			Renderjob* job(const ion_uint32 jobnr);

			void restoreVideoResources();

		protected:
			BSPModel();
			~BSPModel();

			void init(Q3BSP &rQ3BSP);

			void rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer);

			Q3BSP *m_pQ3BSP;
			video::Indexstream *m_pIndices;
			BSPRenderable *m_pBSPRenderable;

			Renderjob **m_pRenderjobs;
			ion_uint32 m_NumRenderjobs;

			math::SphereVolume m_BoundingSphere;
		};

		struct Brush {
			ion_int32 m_FirstBrushside,m_NumBrushsides;
			ion_int32 m_Surfaceindex;
		};

		struct Brushside {
			ion_int32 m_Planeindex,m_Surfaceindex;
		};

		struct BSPVertex {
			math::Vector3f m_Position;
			math::Vector2f m_Texcoord[2];
			math::Vector3f m_Normal,m_Tangent,m_Binormal;
			int m_ColorRGBA[4];
			friend BSPVertex operator *(const BSPVertex& a,const float f);
			friend BSPVertex operator +(const BSPVertex& a,const BSPVertex& b);
		};

		struct Effect {
			base::String m_Name;
			ion_int32 m_Brushindex;
		};

		struct Face {
			ion_int32 m_Surfaceindex,m_Effectindex,m_Type,m_FirstVertex,m_NumVertices,
				m_FirstMeshvert,m_NumMeshverts,m_Lightmapindex,m_LightmapStart[2],m_LightmapSize[2];
			math::Vector3f m_LightmapOrigin,m_SVector,m_TVector,m_Normal;
			ion_int32 m_Patchsize[2];
			BezierPatch *m_pBezierPatch;

			math::Plane m_Plane;
		};


		class ION_API BSPLight:public scene::Node::TransformAlterationFunctor
		{
		friend class Q3BSP;
		public:
			BSPLight(Q3BSP& rBSP,Light &rLight,const ion_uint32 lightidx);
			~BSPLight();

			ion_uint32 numLightingjobs() const;
			Renderjob *lightingjob(const ion_uint32 jobnr);

			const Light& light() const;
			Light& light();

			void transformAltered(const bool rotationAltered,const bool positionAltered);

			void update();
		protected:
			bool sphereTriangleIntersection(const float radius,const math::Vector3f& center,
				const math::Vector3f& p1,const math::Vector3f& p2,const math::Vector3f& p3);

			//void newLightindex(const ion_uint32 lightidx);
			void updateLeafList(/*std::set<ion_uint32> &renderableIndices*/);
			void clearLeafList();
			void addLightToLeaves(const ion_int32 nodeNum,ion_uint32& indicesCounter,std::vector < bool >& alreadyVisible/*,std::set<ion_uint32> &renderableIndices*/);
			//void addLightToLeaves(const ion_int32 nodeNum);

			Q3BSP &m_rQ3BSP;
			static const ion_uint32 s_IndexPadding;
			video::Indexstream *m_pIndices;
			Light &m_rLight;
			std::vector < Leaf* > *m_pLeaves;
			std::vector < BezierPatch* > *m_pBezierPatches;
			Renderjob **m_pJobs,**m_pActiveJobs;
			ion_uint32 m_NumJobs; // Amount of jobs = amount of jobs in BSP, or 1 if there are no surfaces (should never happen)
			ion_uint32 m_NumActiveJobs;
			ion_uint32 m_Lightindex;
		};




		// Progress functor

		class Progress
		{
		public:
			virtual ~Progress();

			enum Stage {
				LoadingWorldData,
				LoadingShaders,
				LoadingVisibilityData
			};

			/**
			* @param stage The current initialization stage
			* @param completed How much of the initialization is complete.
			* Range is 0.0 (nothing completed) - 1.0 (fully completed)
			*/
			virtual void operator () (const Stage stage,const float completed)=0;
		protected:
			Progress();
		};


		Q3BSP(const base::String& bspname,Renderer& rRenderer,Progress* pProgress=0,const float mapscale=1.0f);
		~Q3BSP();


		BSPLight* addLight(Light &rLight);
		void removeLight(BSPLight &rLight);
		void removeLight(Light &rLight);
		void updateLightGeometry();


		BSPRenderable* addRenderable(Renderable &rRenderable);
		void removeRenderable(BSPRenderable &rRenderable);
		void removeRenderable(Renderable &rRenderable);

		void determineVisibity(Renderquery& query);
		void findInRadius(Renderquery& query,const math::Vector3f point,const float radius);
		void findNearest(Renderquery& query,const math::Vector3f point,const ion_uint32 maxNodes);

		ion_uint32 numJobs() const;
		Renderjob* job(const ion_uint32 jobnr);
		ion_uint32 numLightingJobs(const ion_uint32 lightnr,const Light& rLight) const;
		Renderjob* lightingjob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		ion_uint32 numPointlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		Renderjob* pointlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		ion_uint32 numDirlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		Renderjob* dirlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		ion_uint32 numSpecularlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		Renderjob* specularlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);

		void update(const Camera& camera,const Updatemode mode);
		void restoreVideoResources();

		const char *type() const;
		void respawn(base::Streamable& source);
		void serialize(base::Streamable& dest);

		bool isValid() const;



		const base::String& entities() const;

		const Surface* surfaces() const;
		ion_uint32 numSurfaces() const;

		const math::Plane* planes() const;
		ion_uint32 numPlanes() const;

		const Node* nodes() const;
		ion_uint32 numNodes() const;

		const Leaf* leaves() const;
		ion_uint32 numLeaves() const;

		const ion_int32* leaffaces() const;
		ion_uint32 numLeaffaces() const;

		const ion_int32* leafbrushes() const;
		ion_uint32 numLeafbrushes() const;

		const BSPModel* BSPModels() const;
		ion_uint32 numBSPModels() const;

		const Brush* brushes() const;
		ion_uint32 numBrushes() const;

		const Brushside* brushsides() const;
		ion_uint32 numBrushsides() const;

		const BSPVertex* BSPVertices() const;
		ion_uint32 numBSPVertices() const;

		const ion_int32* meshverts() const;
		ion_uint32 numMeshverts() const;

		const Effect* effects() const;
		ion_uint32 numEffects() const;

		const Face* faces() const;
		ion_uint32 numFaces() const;

		const ion_uint8* visdata() const;
		ion_uint32 numVisVectors() const;
		ion_uint32 visvectorSize() const;


		void useFrustumCulling(const bool use);
		bool usesFrustumCulling() const;

		void ambientColor(const video::Color& color);
		const video::Color& ambientColor() const;


	protected:

		enum Lumptype
		{
			Lumptype_Entities=0,
			Lumptype_Textures=1,
			Lumptype_Planes=2,
			Lumptype_Nodes=3,
			Lumptype_Leafs=4,
			Lumptype_Leaffaces=5,
			Lumptype_Leafbrushes=6,
			Lumptype_Models=7,
			Lumptype_Brushes=8,
			Lumptype_Brushsides=9,
			Lumptype_Vertices=10,
			Lumptype_Meshverts=11,
			Lumptype_Effects=12,
			Lumptype_Faces=13,
			Lumptype_Lightmaps=14,
			Lumptype_Lightvols=15,
			Lumptype_Visdata=16
		};



		struct BezierPatch
		{
			video::Vertexstream *m_pVertexstream;
			video::Indexstream *m_pIndexstream;
			Surface *m_pSurface;
			Renderjob *m_pRenderjob;
			math::Vector3f m_Mincoord,m_Maxcoord;
		};


		static void calculateTangents(
			math::Vector3f& normal,
			math::Vector3f& tangent, 
			math::Vector3f& binormal, 
			math::Vector3f& vt1, math::Vector3f& vt2, math::Vector3f& vt3, // vertices
			math::Vector2f& tc1, math::Vector2f& tc2, math::Vector2f& tc3);

		void getLeavesForNode(int node, std::vector <int> &leaves);

		void removeLightindexFromLeaves(const ion_int32 nodeNum,const ion_uint32 lightindex);
		void removeRenderableindexFromLeaves(const ion_int32 nodeNum,const ion_uint32 renderableindex);

		int findLeaf(const math::Vector3f& camPos) const;
		bool isClusterVisible(const int visCluster,const int testCluster) const;

		void rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer);

		void swizzle(math::Vector3f& v);
		void swizzle(ion_int32 *pIntcoords);

		void load(const base::String& bspname,Progress* pProgress);
		void cleanup();

		Direntry m_Direntries[17];

		base::String m_Entities;
		Surface *m_pSurfaces; ion_uint32 m_NumSurfaces;
		math::Plane *m_pPlanes; ion_uint32 m_NumPlanes;
		Node* m_pNodes; ion_uint32 m_NumNodes;
		Leaf* m_pLeaves; ion_uint32 m_NumLeaves;
		ion_int32 *m_pLeaffaces; ion_uint32 m_NumLeaffaces;
		ion_int32 *m_pLeafbrushes; ion_uint32 m_NumLeafbrushes;
		BSPModel *m_pBSPModels; ion_uint32 m_NumBSPModels;
		Brush *m_pBrushes; ion_uint32 m_NumBrushes;
		Brushside *m_pBrushsides; ion_uint32 m_NumBrushsides;
		BSPVertex *m_pBSPVertices; ion_uint32 m_NumBSPVertices;
		ion_int32 *m_pMeshverts; ion_uint32 m_NumMeshverts;
		Effect *m_pEffects; ion_uint32 m_NumEffects;
		Face *m_pFaces; ion_uint32 m_NumFaces;
		ion_uint8 *m_pVisdata; ion_uint32 m_NumVisVectors,m_VisvectorSize,m_NumVisibleTriangles;
		video::Color m_AmbientColor;

		float m_Mapscale;
		bool m_IsValid,m_UsesFrustumCulling;

		struct BezierList;
		BezierList* m_pBezierList;

		video::Indexstream *m_pIndices;
		video::Vertexstream *m_pVertices;

		Renderjob **m_pOpaqueRenderjobs;
		ion_uint32 m_NumOpaqueRenderjobs;

		std::vector < BSPLight* > *m_pLights;
		std::vector < BSPRenderable* > *m_pRenderables;
	};

}
}

#endif
