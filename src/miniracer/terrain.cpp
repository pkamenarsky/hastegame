#include "../base/log.hh"
#include "../base/localfile.hh"
#include "../video/indexformat.hh"
#include "../video/indexiterator.hh"
#include "../video/vertexformat.hh"
#include "../video/vertexiterator.hh"
#include "../scene/renderjob.hh"
#include "../scene/renderjobqueue.hh"
#include "terrain.hh"


const char *serializefilename="heightmap1.ser";


void TerrainSerialize(void* serializeHandle, const void* buffer, size_t size)
{
	ion::base::Localfile *pLocalfile=(ion::base::Localfile *)serializeHandle;

	pLocalfile->write(buffer,(unsigned long)size);
}

void TerrainDeserialize(void* serializeHandle, void* buffer, size_t size)
{
	ion::base::Localfile *pLocalfile=(ion::base::Localfile *)serializeHandle;

	pLocalfile->read(buffer,(unsigned long)size);
}




Terrain::Terrain(const ion::base::String& identifier,NewtonWorld *pNewtonworld,ion::scene::Renderer& rRenderer,
		ion::base::Streamable& heightmap,const ion_uint32 width,const ion_uint32 depth,const ion_uint32 patchwidth,
		const ion_uint32 patchdepth,const ion::math::Vector3f& offset,const ion::math::Vector3f& size):
m_pNewtonworld(pNewtonworld),Node(identifier)
{
	ion_uint32 numpatchesX=(width-1)/(patchwidth-1),numpatchesZ=(depth-1)/(patchdepth-1);
	ion_uint16 *pHeightbuffer=new ion_uint16[(width+1)*(depth+1)];

	{
		ion_uint16 *pHLine=pHeightbuffer;
		for (ion_uint32 z=0;z<depth;++z) {
			heightmap.read(pHLine,width*sizeof(ion_uint16));

			// Copy the first height to the one extra height for correct normal vector calculations
			pHLine[width]=pHLine[0];

			pHLine+=(width+1);
		}
		// Copy the first line to the one extra line for correct normal vector calculations
		memcpy(pHeightbuffer+(width+1)*depth,pHeightbuffer,(width+1)*sizeof(ion_uint16));
	}

	const ion::math::Vector3f psize(
		size.x()/((float)numpatchesX),
		size.y(),
		size.z()/((float)numpatchesZ)
		);

	ion::math::Vector3f poffset(
		offset.x()/*-psize.x()*0.5f*/,
		offset.y(),
		offset.z()/*-psize.z()*0.5f*/
		);

	ion::base::Localfile serializefile;
	bool fileok=serializefile.open(serializefilename,"rb");
	if (fileok) {
		ion::base::log("Terrain::Terrain()",ion::base::Message) << "Using previously serialized terrain data\n";
		serializefile.open(serializefilename,"rb");
		m_pTreeCollision=NewtonCreateTreeCollisionFromSerialization(pNewtonworld,0,TerrainDeserialize,&serializefile);
		serializefile.close();
	} else {
		ion::base::log("Terrain::Terrain()",ion::base::Message) << "Calculating terrain data\n";
		m_pTreeCollision=NewtonCreateTreeCollision(pNewtonworld,0);
		NewtonTreeCollisionBeginBuild(m_pTreeCollision);
		{
			for (ion_uint32 z=0;z<(depth-1);++z) {
				/*float zz1=offset.z()+((float)z)/((float)(depth-1))*zsize;
				float zz2=offset.z()+((float)(z+1))/((float)(depth-1))*zsize;*/

				float zz1=offset.z()+( ((float)z)/((float)(patchdepth-1)) )*psize.z();
				float zz2=offset.z()+( ((float)(z+1))/((float)(patchdepth-1)) )*psize.z();

				unsigned int zT=(z/patchdepth)&1;
				for (ion_uint32 x=0;x<(width-1);++x) {
					float xx1=offset.x()+( ((float)x)/((float)(patchwidth-1)) )*psize.x();
					float xx2=offset.x()+( ((float)(x+1))/((float)(patchwidth-1)) )*psize.x();
					/*float xx1=offset.x()+((float)x)/((float)(width-1))*xsize;
					float xx2=offset.x()+((float)(x+1))/((float)(width-1))*xsize;*/

					float yy11=offset.y()+((float)(pHeightbuffer[x+z*(width+1)]))/65535.0f*size.y();
					float yy21=offset.y()+((float)(pHeightbuffer[x+1+z*(width+1)]))/65535.0f*size.y();
					float yy12=offset.y()+((float)(pHeightbuffer[x+(z+1)*(width+1)]))/65535.0f*size.y();
					float yy22=offset.y()+((float)(pHeightbuffer[x+1+(z+1)*(width+1)]))/65535.0f*size.y();

					float tri1[]={
						xx1,yy11,zz1,
						xx1,yy12,zz2,
						xx2,yy21,zz1
					};

					float tri2[]={
						xx2,yy21,zz1,
						xx1,yy12,zz2,
						xx2,yy22,zz2
					};

					unsigned int xT=(x/patchwidth)&1;

					unsigned int matID=((xT&zT)==1) ? 0 : (xT|zT);

					NewtonTreeCollisionAddFace(m_pTreeCollision,3,tri1,12,0);
					NewtonTreeCollisionAddFace(m_pTreeCollision,3,tri2,12,0);
				}
			}
		}
		NewtonTreeCollisionEndBuild(m_pTreeCollision,0);

		serializefile.open(serializefilename,"wb");
		NewtonTreeCollisionSerialize(m_pTreeCollision,TerrainSerialize,&serializefile);
		serializefile.close();
	}

	NewtonBody *pTerrainBody=NewtonCreateBody(m_pNewtonworld,m_pTreeCollision);
	NewtonReleaseCollision(m_pNewtonworld,m_pTreeCollision);
	NewtonBodySetMatrix(pTerrainBody,ion::math::Matrix4f::identitymatrix());

	for (ion_uint32 z=0;z<numpatchesZ;++z) {

		/*if (z==0) {
			heightmap.read(pHeightbuffer,sizeof(ion_uint16)*width*patchdepth);
		} else {
			memcpy(pHeightbuffer,pHeightbuffer+width*(patchdepth-1),width*sizeof(ion_uint16));
			heightmap.read(pHeightbuffer+width,sizeof(ion_uint16)*width*(patchdepth-1));
		}*/

		poffset.x()=offset.x()/*-psize.x()*0.5f*/;

		for (ion_uint32 x=0;x<numpatchesX;++x) {
			ion_uint16 *pH=pHeightbuffer+x*(patchwidth-1)+z*(width+1)*(patchdepth-1);
			TerrainPatch *pPatch=new TerrainPatch("tpatch",rRenderer,pH,patchwidth,width+1,patchdepth,
				(z==(numpatchesZ-1)),poffset,psize);

			poffset.x()+=psize.x();
			m_Patches.push_back(pPatch);
			addChild(*pPatch);
		}

		poffset.z()+=psize.z();
	}

	delete [] pHeightbuffer;
}

