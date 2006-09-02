/********************************************************************
VEC.C
********************************************************************/

#include "vec.h"

void VecCopy (vec3f src, vec3f dst)
{
	memmove (dst, src, sizeof (vec3f));
}

#define VecSet(vec, x, y, z) { (vec[0]) = x; (vec[1]) = y; (vec[2]) = z; }

float VecDot (const vec3f vec1, const vec3f vec2)
{
	return (vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]);
}

void VecCross (vec3f v1, vec3f v2, vec3f cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void VecAdd (vec3f op1, vec3f op2, vec3f res)
{
	res[0] = op1[0] + op2[0];
	res[1] = op1[1] + op2[1];
	res[2] = op1[2] + op2[2];
}

void VecMA (vec3f v1, float fact, vec3f v2, vec3f res)
{
	vec3f v;

	VecMulScalar (v2, fact, v);
	VecAdd (v, v1, res);
}

void VecSub (vec3f op1, vec3f op2, vec3f res)
{
	res[0] = op1[0] - op2[0];
	res[1] = op1[1] - op2[1];
	res[2] = op1[2] - op2[2];
}

void VecMulScalar (vec3f vec, float scalar, vec3f res)
{
	res[0] = vec[0] * scalar;
	res[1] = vec[1] * scalar;
	res[2] = vec[2] * scalar;
}

float VecNormalize (vec3f vec)
{
	float lenght = (float) sqrt (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

	if (lenght != 0)
	{
		vec[0] /= lenght;
		vec[1] /= lenght;
		vec[2] /= lenght;
	}

	return lenght;
}

float VecNormalize2 (vec3f vec, vec3f res)
{
	float lenght = (float) sqrt (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

	if (lenght != 0)
	{
		res[0] = vec[0] / lenght;
		res[1] = vec[1] / lenght;
		res[2] = vec[2] / lenght;
	}

	return lenght;
}

float VecLength(vec3f vec)
{
	return (float) sqrt(VecDot(vec, vec));
}

void VecAbs (vec3f v1, vec3f v2)
{
	v2[0] = (float)fabs (v1[0]);
	v2[1] = (float)fabs (v1[1]);
	v2[2] = (float)fabs (v1[2]);
}

static mat3x3 mat3x3identity = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

void Mat3x3Identity (mat3x3 mat)
{
	memcpy (mat, mat3x3identity, sizeof (mat3x3));
}

void Mat3x3MulVec (mat3x3 mat, vec3f vec, vec3f res)
{
	res[0] = vec[0] * mat[0][0] + vec[1] * mat[1][0] + vec[2] * mat[2][0];
	res[1] = vec[0] * mat[0][1] + vec[1] * mat[1][1] + vec[2] * mat[2][1];
	res[2] = vec[0] * mat[0][2] + vec[1] * mat[1][2] + vec[2] * mat[2][2];
}

static mat4x4	mat4x4identity = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

void Mat4x4Identity(mat4x4 mat)
{
	memcpy(mat, mat4x4identity, sizeof (mat4x4));
}

void Mat4x4MulVec(mat4x4 mat, vec4f vec, vec4f res)
{
	res[0] = vec[0] * mat[0][0] + vec[1] * mat[1][0] + vec[2] * mat[2][0] + mat[3][0];
	res[1] = vec[0] * mat[0][1] + vec[1] * mat[1][1] + vec[2] * mat[2][1] + mat[3][1];
	res[2] = vec[0] * mat[0][2] + vec[1] * mat[1][2] + vec[2] * mat[2][2] + mat[3][2];
}

#if 0

void RotatePointAroundVector(vec3f dst, const vec3f dir, const vec3f point, float degrees)
{
	float	m[3][3];
	float	im[3][3];
	float	zrot[3][3];
	float	tmpmat[3][3];
	float	rot[3][3];
	int		i;
	vec3f	vr, vup, vf;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector(vr, dir);
	VecCross(vr, vf, vup);

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy(im, m, sizeof(im));

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset(zrot, 0, sizeof(zrot));
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	zrot[0][0] = cos(DEG2RAD(degrees));
	zrot[0][1] = sin(DEG2RAD(degrees));
	zrot[1][0] = -sin(DEG2RAD(degrees));
	zrot[1][1] = cos(DEG2RAD(degrees));

	R_ConcatRotations(m, zrot, tmpmat);
	R_ConcatRotations(tmpmat, im, rot);

	for (i = 0; i < 3; i++)
	{
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}

#endif

void AngleVectors(vec3f angles, vec3f forward, vec3f right, vec3f up)
{
	float			angle;
	static float	sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = (float) (angles[0] * (M_PI*2 / 360));	//yaw
	sy = (float) sin(angle);
	cy = (float) cos(angle);
	angle = (float) (angles[1] * (M_PI*2 / 360)); //pitch
	sp = (float) sin(angle);
	cp = (float) cos(angle);
	angle = (float) (angles[2] * (M_PI*2 / 360));	//roll
	sr = (float) sin(angle);
	cr = (float) cos(angle);

	if(forward)
	{	
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if(right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if(up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

void ProjectPointOnPlane(vec3f dst, const vec3f p, const vec3f normal)
{
	float		d;
	vec3f		n;
	float		inv_denom;

	inv_denom = 1.0F / VecDot(normal, normal);

	d = VecDot(normal, p) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

/*
** assumes "src" is normalized
*/
void PerpendicularVector(vec3f dst, const vec3f src)
{
	int		pos;
	int		i;
	float	minelem = 1.0F;
	vec3f	tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for (pos = 0, i = 0; i < 3; i++)
	{
		if (fabs(src[i]) < minelem)
		{
			pos = i;
			minelem = (float) fabs(src[i]);
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	/*
	** project the point onto the plane defined by src
	*/
	ProjectPointOnPlane(dst, tempvec, src);

	/*
	** normalize the result
	*/
	VecNormalize(dst);
}

int BoxOnPlaneSide(vec3f mins, vec3f maxs, plane_t *p)
{
	int		i;
	float	dist1, dist2;
	int		sides = 0;
	vec3f	corners[2];
	
	for(i = 0; i < 3; i++)
	{
		if(p->normal[i] < 0)
		{
			corners[0][i] = mins[i];
			corners[1][i] = maxs[i];
		}
		else
		{
			corners[1][i] = mins[i];
			corners[0][i] = maxs[i];
		}
	}
	
	dist1 = VecDot(p->normal, corners[0]) - p->dist;
	dist2 = VecDot(p->normal, corners[1]) - p->dist;
	
	if(dist1 >= 0) sides = 1;
	if(dist2 < 0) sides |= 2;
	
	return sides;
}
	
