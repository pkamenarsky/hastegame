#include <algorithm>
#include <float.h>
#include "renderer.hh"
#include "renderjob.hh"
#include "spritecache.hh"

namespace ion {
namespace scene {


	Spritecache::Spritecache(const base::String& identifier,Renderer& rRenderer,const ion_uint32 cacheSize):Renderable(identifier)
	{
		video::Vertexformat vf;
		vf.addEntry(video::VertexFormatEntry_Position,video::VertexFormatSemantic_None);
		vf.addEntry(video::VertexFormatEntry_Texcoord2D,video::VertexFormatSemantic_None);
		vf.addEntry(video::VertexFormatEntry_Texcoord3D,video::VertexFormatSemantic_None);
		vf.addEntry(video::VertexFormatEntry_Diffuse,video::VertexFormatSemantic_None);

		renderer(rRenderer);
		m_Size=cacheSize;
		m_pVertexstream=rRenderer.videodevice()->createVertexstream(
			cacheSize*6,vf,video::Streamflags_Dynamic|video::Streamflags_Writeonly,video::Mempool_Default);

		m_pEntries=new std::vector < Entry >;
		m_pDrawcalls=new std::vector < Drawcall >;

		m_Sphere.center(math::Vector3f(0,0,0));
		m_Sphere.radius(FLT_MAX);
	}

	Spritecache::~Spritecache()
	{
		for (std::vector < Entry >::iterator it=m_pEntries->begin();it!=m_pEntries->end();++it) {
			Entry &entry=(*it);
			delete [] entry.m_pTextures;
		}

		delete m_pVertexstream;
		delete m_pEntries;
		delete m_pDrawcalls;
	}

	void Spritecache::time(const float milliseconds)
	{
		float dt=milliseconds-m_BaseTime;
		
		for (std::vector < Entry >::iterator it=m_pEntries->begin();it!=m_pEntries->end();++it) {
			Entry &entry=(*it);
			entry.m_CurSprite=(ion_uint32)(dt*((float)entry.m_FPS));
			if (entry.m_CurSprite>=entry.m_NumSprites) {
				if (entry.m_LoopAnimation) entry.m_CurSprite=entry.m_CurSprite%entry.m_NumSprites;
				else { entry.m_CurSprite=0; entry.m_Age=0; }
			}
		}
	}

	void Spritecache::basetime(const float newbasetime)
	{
		m_BaseTime=newbasetime;
	}

	ion_uint32 Spritecache::numJobs() const
	{
		return 0;
	}

	Renderjob* Spritecache::job(const ion_uint32 jobnr)
	{
		return 0;
	}

	void Spritecache::executeBlendedJob(const ion_uint32 jobnr)
	{
		m_pRenderer->videodevice()->culling(false);
		for (std::vector < Drawcall >::iterator it=m_pDrawcalls->begin();it!=m_pDrawcalls->end();++it) {
			Drawcall& drawcall=(*it);
			m_pRenderer->videodevice()->bindTexture(0,drawcall.m_pTexture);
			m_pRenderer->videodevice()->blendfunc(drawcall.m_SrcBlend,drawcall.m_DestBlend);
			m_pRenderer->videodevice()->drawPrimitives(video::Primitives_Triangles,
				*m_pVertexstream,drawcall.m_StartTriangle,drawcall.m_NumTriangles);
		}
		m_pRenderer->videodevice()->culling(true);
	}

	ion_uint32 Spritecache::numExecutableBlendedJobs()const
	{
		return 1;
	}

	ion_uint32 Spritecache::numExecutableBlendedLitJobs()const
	{
		return 1;
	}

	void Spritecache::executeBlendedLitJob(const ion_uint32 jobnr)
	{
		m_pRenderer->videodevice()->blending(true);
		for (std::vector < Drawcall >::iterator it=m_pDrawcalls->begin();it!=m_pDrawcalls->end();++it) {
			Drawcall& drawcall=(*it);
			m_pRenderer->videodevice()->bindTexture(0,drawcall.m_pTexture);
			m_pRenderer->videodevice()->drawPrimitives(video::Primitives_Triangles,
				*m_pVertexstream,drawcall.m_StartTriangle,drawcall.m_NumTriangles);
		}
		m_pRenderer->videodevice()->culling(true);
	}

