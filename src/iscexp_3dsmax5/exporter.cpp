#include <windows.h>
#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include "Max.h"
#include "stdmat.h"
#include "decomp.h"
#include "../math/meshconv.hh"

// Max interface code

int controlsInit = FALSE;

HINSTANCE hInstance;







class SceneEnumerator:public ITreeEnumProc
{
public:

	SceneEnumerator(ExpInterface *ei, Interface *i);
	~SceneEnumerator();

	int callback( INode *node );

	inline size_t numTriobjects() const { return m_Objects.size(); }
	inline TriObject* triobject(const size_t index) {
		return (index>=m_Objects.size()) ? 0 : m_Objects[index].m_pTriobject;
	}
	inline Matrix3* triobjecttransform(const size_t index) {
		return (index>=m_Objects.size()) ? 0 : &(m_Objects[index].m_Transform);
	}

	inline std::string triobjectname(const size_t index) {
		return (index>=m_Objects.size()) ? std::string("") : m_Objects[index].m_Name;
	}

	inline Mtl* triobjectmaterial(const size_t index) {
		return (index>=m_Objects.size()) ? 0 : m_Objects[index].m_pMtl;
	}

	inline size_t numMaterials() const { return m_Materials.size(); }
	inline Mtl* material(const size_t index) {
		return (index>=m_Materials.size()) ? 0 : m_Materials[index];
	}
	inline size_t findMaterial(Mtl* pMtl) {
		std::vector <Mtl*>::iterator it=std::find(m_Materials.begin(),m_Materials.end(),pMtl);
		return (it!=m_Materials.end()) ? (it-m_Materials.begin()) : 0xffffffff;
	}

protected:
	void parseNode(INode *node);

	struct Triobjentry
	{
		TriObject *m_pTriobject;
		std::string m_Name;
		bool m_CanBeDeleted;
		Matrix3 m_Transform;
		Mtl *m_pMtl;
	};

	ExpInterface *m_pEI;
	Interface *m_pInterface;
	std::vector <Triobjentry> m_Objects;
	std::vector <Mtl*> m_Materials;
};

SceneEnumerator::SceneEnumerator(ExpInterface *ei, Interface *i):m_pEI(ei),m_pInterface(i)
{
	ei->theScene->EnumTree(this);
}

SceneEnumerator::~SceneEnumerator()
{
	for (std::vector <Triobjentry>::iterator it=m_Objects.begin();it!=m_Objects.end();++it) {
		if ((*it).m_CanBeDeleted) (*it).m_pTriobject->DeleteMe();
	}
}


void SceneEnumerator::parseNode(INode *node)
{
	Object *obj=node->EvalWorldState(m_pInterface->GetTime()).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID,0))) { 
		Triobjentry entry;
		entry.m_Name=node->NodeName();
		entry.m_Transform=node->GetObjTMAfterWSM(0);

		entry.m_pTriobject=(TriObject *)(obj->ConvertToType(m_pInterface->GetTime(),Class_ID(TRIOBJ_CLASS_ID, 0)));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		entry.m_CanBeDeleted=(obj!=entry.m_pTriobject);
		entry.m_pMtl=node->GetMtl();

		if (entry.m_pMtl!=0) {
			std::vector <Mtl*>::iterator it=std::find(m_Materials.begin(),m_Materials.end(),entry.m_pMtl);
			if (it==m_Materials.end()) {
				m_Materials.push_back(entry.m_pMtl);
			}
		}

		if (entry.m_pTriobject) m_Objects.push_back(entry);
	}/* else if (obj->CanConvertToType(Class_ID(MATERIAL_CLASS_ID,0))) {
		Mtl *pMtl=(Mtl*)obj;
		if (pMtl) m_Materials.push_back(pMtl);
	}*/
}

int SceneEnumerator::callback( INode *node )
{
	parseNode(node);

	return TREE_CONTINUE;
}








class ISCExport:public SceneExport
{
public:
	ISCExport();
	~ISCExport();

	int ExtCount() { return 1; }
	const TCHAR *Ext(int i) { return "isc"; }
	const TCHAR *LongDesc() { return "ion engine ISC model"; }
	const TCHAR *ShortDesc() { return "ion ISC"; }
	const TCHAR *AuthorName() { return "dv"; }
	const TCHAR *CopyrightMessage() { return "2005 by dv"; }
	const TCHAR *OtherMessage1() { return ""; }
	const TCHAR *OtherMessage2() { return ""; }
	unsigned int Version() { return 1; }
	void ShowAbout(HWND hWnd) {}
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

protected:
	Point3 getVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	void saveMesh(FILE *file,TriObject *pTriobj,const Matrix3* pM,Mtl *pMeshMtl,SceneEnumerator& enumerator);
	void saveMaterial(FILE *file,Mtl* pMtl);
	void saveISC(const TCHAR *name,ExpInterface *ei,Interface *i);
};

