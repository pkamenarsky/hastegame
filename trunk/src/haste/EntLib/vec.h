/********************************************************************
VEC.H
********************************************************************/

#ifndef _VEC_H_
#define _VEC_H_

#include <string.h>
#include <math.h>
#include "../../../common/apd.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

typedef float vec3f[3];
typedef float vec4f[4];

typedef float mat3x3[3][3];
typedef float mat4x4[4][4];

#define DEG2RAD(x) ((x) * 3.14159265f / 180.0f)
/*
typedef struct
{
	float	normal[3];
	float	dist;
}plane_t;
*/
#ifdef MACRO_VEC_FUNCTIONS

//it's possible to make functions and optimize them for ISSE or 
//3DNOW!, but these macros are type-independent, i.e. you can make 
//f.e. a dot product with a float and an int vector

float lenght;

#define VecCopy(src, dst) { (dst[0])=(src[0]); (dst[1])=(src[1]); (dst[2])=(src[2]); }

#define VecSet(vec, x, y, z) { (vec[0])=x; (vec[1])=y; (vec[2])=z; }

#define VecDot(vec1, vec2)  (vec1[0])*(vec2[0])+(vec1[1])*(vec2[1])+(vec1[2])*(vec2[2]);

#define VecAdd(op1, op2, dst) { (dst[0])=(op1[0])+(op2[0]);\
							  (dst[1])=(op1[1])+(op2[1]);\
							  (dst[2])=(op1[2])+(op2[2]); }

#define VecSub(op1, op2, dst) { (dst[0])=(op1[0])-(op2[0]);\
							  (dst[1])=(op1[1])-(op2[1]);\
							  (dst[2])=(op1[2])-(op2[2]); }

#define VecMulScalar(vec, scalar, dst) { (dst[0])=(vec[0])*(scalar);\
									   (dst[1])=(vec[1])*(scalar);\
									   (dst[2])=(vec[2])*(scalar); }

#define VecNormalize(vec) (lenght=sqrt((vec[0])*(vec[0])+\
(vec[1]) * (vec[1]) + (vec[2]) * (vec[2])));
if (lenght != 0)
{
	(vec[0]) \= lenght;
	(vec[1]) \= lenght;
	(vec[2]) \= lenght;
}
#else

#define VecSet(vec, x, y, z) { (vec[0]) = x; (vec[1]) = y; (vec[2]) = z; }
#define Vec4Set(vec, x, y, z, w) { (vec[0]) = x; (vec[1]) = y; (vec[2]) = z; (vec[3]) = w; }
#define VecClear(vec) { (vec[0]) = 0; (vec[1]) = 0; (vec[2]) = 0; }

#define VecPrint(vec) printf("<%f %f %f> ", (vec)[0], (vec)[1], (vec)[2]);

void	VecCopy (vec3f src, vec3f dst);
float	VecDot (const vec3f vec1, const vec3f vec2);
void	VecCross (vec3f v1, vec3f v2, vec3f cross);
void	VecAdd (vec3f op1, vec3f op2, vec3f res);
void	VecMA (vec3f v1, float fact, vec3f v2, vec3f res);
void	VecSub (vec3f op1, vec3f op2, vec3f res);
void	VecMulScalar (vec3f vec, float scalar, vec3f res);
float	VecNormalize (vec3f vec);
float 	VecNormalize2 (vec3f vec, vec3f res);
float	VecLength (vec3f vec);
void	VecAbs (vec3f v1, vec3f v2);

void	Mat3x3Identity (mat3x3 mat);
void	Mat3x3MulVec (mat3x3 mat, vec3f vec1, vec3f res);

void	Mat4x4Identity (mat4x4 mat);
void	Mat4x4MulVec (mat4x4 mat, vec4f vec1, vec4f res);
void	Mat4x4MulMat (mat4x4 m1, mat4x4 m2, mat4x4 res);

void	RotatePointAroundVector(vec3f dst, const vec3f dir, const vec3f point, float degrees);
void	AngleVectors(vec3f angles, vec3f forward, vec3f right, vec3f up);
void	ProjectPointOnPlane(vec3f dst, const vec3f p, const vec3f normal);
void	PerpendicularVector(vec3f dst, const vec3f src);

int		BoxOnPlaneSide(vec3f mins, vec3f maxs, plane_t *p);

#endif

#endif