	void Spritecache::restoreVideoResources()
	{
	}

	const char *Spritecache::type() const
	{
		return "Spritecache";
	}

	void Spritecache::respawn(base::Streamable& source)
	{
	}

	void Spritecache::serialize(base::Streamable& dest)
	{
	}

	bool Spritecache::isValid() const
	{
		return true;
	}

	void Spritecache::rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer)
	{
	}

	void Spritecache::age(const unsigned long ageamount)
	{
		std::vector < Entry >::iterator it=m_pEntries->begin();
		while (it!=m_pEntries->end()) {
			Entry &entry=(*it);

			if (entry.m_Age<=ageamount) {
				delete [] it->m_pTextures;
				m_pEntries->erase(it);
				it=m_pEntries->begin();
				continue;
			} else entry.m_Age-=ageamount;

			++it;
		}
	}

	ion_uint32 Spritecache::addSprite(const math::Vector3f& position,const math::Vector2f& size,video::Texture2D *pTexture,
		const video::BlendFunc srcblend,const video::BlendFunc destblend,const unsigned long initialAge)
	{
		if (m_Size<=m_pEntries->size()) return 0xffffffff;

		Entry entry;
		entry.m_Position=position;
		entry.m_Size=size;
		entry.m_pTextures=new video::Texture2D*[1];
		entry.m_pTextures[0]=pTexture;
		entry.m_SrcBlend=srcblend;
		entry.m_DestBlend=destblend;
		entry.m_Age=initialAge;
		entry.m_FPS=1;
		entry.m_CurSprite=0;
		entry.m_NumSprites=1;
		entry.m_LoopAnimation=true;

		m_pEntries->push_back(entry);

		return (ion_uint32)(m_pEntries->size()-1);
	}

	ion_uint32 Spritecache::addSprites(const math::Vector3f& position,const math::Vector2f& size,video::Texture2D **pTextures,
		const ion_uint32 numsprites,const ion_uint32 fps,const video::BlendFunc srcblend,const video::BlendFunc destblend,
		const unsigned long initialAge,const bool loopAnimation)
	{
		if (m_Size<=m_pEntries->size()) return 0xffffffff;

		Entry entry;
		entry.m_Position=position;
		entry.m_Size=size;
		entry.m_pTextures=new video::Texture2D*[numsprites];
		for (ion_uint32 i=0;i<numsprites;++i) entry.m_pTextures[i]=pTextures[i];
		entry.m_SrcBlend=srcblend;
		entry.m_DestBlend=destblend;
		entry.m_Age=initialAge;
		entry.m_FPS=fps;
		entry.m_CurSprite=0;
		entry.m_NumSprites=numsprites;
		entry.m_LoopAnimation=loopAnimation;

		m_pEntries->push_back(entry);

		return (ion_uint32)(m_pEntries->size()-1);
	}

	void Spritecache::removeSprite(const ion_uint32 spritenr)
	{
		if (spritenr<numSprites()) {
			delete [] (m_pEntries->begin()+spritenr)->m_pTextures;
			m_pEntries->erase(m_pEntries->begin()+spritenr);
		}
	}

	ion_uint32 Spritecache::numSprites() const
	{
		return (ion_uint32)(m_pEntries->size());
	}

	Spritecache::Entry* Spritecache::spriteEntry(const ion_uint32 spritenr)
	{
		return (spritenr<numSprites()) ? &(*(m_pEntries))[spritenr] : 0;
	}

	const Spritecache::Entry* Spritecache::spriteEntry(const ion_uint32 spritenr) const
	{
		return (spritenr<numSprites()) ? &(*(m_pEntries))[spritenr] : 0;
	}

	void Spritecache::clearList()
	{
		m_pEntries->clear();
	}

	bool Spritecache::sortEntries(const Entry& e1,const Entry& e2)
	{
		//return e1.m_pTexture<e2.m_pTexture;
		return e1.m_SquaredDistanceToEye<e2.m_SquaredDistanceToEye;
	}

