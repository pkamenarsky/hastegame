#ifndef ION_SCENE_SPRITECACHE_HH_INCLUDED
#define ION_SCENE_SPRITECACHE_HH_INCLUDED

#include <vector>
#include "../base/refcounted.hh"
#include "../base/string.hh"
#include "../video/videodevice.hh"
#include "renderable.hh"

namespace ion {
namespace scene {

	class ION_API Spritecache:public Renderable
	{
	public:
		struct Entry {
			float m_SquaredDistanceToEye;
			math::Vector3f m_Position;
			math::Vector2f m_Size;
			video::Texture2D **m_pTextures;
			video::BlendFunc m_SrcBlend,m_DestBlend;
			unsigned long m_Age,m_FPS,m_CurSprite,m_NumSprites;
			bool m_LoopAnimation;
		};

		Spritecache(const base::String& identifier,Renderer& rRenderer,const ion_uint32 cacheSize);
		~Spritecache();

		void time(const float milliseconds);
		void basetime(const float newbasetime);

		ion_uint32 numJobs() const;
		Renderjob* job(const ion_uint32 jobnr);
		ion_uint32 numExecutableBlendedJobs()const;
		void executeBlendedJob(const ion_uint32 jobnr);
		ion_uint32 numExecutableBlendedLitJobs()const;
		void executeBlendedLitJob(const ion_uint32 jobnr);

		void restoreVideoResources();

		const char *type() const;
		void respawn(base::Streamable& source);
		void serialize(base::Streamable& dest);

		bool isValid() const;

		void age(const unsigned long ageamount);

		ion_uint32 addSprite(const math::Vector3f& position,const math::Vector2f& size,video::Texture2D *pTexture,
			const video::BlendFunc srcblend,const video::BlendFunc destblend,const unsigned long initialAge);
		ion_uint32 addSprites(const math::Vector3f& position,const math::Vector2f& size,video::Texture2D **pTextures,
			const ion_uint32 numsprites,const ion_uint32 fps,const video::BlendFunc srcblend,const video::BlendFunc destblend,
			const unsigned long initialAge,const bool loopAnimation=false);
		void removeSprite(const ion_uint32 spritenr);
		ion_uint32 numSprites() const;
		Entry* spriteEntry(const ion_uint32 spritenr);
		const Entry* spriteEntry(const ion_uint32 spritenr) const;
		void clearList();

		void update(const Camera& camera,const Updatemode mode);

		const math::SphereVolume& boundingSphere() const;
	protected:
		void rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer);

		ion_uint32 m_Size;
		video::Vertexstream *m_pVertexstream;
		math::SphereVolume m_Sphere;
		float m_BaseTime;

		struct Drawcall {
			ion_uint32 m_StartTriangle,m_NumTriangles;
			video::Texture2D *m_pTexture;
			video::BlendFunc m_SrcBlend,m_DestBlend;
		};
		static bool sortEntries(const Entry& e1,const Entry& e2);

		std::vector < Entry > *m_pEntries;
		std::vector < Drawcall > *m_pDrawcalls;
	};

}
}

#endif
