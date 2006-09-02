/********************************************************************
POLYLIB.H
********************************************************************/

#ifndef _POLYLIB_H_
#define _POLYLIB_H_

#include "vec.h"
#include "typedefs.h"
#include "apd.h"

#define MAX_POINTS_ON_WINDING	128
#define MAX_BRUSH_WINDINGS		32

#define WORLD_MAX_COORDS 65535
#define WORLD_MIN_COORDS -65535

#define POLY_CROSS	0
#define POLY_FRONT	1
#define POLY_BACK	2
#define POLY_ON		3
#define POLY_NONE	4

#define PLANE_EPSILON	0.01f

#define	BOGUS_RANGE		8000

/*
typedef struct
{
	float normal[3], dist;
}plane_t;
*/

typedef struct
{
	plane_t plane;
	int numpoints;
	
	vec3f points[MAX_POINTS_ON_WINDING];
	int indices[MAX_POINTS_ON_WINDING];
}winding_t;


int		SplitWindingByPlane (winding_t * winding, plane_t * plane, winding_t * front, winding_t * back, float epsilon);
void	CopyWinding (winding_t * src, winding_t * dst);
int		CompareWindings (winding_t * w1, winding_t * w2);

void	CopyPlane (plane_t * src, plane_t * dst);
void	MakeWindingFromPlane (plane_t * plane, winding_t * winding);
void	MakePlaneFrom3Points (vec3f p1, vec3f p2, vec3f p3, plane_t * plane);
void	CorrectWinding(int type, winding_t *w);
int		SamePlane (plane_t * p1, plane_t * p2);
void	PlaneInvert (plane_t * plane);

int		SamePoint (vec3f p1, vec3f p2);

#endif