	void Spritecache::update(const Camera& camera,const Updatemode mode)
	{
		m_pDrawcalls->clear();
		if (m_pEntries->size()<=0) return;

		math::Matrix4f m;
		m=camera.viewmatrix().invert();
		m(3,0)=m(3,1)=m(3,2)=0;

		std::vector < Entry >::iterator it;
		for (it=m_pEntries->begin();it!=m_pEntries->end();++it) {
			Entry &entry=(*it);
			entry.m_SquaredDistanceToEye=(entry.m_Position-camera.transform().position()).lengthSquared();
		}

		math::Vector3f minv,maxv;
		bool minmaxinitialized=false;

		//std::sort(m_pEntries->begin(),m_pEntries->end(),sortEntries);

		Drawcall curDrawcall;
		curDrawcall.m_NumTriangles=0;
		curDrawcall.m_StartTriangle=0;
		curDrawcall.m_pTexture=m_pEntries->at(0).m_pTextures[m_pEntries->at(0).m_CurSprite];
		curDrawcall.m_SrcBlend=m_pEntries->at(0).m_SrcBlend;
		curDrawcall.m_DestBlend=m_pEntries->at(0).m_DestBlend;
		ion_uint32 curTrianglePos=0;
		ion_uint32 vtxnr=0;

		m_pVertexstream->map(video::Map_Discard|video::Map_Writeonly);

		for (it=m_pEntries->begin();it!=m_pEntries->end();++it) {
			Entry &entry=(*it);
			if (entry.m_Age==0) continue;

			if ((entry.m_pTextures[entry.m_CurSprite]!=curDrawcall.m_pTexture) ||
			(entry.m_SrcBlend!=curDrawcall.m_SrcBlend) ||
			(entry.m_DestBlend!=curDrawcall.m_DestBlend)) {
				curDrawcall.m_NumTriangles=curTrianglePos-curDrawcall.m_StartTriangle;
				m_pDrawcalls->push_back(curDrawcall);
				curDrawcall.m_StartTriangle=curTrianglePos;
				curDrawcall.m_NumTriangles=0;
				curDrawcall.m_pTexture=entry.m_pTextures[entry.m_CurSprite];
				curDrawcall.m_SrcBlend=entry.m_SrcBlend;
				curDrawcall.m_DestBlend=entry.m_DestBlend;
			}

			math::Vector3f v[4];

			ion_uint32 i;

			for (i=0;i<4;++i) {
				const float signx[4]={-1, 1, 1,-1};
				const float signy[4]={ 1, 1,-1,-1};

				v[i]=entry.m_Position+m*(0.5f*math::Vector3f(entry.m_Size.x()*signx[i],entry.m_Size.y()*signy[i],0));
				//v[i]=entry.m_Position+0.5f*math::Vector3f(entry.m_Size.x()*signx[i],entry.m_Size.y()*signy[i],10);

				if (!minmaxinitialized) {
					minmaxinitialized=true;
					minv=v[i];
					maxv=v[i];
				} else {
					if (minv.x()>v[i].x()) minv.x()=v[i].x();
					if (minv.y()>v[i].y()) minv.x()=v[i].y();
					if (minv.z()>v[i].z()) minv.x()=v[i].z();

					if (maxv.x()<v[i].x()) maxv.x()=v[i].x();
					if (maxv.y()<v[i].y()) maxv.y()=v[i].y();
					if (maxv.z()<v[i].z()) maxv.z()=v[i].z();
				}
			}

			const int permutation[6]={0,1,2,2,3,0};
			const float tu[4]={0,1,1,0};
			const float tv[4]={1,1,0,0};

			for (i=0;i<6;++i) {
				m_pVertexstream->position(vtxnr+i,v[permutation[i]]);
				m_pVertexstream->diffuseColor(vtxnr+i,0xffffffff);
				m_pVertexstream->texcoord2D(vtxnr+i,0,tu[permutation[i]],tv[permutation[i]]);
				m_pVertexstream->texcoord3D(vtxnr+i,0,entry.m_Position);
			}

			curTrianglePos+=2;
			vtxnr+=6;
		}

		curDrawcall.m_NumTriangles=curTrianglePos-curDrawcall.m_StartTriangle;
		m_pDrawcalls->push_back(curDrawcall);

		m_pVertexstream->unmap();

		/*m_Sphere.center(this->transform().position()-(minv+maxv)*0.5f);
		m_Sphere.radius((maxv-minv).length()*0.5f);*/
	}

	const math::SphereVolume& Spritecache::boundingSphere() const
	{
		return m_Sphere;
	}

}
}
