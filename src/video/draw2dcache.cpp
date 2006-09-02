#include <algorithm>
#define ION_VIDEO_DRAW2DCACHE_IMPLEMENTATION
#include "draw2dcache.hh"
#include "vcache.hh"
#include "videodevice.hh"
#include "vertexformat.hh"

namespace ion {
namespace video {

	Draw2DCache::Draw2DCache(Videodevice& rVideodevice,const ion_uint32 cachesize)
	{
		m_pEntries=new std::vector < Entry >;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_None);
		vf.addEntry(VertexFormatEntry_Diffuse,VertexFormatSemantic_None);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_None);

		m_pVCache=new VCache(rVideodevice,vf,cachesize*6);
	}

	Draw2DCache::~Draw2DCache()
	{
		delete m_pEntries;
		delete m_pVCache;
	}

	bool Draw2DCache::sortEntries(const Entry& e1,const Entry& e2)
	{
		return e1.m_pTex<e2.m_pTex;
	}

	void Draw2DCache::begin()
	{
		m_pEntries->clear();
	}

	void Draw2DCache::addImage(Texture2D &rTex,const math::FloatRectangle *pSrcRectangle,const math::FloatRectangle& rDestRectangle,const AddImageMode mode)
	{
		addImage(rTex,Color::white(),pSrcRectangle,rDestRectangle,mode);
	}

	void Draw2DCache::addImage(Texture2D &rTex,const math::FloatRectangle *pSrcRectangle,const math::Vector2f& position,const AddImageMode mode)
	{
		addImage(rTex,Color::white(),pSrcRectangle,position,mode);
	}

	void Draw2DCache::addImage(Texture2D &rTex,const Color& color,const math::FloatRectangle *pSrcRectangle,const math::Vector2f& position,const AddImageMode mode)
	{
		Entry entry;

		switch (mode) {
			case AddImageMode_ScreenNormalizedCoordinates: {
				float w=(float)(m_pVCache->videodevice().displaywidth()-1);
				float h=(float)(m_pVCache->videodevice().displayheight()-1);

				entry.m_X[0]=position.x()*w;
				entry.m_Y[0]=position.y()*h;

				entry.m_X[1]=entry.m_X[0]+((float)(rTex.width()));
				entry.m_Y[1]=entry.m_Y[0]+((float)(rTex.height()));
			} break;
			case AddImageMode_DirectCoordinates: {
				entry.m_X[0]=position.x();
				entry.m_Y[0]=position.y();
				entry.m_X[1]=entry.m_X[0]+((float)(rTex.width()));
				entry.m_Y[1]=entry.m_Y[0]+((float)(rTex.height()));
			} break;
			default:return;
		}

		entry.m_X[0]=floorf(entry.m_X[0]);
		entry.m_X[1]=floorf(entry.m_X[1]);
		entry.m_Y[0]=floorf(entry.m_Y[0]);
		entry.m_Y[1]=floorf(entry.m_Y[1]);

		if (pSrcRectangle) {
			entry.m_U[0]=pSrcRectangle->left();
			entry.m_U[1]=pSrcRectangle->left()+pSrcRectangle->width();
			entry.m_V[0]=pSrcRectangle->top();
			entry.m_V[1]=pSrcRectangle->top()+pSrcRectangle->height();
		} else {
			entry.m_U[0]=0;
			entry.m_U[1]=1;
			entry.m_V[0]=0;
			entry.m_V[1]=1;
		}
		entry.m_pTex=&rTex;
		entry.m_Color=color;

		m_pEntries->push_back(entry);
	}

	void Draw2DCache::addImage(Texture2D &rTex,const Color& color,const math::FloatRectangle *pSrcRectangle,const math::FloatRectangle& rDestRectangle,const AddImageMode mode)
	{
		Entry entry;

		switch (mode) {
			case AddImageMode_ScreenNormalizedCoordinates: {
				float w=(float)(m_pVCache->videodevice().displaywidth()-1);
				float h=(float)(m_pVCache->videodevice().displayheight()-1);

				entry.m_X[0]=rDestRectangle.left()*w;
				entry.m_Y[0]=rDestRectangle.top()*h;

				entry.m_X[1]=entry.m_X[0]+w*rDestRectangle.width();
				entry.m_Y[1]=entry.m_Y[0]+h*rDestRectangle.height();
			} break;
			case AddImageMode_DirectCoordinates: {
				entry.m_X[0]=rDestRectangle.left();
				entry.m_Y[0]=rDestRectangle.top();
				entry.m_X[1]=entry.m_X[0]+rDestRectangle.width();
				entry.m_Y[1]=entry.m_Y[0]+rDestRectangle.height();
			} break;
			default:return;
		}

		entry.m_X[0]=floorf(entry.m_X[0]);
		entry.m_X[1]=floorf(entry.m_X[1]);
		entry.m_Y[0]=floorf(entry.m_Y[0]);
		entry.m_Y[1]=floorf(entry.m_Y[1]);

		if (pSrcRectangle) {
			entry.m_U[0]=pSrcRectangle->left();
			entry.m_U[1]=pSrcRectangle->left()+pSrcRectangle->width();
			entry.m_V[0]=pSrcRectangle->top();
			entry.m_V[1]=pSrcRectangle->top()+pSrcRectangle->height();
		} else {
			entry.m_U[0]=0;
			entry.m_U[1]=1;
			entry.m_V[0]=0;
			entry.m_V[1]=1;
		}
		entry.m_pTex=&rTex;
		entry.m_Color=color;

		m_pEntries->push_back(entry);
	}

	void Draw2DCache::end()
	{
		if (m_pEntries->empty()) return;

		std::sort(m_pEntries->begin(),m_pEntries->end(),sortEntries);

		math::Matrix4f proj,oldproj,oldview,oldworld;
		proj.orthogonal(0,
			(float)(m_pVCache->videodevice().displaywidth()-1),
			(float)(m_pVCache->videodevice().displayheight()-1),
			0,0.8f,10.0f);

		oldproj=m_pVCache->videodevice().projmatrix();
		oldview=m_pVCache->videodevice().viewmatrix();
		oldworld=m_pVCache->videodevice().worldmatrix();

		m_pVCache->videodevice().activateFFPipeline(FixedfunctionVertex);
		m_pVCache->videodevice().activateFFPipeline(FixedfunctionFragment);
		m_pVCache->videodevice().projmatrix(proj);
		m_pVCache->videodevice().fixedLighting(false);
		m_pVCache->videodevice().viewmatrix(math::Matrix4f::identitymatrix());
		m_pVCache->videodevice().worldmatrix(math::Matrix4f::identitymatrix());
		m_pVCache->videodevice().culling(false);
		m_pVCache->videodevice().zbuffer(false);

		Texture2D *pTex=m_pEntries->begin()->m_pTex;
		m_pVCache->videodevice().bindTexture(0,pTex);

		static const int idxtable[6][2]={
			{0,0},{1,0},{0,1},
			{0,1},{1,0},{1,1}
		};

		m_pVCache->begin(Primitives_Triangles);
		for (std::vector < Entry >::const_iterator it=m_pEntries->begin();it!=m_pEntries->end();++it) {
			const Entry& entry=(*it);

			if (entry.m_pTex!=pTex) {
				m_pVCache->end();
				pTex=entry.m_pTex;
				m_pVCache->videodevice().bindTexture(0,pTex);
				m_pVCache->begin(Primitives_Triangles);
			}

			for (int i=0;i<6;++i) {
				int xidx=idxtable[i][0],yidx=idxtable[i][1];
				m_pVCache->position(entry.m_X[xidx]-0.5f,entry.m_Y[yidx]-0.5f,1.0f);
				m_pVCache->diffuseColor(entry.m_Color.aUint8(),entry.m_Color.rUint8(),entry.m_Color.gUint8(),entry.m_Color.bUint8());
				m_pVCache->texcoord2D(0,entry.m_U[xidx],entry.m_V[yidx]);
				m_pVCache->pushVertex();
			}
		}
		m_pVCache->end();

		m_pVCache->videodevice().bindTexture(0,0);
		m_pVCache->videodevice().projmatrix(oldproj);
		m_pVCache->videodevice().viewmatrix(oldview);
		m_pVCache->videodevice().worldmatrix(oldworld);

	}

	bool Draw2DCache::isValid() const
	{
		return (m_pVCache==0) ? false : m_pVCache->isValid();
	}

	Videodevice& Draw2DCache::videodevice() const
	{
		return m_pVCache->videodevice();
	}

}
}
