#ifndef _TRACE_H_
#define _TRACE_H_

#include "common.h"

//#define COLDET_PATCHES

class Trace
{
public:
	Trace(void);
	Trace(Q3BSP *v_bsp);

	void					SetBSP(Q3BSP *v_bsp);

	float					TraceBox(Vector3f start, Vector3f end, Vector3f mins, Vector3f maxs, Q3BSP *v_bsp = NULL, int flags = 1);
	float					TraceFacet(Vector3f v_start, Vector3f v_end, Vector3f v_mins, Vector3f v_maxs, Vector3f v1, Vector3f v2, Vector3f v3);

	Vector3f				&GetCollisionPoint(void);
	Plane					*GetCollisionPlane(void);
	Q3BSP::Surface			*GetCollisionSurface(void);
	float					GetFraction(void);

	ion_int32				GetModelnum(void);

	bool					StartSolid(void);

public:

	Vector3f				mins, maxs;
	Vector3f				p, start, end, col;

	bool					start_solid;
	bool					all_solid;

	float					col_frac;
	ion_int32				col_surface_index;
	ion_int32				col_model_num;

	Plane					*col_plane;

	Q3BSP					*BSP;

	const Q3BSP::BSPModel	*bsp_models;
	const Q3BSP::Node		*nodes;
	const Q3BSP::Leaf		*leaves;
	const Q3BSP::BSPVertex	*vertices;
	const Q3BSP::Face		*faces;
	const Q3BSP::Brush		*brushes;
	const Q3BSP::Brushside	*brushsides;
	const Q3BSP::Surface	*surfaces;
	const ion_int32			*leafbrushes;
	const ion_int32			*leaffaces;
	const Plane				*planes;

	ion_int32				num_bsp_models;

	void AddModel(ion_int32 model_num, bool cull);

	void AddLeafbrushes(void);
	void AddModelbrushes(ion_int32 model_num, bool cull);
	void AddLeafbrushNode(ion_int32 node_num);
};

void SetColdetBSP(Q3BSP *v_bsp);

template <class T> static int CheckAABB(Vector3f mins1, Vector3f maxs1, const T mins2, const T maxs2);

#endif