ISCExport::ISCExport()
{
}

ISCExport::~ISCExport()
{
}

int ISCExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	bool exportSelected=(options&SCENE_EXPORT_SELECTED)!=0;

	try {
		saveISC(name,ei,i);
	} catch (std::exception &e) {
		DebugPrint("Exception in ISC exporter: %s\n",e.what());
		return IMPEXP_FAIL;
	}

	return IMPEXP_SUCCESS;
}

void ISCExport::saveISC(const TCHAR *name,ExpInterface *ei,Interface *i)
{
	SceneEnumerator enumerator(ei,i);

	// No triobjects to export -> quit
	if (enumerator.numTriobjects()==0) return;

	size_t objnum=0;
	unsigned long chunksize,namessize=0;

	FILE *outfile=fopen(name,"wb");
	if (!outfile) return;

	for (objnum=0;objnum<enumerator.numTriobjects();++objnum)
		namessize+=(unsigned long)(enumerator.triobjectname(objnum).length())+1;


	fwrite("isc0",1,4,outfile);


	fwrite("objt",1,4,outfile);
	chunksize=(unsigned long)(enumerator.numTriobjects()*(3*sizeof(float)+4*sizeof(float)+4))+namessize;
	fwrite(&chunksize,1,4,outfile);
	{ DWORD numobjs=(DWORD)(enumerator.numTriobjects()); fwrite(&numobjs,1,4,outfile); }

	for (objnum=0;objnum<enumerator.numTriobjects();++objnum) {
		TriObject *pTriobj=enumerator.triobject(objnum);
		std::string objname=enumerator.triobjectname(objnum);
		Matrix3 *pTM=enumerator.triobjecttransform(objnum);

		/*Point3 trans=pTM->GetTrans();
		Quat rot(*pTM);*/
		AffineParts parts;
		decomp_affine(*pTM,&parts);
		Point3 trans=parts.t;
		Quat rot=parts.q;

		//float ang[3];
		//QuatToEuler(rot,ang);
		/*rot.GetEuler(&ang[0],&ang[1],&ang[2]);
		//{ float f=ang[1]; ang[1]=ang[2]; ang[2]=f; }
		rot.SetEuler(ang[0],ang[2],ang[1]);*/

		trans.x=trans.y=trans.z=0;
		rot.Identity();

		fwrite(objname.c_str(),1,objname.length()+1,outfile);

		//trans.x*=-1;

		fwrite(&(trans.x),1,sizeof(float),outfile);
		fwrite(&(trans.y),1,sizeof(float),outfile);
		fwrite(&(trans.z),1,sizeof(float),outfile);

		fwrite(&(rot.x),1,sizeof(float),outfile);
		fwrite(&(rot.y),1,sizeof(float),outfile);
		fwrite(&(rot.z),1,sizeof(float),outfile);
		fwrite(&(rot.w),1,sizeof(float),outfile);

		fwrite(&objnum,1,4,outfile);
	}


	fwrite("mesh",1,4,outfile);
	chunksize=(unsigned long)(enumerator.numTriobjects()*(2*4 + 3*4 + 4 + (3+3+2+3+3)*sizeof(float)))+namessize;
	fwrite(&chunksize,1,4,outfile);
	{ DWORD numobjs=(DWORD)(enumerator.numTriobjects()); fwrite(&numobjs,1,4,outfile); }

	for (objnum=0;objnum<enumerator.numTriobjects();++objnum) {
		Matrix3 *pTM=enumerator.triobjecttransform(objnum);

		TriObject *pTriobj=enumerator.triobject(objnum);
		std::string objname=enumerator.triobjectname(objnum);
		fwrite(objname.c_str(),1,objname.length()+1,outfile);
		saveMesh(outfile,pTriobj,pTM,enumerator.triobjectmaterial(objnum),enumerator);
	}


	fwrite("mtrl",1,4,outfile);
	chunksize=0;
	fwrite(&chunksize,1,4,outfile);
	{ DWORD nummtrls=(DWORD)(enumerator.numMaterials()); fwrite(&nummtrls,1,4,outfile); }

	for (size_t mtrlnum=0;mtrlnum<enumerator.numMaterials();++mtrlnum) {
		Mtl* pMtl=enumerator.material(mtrlnum);
		saveMaterial(outfile,pMtl);
	}


	fclose(outfile);
}

