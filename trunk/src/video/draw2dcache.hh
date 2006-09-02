#ifndef ION_VIDEO_DRAW2DCACHE_HH_INCLUDED
#define ION_VIDEO_DRAW2DCACHE_HH_INCLUDED

#ifdef ION_VIDEO_DRAW2DCACHE_IMPLEMENTATION
#include <vector>
#endif

#include "../base/basememobj.hh"
#include "../math/rectangle.hh"
#include "../math/transform.hh"
#include "color.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	class VCache;

	class ION_API Draw2DCache:public base::BaseMemObj
	{
	public:
		enum AddImageMode {
			AddImageMode_ScreenNormalizedCoordinates,
			AddImageMode_DirectCoordinates
		};

		Draw2DCache(Videodevice& rVideodevice,const ion_uint32 cachesize);
		~Draw2DCache();

		void begin();
		void addImage(Texture2D &rTex,const math::FloatRectangle *pSrcRectangle,const math::FloatRectangle& rDestRectangle,const AddImageMode mode);
		void addImage(Texture2D &rTex,const math::FloatRectangle *pSrcRectangle,const math::Vector2f& position,const AddImageMode mode);
		void addImage(Texture2D &rTex,const Color& color,const math::FloatRectangle *pSrcRectangle,const math::FloatRectangle& rDestRectangle,const AddImageMode mode);
		void addImage(Texture2D &rTex,const Color& color,const math::FloatRectangle *pSrcRectangle,const math::Vector2f& position,const AddImageMode mode);
		void end();

		bool isValid() const;

		Videodevice& videodevice() const;

	protected:
		VCache *m_pVCache;

		struct Entry {
			float m_X[2],m_Y[2];
			float m_U[2],m_V[2];
			Texture2D *m_pTex;
			Color m_Color;
		};

		static bool sortEntries(const Entry& e1,const Entry& e2);

		// Preserves stack
#ifdef ION_VIDEO_DRAW2DCACHE_IMPLEMENTATION
		std::vector < Entry > *m_pEntries;
#else
		void *m_pDummy;
#endif
	};

}
}

#endif
