#include <string.h>
#include <string>
#include "../base/log.hh"
#include "iscloader.hh"

namespace ion {
namespace scene {

	struct ISCObj
	{
		ion_uint32 m_Meshindex;
		RenderableMeshNode *m_pMeshNode;
	};

	struct ISCMesh
	{
		std::vector<ion_uint32> m_MatIndices;
		RenderableMesh* m_pMesh;
	};

	Group* loadISC(base::LocalStreamable& streamable,Renderer& renderer,Renderablelist* pRenderablelist,Node *pParent,
		base::Refcountedsource< video::Texture2D > *pTexturesource,
		base::Refcountedsource< Shader > *pShadersource
		)
	{

		std::vector < ISCObj > iscobjects;
		std::vector < ISCMesh > iscmeshes;
		std::vector < video::Propertytable* > materials;
		std::vector < Shader* > shaders;
	
		streamable.raw();

		char fourcc[5]; fourcc[4]=0;
	
		streamable.read(fourcc,4);

		if (strcmp("isc0",fourcc)!=0) {
			base::log("scene::loadISC()",base::Error) << "Source \"" << streamable.name() << "\" is not of ISC format!\n";
			return 0;
		}

		Group *pGroup=new Group(streamable.name()); // TODO: explicit naming useful? this has to be clarified
		if (pParent) pParent->addChild(*pGroup);

		while (!streamable.eof()) {
			ion_uint32 chunksize;
			std::string chunkname;

			streamable.read(fourcc,4); chunkname=fourcc; streamable >> chunksize;
			
			base::logstream() << "Chunk " << chunkname << " with " << chunksize << " byte\n";

			if (chunkname=="objt") {
				ion_uint32 numobjs;
				streamable >> numobjs;

				for (ion_uint32 objnum=0;objnum<numobjs;++objnum) {
					ISCObj iscobj;

					float rotation[4],position[3];

					base::String objname;
					streamable >> objname;

					streamable >> position[0] >> position[1] >> position[2];
					streamable >> rotation[0] >> rotation[1] >> rotation[2] >> rotation[3];
					streamable >> iscobj.m_Meshindex;

					iscobj.m_pMeshNode=new RenderableMeshNode(objname);
					pGroup->addChild(*(iscobj.m_pMeshNode));
					if (pRenderablelist/* && (objnum==14)*/) pRenderablelist->addRenderable(*(iscobj.m_pMeshNode));
					iscobj.m_pMeshNode->position(position[0],position[1],position[2]);
					iscobj.m_pMeshNode->rotation(math::Quaternion(rotation[0],rotation[1],rotation[2],rotation[3]).normalized());

					//base::logstream() << "objpos " << position[0] << " " << position[1] << " " << position[2] << "\n";

					iscobjects.push_back(iscobj);
				}

			} else if (chunkname=="mesh") {
				ion_uint32 nummeshes;
				streamable >> nummeshes;

				for (ion_uint32 meshnum=0;meshnum<nummeshes;++meshnum) {
					ISCMesh iscmesh;

					base::String meshname;
					streamable >> meshname;

					ion_uint32 numtriangles,numvertices;
					streamable >> numtriangles >> numvertices;

					video::Vertexformat vf;
					vf.addEntry(video::VertexFormatEntry_Position,video::VertexFormatSemantic_None);
					vf.addEntry(video::VertexFormatEntry_Normal,video::VertexFormatSemantic_None);
					vf.addEntry(video::VertexFormatEntry_Texcoord2D,video::VertexFormatSemantic_Texcoord);
					vf.addEntry(video::VertexFormatEntry_Texcoord3D,video::VertexFormatSemantic_Tangent);
					vf.addEntry(video::VertexFormatEntry_Texcoord3D,video::VertexFormatSemantic_Binormal);

					iscmesh.m_pMesh=new RenderableMesh(meshname,renderer); // TODO: source mesh & identifier
					iscmesh.m_pMesh->reinit(numtriangles,numvertices,vf,
						(numvertices<65536) ? video::Indexformat_16bit : video::Indexformat_32bit);
					iscmesh.m_pMesh->addRef(); // needed for removal of unused meshes; see above

					video::Vertexstream& vertexstream=iscmesh.m_pMesh->vertexstream();
					video::Indexstream& indexstream=iscmesh.m_pMesh->indexstream();
					video::Attributebuffer& attributebuffer=iscmesh.m_pMesh->attributebuffer();

					ion_uint32 tri,vtx;

					indexstream.map(video::Map_Writeonly);
					for (tri=0;tri<numtriangles;++tri) {
						ion_uint32 idx[3];
						streamable >> idx[0] >> idx[1] >> idx[2];
						indexstream.index(tri*3+0,idx[0]);
						indexstream.index(tri*3+1,idx[1]);
						indexstream.index(tri*3+2,idx[2]);
					}
					indexstream.unmap();

					for (tri=0;tri<numtriangles;++tri) {
						ion_uint32 attrib; streamable >> attrib;
						attributebuffer[tri]=attrib;
					}

					math::Vector3f minvtx,maxvtx;

					vertexstream.map(video::Map_Writeonly);
					for (vtx=0;vtx<numvertices;++vtx) {
						float xyz[3],n[3],uv[2],t[3],b[3];

						streamable >> xyz[0] >> xyz[1] >> xyz[2];
						streamable >> n[0] >> n[1] >> n[2];
						streamable >> uv[0] >> uv[1];
						streamable >> t[0] >> t[1] >> t[2];
						streamable >> b[0] >> b[1] >> b[2];

						/*xyz[0]*=0.1f;
						xyz[1]*=0.1f;
						xyz[2]*=0.1f;*/

						if (vtx==0) {
							minvtx.x()=maxvtx.x()=xyz[0];
							minvtx.y()=maxvtx.y()=xyz[1];
							minvtx.z()=maxvtx.z()=xyz[2];
						} else {
							if (minvtx.x()>xyz[0]) minvtx.x()=xyz[0];
							if (minvtx.y()>xyz[1]) minvtx.y()=xyz[1];
							if (minvtx.z()>xyz[2]) minvtx.z()=xyz[2];

							if (maxvtx.x()<xyz[0]) maxvtx.x()=xyz[0];
							if (maxvtx.y()<xyz[1]) maxvtx.y()=xyz[1];
							if (maxvtx.z()<xyz[2]) maxvtx.z()=xyz[2];
						}

						vertexstream.position(vtx,xyz[0],xyz[1],xyz[2]);
						vertexstream.normal(vtx,n[0],n[1],n[2]);
						vertexstream.texcoord2D(vtx,0,uv[0],uv[1]);
						vertexstream.texcoord3D(vtx,0,t[0],t[1],t[2]);
						vertexstream.texcoord3D(vtx,1,b[0],b[1],b[2]);
					}
					vertexstream.unmap();

					RenderableMesh &mesh=*(iscmesh.m_pMesh);

					mesh.attributetable().recalculate(mesh.attributebuffer());

					mesh.boundingSphere().center( (maxvtx+minvtx)*0.5f );
					mesh.boundingSphere().radius( (maxvtx-minvtx).length()*0.5f+0000.0f );
					
					/*base::logstream() << "mesh " << mesh.boundingSphere().radius() << "\n";
					base::logstream() << "minvtx " << minvtx.x() << " " << minvtx.y() << " " << minvtx.z() << "\n";
					base::logstream() << "maxvtx " << maxvtx.x() << " " << maxvtx.y() << " " << maxvtx.z() << "\n";
					base::logstream() << "center "
						<< mesh.boundingSphere().center().x() << " "
						<< mesh.boundingSphere().center().y() << " "
						<< mesh.boundingSphere().center().z() << "\n";*/

					iscmeshes.push_back(iscmesh);

				}

			} else if (chunkname=="mtrl") {
				//streamable.seek(chunksize,base::LocalStreamable::SeekRelative);
				ion_uint32 nummtrls;
				streamable >> nummtrls;

				for (ion_uint32 mtrlnum=0;mtrlnum<nummtrls;++mtrlnum) {
					base::String mtrlname,shadername;
					ion_uint32 numprops;
					streamable >> mtrlname >> shadername >> numprops;
					
					// TODO: support shaders
					if ((pShadersource!=0) && !shadername.empty()) {
						Shader *pShader=pShadersource->get(shadername);
						shaders.push_back(pShader);
					} else shaders.push_back(0); // insert nullpointers to preserve order

					if (numprops>0) {
						video::Propertytable *pProperties=new video::Propertytable;
						for (ion_uint32 propnum=0;propnum<numprops;++propnum) {
							ion_uint8 proptype;
							base::String propname;
							streamable >> proptype >> propname;

							switch (proptype) {

								case 1: {
									float f; streamable >> f;
									pProperties->addFloat(propname,f);
								} break;

								case 2: {
									base::Float2 f; streamable >> f[0] >> f[1];
									pProperties->addFloat2(propname,f);
								} break;

								case 3: {
									base::Float3 f; streamable >> f[0] >> f[1] >> f[2];
									pProperties->addFloat3(propname,f);
								} break;

								case 4: {
									base::Float4 f; streamable >> f[0] >> f[1] >> f[2] >> f[3];
									pProperties->addFloat4(propname,f);
								} break;

								case 5: {
									base::Float3x3 f;
									for (int i=0;i<3*3;++i) streamable >> f[i];
									pProperties->addFloat3x3(propname,f);
								} break;

								case 6: {
									base::Float4x3 f;
									for (int i=0;i<4*3;++i) streamable >> f[i];
									pProperties->addFloat4x3(propname,f);
								} break;

								case 7: {
									base::Float3x4 f;
									for (int i=0;i<3*4;++i) streamable >> f[i];
									pProperties->addFloat3x4(propname,f);
								} break;

								case 8: {
									base::Float4x4 f;
									for (int i=0;i<4*4;++i) streamable >> f[i];
									pProperties->addFloat4x4(propname,f);
								} break;

								case 10:	// 2D texture
								{
									base::String mapname;
									streamable >> mapname;
									
									if (pTexturesource) {
										video::Texture2D *pTex=pTexturesource->get(mapname);
										pProperties->add2DTexture(propname,pTex);
									}
								} break;

								case 9:		// 1D texture
								case 11:	// 3D texture
								case 12:	// Cubemap
								{
									base::String mapname;
									streamable >> mapname;
									// TODO: texture loading
								} break;

								case 13: {
									bool b; ion_uint8 bb; streamable >> bb; b=(bb!=0);
									pProperties->addBool(propname,b);
								} break;

								case 14: {
									ion_int32 i32; streamable >> i32;
									pProperties->addInt(propname,i32);
								} break;

								default:break;
							}
						}

						materials.push_back(pProperties);
					}
				}
			} else {
				base::log("scene::loadISC()",base::Warning) << "Source \"" << streamable.name() << "\" has unknown chunk " << chunkname << " with " << chunksize << " byte - skipping\n";
				streamable.seek(chunksize,base::LocalStreamable::SeekRelative);
			}
		}

		{
			for (std::vector < ISCObj >::iterator itobj=iscobjects.begin();itobj!=iscobjects.end();++itobj) {
				ISCObj& obj=(*itobj);
				if (obj.m_Meshindex<iscmeshes.size())
					obj.m_pMeshNode->renderableMesh(*(iscmeshes[obj.m_Meshindex].m_pMesh));

				const scene::RenderableMesh *pRMesh=obj.m_pMeshNode->renderableMesh();
				for (ion_uint32 matnum=0;matnum<pRMesh->attributetable().numArrays();++matnum) {
					
					ion_uint32 attrib=pRMesh->attributetable().array(matnum)->faceattribute();
					video::Propertytable *pProps=(attrib<materials.size()) ? materials[attrib] : 0;
					Shader *pShader=(attrib<shaders.size()) ? shaders[attrib] : 0;

					obj.m_pMeshNode->materialProperties(matnum,pProps);
					obj.m_pMeshNode->shader(matnum,pShader);
				}
			}

			// Calling release() on all meshes; this way, unused meshes are deleted. The
			// obj.m_pMeshNode->renderableMesh() call increases the refcount, therefore meshes that are used have
			// a refcount of 2, unused ones 1 (the additional refcount comes from the addRef() below).
			// This release() call decreases the refcount, resulting in 1 for used meshes and 0 for unused ones
			// -> at refcount zero release() calls the delete operator, removing the unused meshes.
			for (std::vector < ISCMesh >::iterator itmesh=iscmeshes.begin();itmesh!=iscmeshes.end();++itmesh)
				(*itmesh).m_pMesh->release();
		}

		return pGroup;
	}

}
}