void ISCExport::saveMaterial(FILE *file,Mtl* pMtl)
{
	std::string mtlname(pMtl->GetName());
	fwrite(mtlname.c_str(),1,mtlname.length()+1,file);

	{ const char *shader=""; fwrite(shader,1,strlen(shader)+1,file); }
	DWORD dw;
	BYTE b;
	const char *attrname;

	dw=3;
	Texmap *pDiffuseTex=pMtl->GetSubTexmap(ID_DI);
	if (pDiffuseTex)
		if (pDiffuseTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
			++dw;

	fwrite(&dw,1,sizeof(dw),file);	// min. 3 properties

#define WRITECOLOR(type,name,func) \
	{ b=(type); fwrite(&b,1,sizeof(b),file);						\
	attrname=name; fwrite(attrname,1,strlen(attrname)+1,file);		\
	fwrite(&(pMtl->##func().r),1,sizeof(float),file);			\
	fwrite(&(pMtl->##func().g),1,sizeof(float),file);			\
	fwrite(&(pMtl->##func().b),1,sizeof(float),file); }

	WRITECOLOR(3,"ambientColor",GetAmbient);
	WRITECOLOR(3,"diffuseColor",GetDiffuse);
	WRITECOLOR(3,"specularColor",GetSpecular);
#undef WRITECOLOR

	if (pDiffuseTex) {
		if (pDiffuseTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)) {
			b=10; fwrite(&b,1,sizeof(b),file);
			TSTR mapName = ((BitmapTex *)pDiffuseTex)->GetMapName();
			attrname="diffuseTexture"; fwrite(attrname,1,strlen(attrname)+1,file);
			fwrite(mapName,1,strlen(mapName)+1,file);
		}
	}
}

void ISCExport::saveMesh(FILE *file,TriObject *pTriobj,const Matrix3* pM,Mtl *pMeshMtl,SceneEnumerator& enumerator)
{
	ion::math::Meshconv msh(true,false);
	Mesh &maxmesh=pTriobj->GetMesh();

	DWORD meshMtlID=(pMeshMtl!=0) ? (DWORD)(enumerator.findMaterial(pMeshMtl)) : 0;

	bool negScale = (pM) ? (DotProd(CrossProd(pM->GetRow(0),pM->GetRow(1)),pM->GetRow(2))>0.0) : false;

	int vx[3];

	if (negScale) {
		vx[0]=2;
		vx[1]=1;
		vx[2]=0;
	} else {
		vx[0]=0;
		vx[1]=1;
		vx[2]=2;
	}

	{
		// TODO: Normals

		/*ion::math::Meshconv::InNormal n;
		n.m_X=1;
		n.m_Y=n.m_Z=0;
		msh.addNormal(n);*/

		for (int facenr=0;facenr<maxmesh.getNumFaces();++facenr) {
			ion::math::Meshconv::InTriangle intr;

			Face *pVFace=&maxmesh.faces[facenr];

			intr.m_XYZ[0]=pVFace->v[vx[0]];
			intr.m_XYZ[1]=pVFace->v[vx[1]];
			intr.m_XYZ[2]=pVFace->v[vx[2]];

			if (maxmesh.tvFace!=0) {
				TVFace *pTVFace=&maxmesh.tvFace[facenr];

				intr.m_Texcoords2D[0]=pTVFace->t[vx[0]];
				intr.m_Texcoords2D[1]=pTVFace->t[vx[1]];
				intr.m_Texcoords2D[2]=pTVFace->t[vx[2]];
			} else {
				intr.m_Texcoords2D[0]=0;
				intr.m_Texcoords2D[1]=0;
				intr.m_Texcoords2D[2]=0;
			}

			intr.m_Normals[0]=pVFace->v[vx[0]];
			intr.m_Normals[1]=pVFace->v[vx[1]];
			intr.m_Normals[2]=pVFace->v[vx[2]];

			msh.addTriangle(intr);
		}

		if (maxmesh.normalsBuilt==0)
			maxmesh.buildNormals();

		for (int vtxnr=0;vtxnr<maxmesh.getNumVerts();++vtxnr) {
			Point3 *pVtx=&maxmesh.verts[vtxnr];

			Point3 vtx2;
			if (pM)
				vtx2=pM->PointTransform(*pVtx);
			else vtx2=*pVtx;

			// Swapping y and z to correct the coordinate system
			ion::math::Meshconv::InVtx invtx;
			invtx.m_X=vtx2.x;
			invtx.m_Y=vtx2.z;
			invtx.m_Z=vtx2.y;

			//vn =getVertexNormal(&maxmesh, i, maxmesh.getRVertPtr(vert));
			Point3 vn=maxmesh.getNormal(vtxnr);
			float vnl=sqrtf(vn.x*vn.x+vn.y*vn.y+vn.z*vn.z);
			if (vnl!=0) vnl=1.0f/vnl;
			ion::math::Meshconv::InNormal n;
			n.m_X=vn.x*vnl;
			n.m_Y=vn.z*vnl;
			n.m_Z=vn.y*vnl;

			msh.addXYZVtx(invtx);
			msh.addNormal(n);
		}

		for (int tuvnr=0;tuvnr<maxmesh.getNumTVerts();++tuvnr) {
			UVVert *pUV=&maxmesh.tVerts[tuvnr];

			// TODO: need to check whether UV, VW, or WU is used
			ion::math::Meshconv::InTexcoord2D intexcoord;
			intexcoord.m_U=pUV->x;
			intexcoord.m_V=pUV->y;

			msh.addTexcoord2D(intexcoord);
		}

		msh.calculate();

	}


	{ DWORD numTris=msh.numOutTriangles(); fwrite(&numTris,1,4,file); }
	{ DWORD numVtx=msh.numOutVertices(); fwrite(&numVtx,1,4,file); }

	// Indices
	{
		for (unsigned long trinr=0;trinr<msh.numOutTriangles();++trinr) {
			const ion::math::Meshconv::OutTriangle &tri=msh.outTriangle(trinr);

			fwrite(&tri.m_Indices[0],1,4,file);
			fwrite(&tri.m_Indices[1],1,4,file);
			fwrite(&tri.m_Indices[2],1,4,file);
		}
	}

	// Face attributes
	// TODO: Read the attributes from max
	{
		for (unsigned long trinr=0;trinr<msh.numOutTriangles();++trinr) {
			//DWORD id=maxmesh.faces[trinr].getMatID();
			fwrite(&meshMtlID,1,4,file);
		}
	}

	// Vertices
	{
		for (unsigned long vtxnr=0;vtxnr<msh.numOutVertices();++vtxnr) {
			const ion::math::Meshconv::OutVertex &vtx=msh.outVertex(vtxnr);

			float tangent[3]={1,0,0};
			float binormal[3]={0,1,0};

			fwrite(&vtx.m_XYZ.m_X,1,sizeof(float),file);
			fwrite(&vtx.m_XYZ.m_Y,1,sizeof(float),file);
			fwrite(&vtx.m_XYZ.m_Z,1,sizeof(float),file);
			fwrite(&vtx.m_Normal.m_X,1,sizeof(float),file);
			fwrite(&vtx.m_Normal.m_Y,1,sizeof(float),file);
			fwrite(&vtx.m_Normal.m_Z,1,sizeof(float),file);
			fwrite(&vtx.m_Texcoord.m_U,1,sizeof(float),file);
			fwrite(&vtx.m_Texcoord.m_V,1,sizeof(float),file);
			fwrite(tangent,1,3*sizeof(float),file);
			fwrite(binormal,1,3*sizeof(float),file);
		}
	}

}

Point3 ISCExport::getVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}









BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	hInstance = hinstDLL;

	if ( !controlsInit ) {
		controlsInit = TRUE;

		// jaguar controls
		InitCustomControls(hInstance);

		InitCommonControls();
	}
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			//MessageBox(NULL,_T("AIEXP.DLL: DllMain"),_T("AIEXP"),MB_OK);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return(TRUE);
}



//------------------------------------------------------

class ISCClassDesc:public ClassDesc {
public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE) { return new ISCExport; }
	const TCHAR *	ClassName() { return "ion engine"; }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return Class_ID(0x1a5d641c, 0x77092034); }
	const TCHAR* 	Category() { return "Scene Export";  }
};

static ISCClassDesc ISCDesc;



extern "C" {

	__declspec( dllexport ) const TCHAR * LibDescription() { return "ion ISC model exporter"; }

	__declspec( dllexport ) int  LibNumberClasses() { return 1; }

	__declspec( dllexport ) ClassDesc * LibClassDesc(int i) {
		switch(i) {
			case 0: return &ISCDesc; break;
			default: return 0; break;
		}
	}

	// Return version so can detect obsolete DLLs
	__declspec( dllexport ) ULONG LibVersion() { return VERSION_3DSMAX; }

	// Let the plug-in register itself for deferred loading
	__declspec( dllexport ) ULONG CanAutoDefer()
	{
		return 1;
	}

}