Terrain::~Terrain()
{
	NewtonReleaseCollision(m_pNewtonworld,m_pTreeCollision);
}

void Terrain::terrainTexture(ion::video::Texture2D *pTex)
{
	std::vector< TerrainPatch* >::iterator it=m_Patches.begin();
	for (;it!=m_Patches.end();++it)
		(*it)->terrainTexture(pTex);
}

const char* Terrain::type() const
{
	return "Terrain";
}

void Terrain::respawn(ion::base::Streamable& source)
{
}

void Terrain::serialize(ion::base::Streamable& dest)
{
}

std::vector< TerrainPatch* >& Terrain::patches()
{
	return m_Patches;
}

bool Terrain::isValid() const
{
	return true;
}







TerrainPatch::TerrainPatch(const ion::base::String& identifier,ion::scene::Renderer& rRenderer,
	const ion_uint16 *pHeightdata,const ion_uint32 width,const ion_uint32 pitch,const ion_uint32 depth,
	const bool lastrow,const ion::math::Vector3f& offset,const ion::math::Vector3f& size):Renderable(identifier),
	m_pTex(0)
{
	m_pRenderjob=new ion::scene::Renderjob(*this);

	ion::video::Vertexformat vf;
	vf.addEntry(ion::video::VertexFormatEntry_Position,ion::video::VertexFormatSemantic_Position);
	vf.addEntry(ion::video::VertexFormatEntry_Normal,ion::video::VertexFormatSemantic_Normal);
	vf.addEntry(ion::video::VertexFormatEntry_Texcoord2D,ion::video::VertexFormatSemantic_Texcoord);

	m_pVertices=rRenderer.videodevice()->createVertexstream(width*depth,vf,ion::video::Streamflags_Writeonly,ion::video::Mempool_Managed);
	m_pIndices=rRenderer.videodevice()->createIndexstream((width-1)*(depth-1)*6,
		(m_pVertices->capacity()>65536) ? ion::video::Indexformat_32bit : ion::video::Indexformat_16bit,
		ion::video::Streamflags_Writeonly,ion::video::Mempool_Managed);

	ion_uint32 x,z;

	position(offset);

	m_BoundingSphere.center(ion::math::Vector3f(size.x()*0.5f,0,size.z()*0.5f));
	m_BoundingSphere.radius(ion::math::Vector3f(size.x()*0.5f,size.y(),size.z()*0.5f).length());

	m_pVertices->map(ion::video::Map_Writeonly);
	ion::video::Vertexiterator v(*m_pVertices);
	for (z=0;z<depth;++z) {
		for (x=0;x<width;++x) {
			float xx=((float)x)/((float)(width-1))*size.x();
			float zz=((float)z)/((float)(depth-1))*size.z();

			float xx2=((float)(x+1))/((float)(width-1))*size.x();
			float zz2=((float)(z+1))/((float)(depth-1))*size.z();

			ion_uint32 x_1=x;
			ion_uint32 z_1=z;
			ion_uint32 x_2=(x+1);
			ion_uint32 z_2=(z+1);

			ion_uint16 h=pHeightdata[x_1+z_1*pitch];
			ion_uint16 h21=pHeightdata[x_2+z_1*pitch];
			ion_uint16 h12=pHeightdata[x_1+z_2*pitch];

			float yy=((float)h)/65535.0f*size.y();
			float yy21=((float)h21)/65535.0f*size.y();
			float yy12=((float)h12)/65535.0f*size.y();

			{
				ion::math::Vector3f vn(xx,yy,zz);
				ion::math::Vector3f vn21(xx2,yy21,zz);
				ion::math::Vector3f vn12(xx,yy12,zz2);

				ion::math::Vector3f n1((vn21-vn).normalizedVector());
				ion::math::Vector3f n2((vn12-vn).normalizedVector());

				v.normal((n2^n1).normalizedVector());
			}

			float tu=((float)(x*4))/((float)(width-1));
			float tv=((float)(z*4))/((float)(depth-1));

			v.position(xx,yy,zz);
			v.texcoord2D(0,tu,tv);
			++v;
		}
	}
	m_pVertices->unmap();

	m_pIndices->map(ion::video::Map_Writeonly);
	ion::video::Indexiterator i(*m_pIndices);
	for (z=0;z<(depth-1);++z) {
		for (x=0;x<(width-1);++x) {
			ion_uint32 i1=x+z*width;
			ion_uint32 i2=x+1+z*width;
			ion_uint32 i3=x+(z+1)*width;
			ion_uint32 i4=x+1+(z+1)*width;

			i=i1; ++i;
			i=i3; ++i;
			i=i2; ++i;

			i=i2; ++i;
			i=i3; ++i;
			i=i4; ++i;
		}
	}
	m_pIndices->unmap();

	m_Properties.addRef();
	m_Properties.addRef();
	m_Properties.add2DTexture("diffuseTexture",m_pTex);
	m_Properties.addBool("wireframe",false);

	m_pRenderjob->firstElement(0);
	m_pRenderjob->indexOffset(0);
	m_pRenderjob->numElements(m_pIndices->capacity()/3);
	m_pRenderjob->primitivestype(ion::video::Primitives_Triangles);
	m_pRenderjob->worldtransform()=transform();
	m_pRenderjob->indexstream(m_pIndices);
	m_pRenderjob->vertexstream(*m_pVertices);
	m_pRenderjob->propertytable(&m_Properties);
}

TerrainPatch::~TerrainPatch()
{
	delete m_pVertices;
	delete m_pIndices;
	delete m_pRenderjob;
}

const ion::math::SphereVolume& TerrainPatch::boundingSphere() const
{
	return m_BoundingSphere;
}

ion::math::SphereVolume& TerrainPatch::boundingSphere()
{
	return m_BoundingSphere;
}

void TerrainPatch::terrainTexture(ion::video::Texture2D *pTex)
{
	m_pTex=pTex;
	m_Properties.add2DTexture("diffuseTexture",m_pTex);
}

const char* TerrainPatch::type() const
{
	return "TerrainPatch";
}

void TerrainPatch::respawn(ion::base::Streamable& source)
{
}

void TerrainPatch::serialize(ion::base::Streamable& dest)
{
}

void TerrainPatch::visit(ion::scene::Renderjobqueue& queue) const
{
	m_pRenderjob->worldtransform()=transform();
	queue.addJob(*m_pRenderjob);
}

void TerrainPatch::restoreVideoResources()
{
}

bool TerrainPatch::isValid() const
{
	return true;
}

void TerrainPatch::rendererChanging(const bool videodeviceIsDifferent,ion::scene::Renderer *pNewRenderer)
{
}
