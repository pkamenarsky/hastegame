/*****************************************************************************
POLYLIB.C
*****************************************************************************/

#include "common.h"

int SplitWindingByPlane (winding_t * winding, plane_t * plane, winding_t * front, winding_t * back, float epsilon)
{
	int i;
	winding_t f, b;
	int points_on = 0;

	if (winding->numpoints == 0)
	{
		if(front) front->numpoints = 0;
		if(back) back->numpoints = 0;
		return POLY_NONE;
	}

	CopyPlane (&winding->plane, &f.plane);
	CopyPlane (&winding->plane, &b.plane);

	f.numpoints = 0;
	b.numpoints = 0;

	for (i = 0; i < winding->numpoints; i++)
	{
		vec3f *p1 = winding->points + i;
		vec3f *p2 = winding->points + ((i + 1) % winding->numpoints);

		float dot1, dot2;

		dot1 = VecDot (*p1, plane->normal) - plane->dist;
		dot2 = VecDot (*p2, plane->normal) - plane->dist;

		if (dot1 < -epsilon)
			VecCopy (*p1, b.points[b.numpoints++]);
		else if (dot1 > epsilon)
			VecCopy (*p1, f.points[f.numpoints++]);
		else
		{
			points_on++;
			VecCopy (*p1, b.points[b.numpoints++]);
			VecCopy (*p1, f.points[f.numpoints++]);
		}

		if ((dot1 < -epsilon && dot2 > epsilon) || (dot2 < -epsilon && dot1 > epsilon))
		{
			vec3f mid;

			VecSub (*p2, *p1, mid);
			VecMulScalar (mid, dot1 / (dot1 - dot2), mid);
			VecAdd (*p1, mid, mid);

			VecCopy (mid, b.points[b.numpoints++]);
			VecCopy (mid, f.points[f.numpoints++]);
		}
	}

	if (f.numpoints < 3)
		f.numpoints = 0;
	if (b.numpoints < 3)
		b.numpoints = 0;

	if (front)
		CopyWinding (&f, front);
	if (back)
		CopyWinding (&b, back);

	if (points_on == winding->numpoints)
		return POLY_ON;
	if (!f.numpoints)
		return POLY_BACK;
	if (!b.numpoints)
		return POLY_FRONT;

	return POLY_CROSS;
}

void CopyWinding (winding_t * src, winding_t * dst)
{
	memcpy (dst, src, sizeof (winding_t));
}

void CopyPlane (plane_t * src, plane_t * dst)
{
	memcpy (dst, src, sizeof (plane_t));
}

#define _PLANE_TOL_ 0.05

void MakeWindingFromPlane (plane_t * p, winding_t * w)
{
	int i, x;
	float max, v;
	vec3f org, vright, vup;

// find the major axis

	max = -BOGUS_RANGE;
	x = -1;
	for (i = 0; i < 3; i++)
	{
		v = fabs (p->normal[i]);
		if (v > max)
		{
			x = i;
			max = v;
		}
	}
	if (x == -1)
		Error ("MakeWindingFromPlane: no axis found");

	VecCopy (vec3f_origin, vup);
	switch (x)
	{
	case 0:
	case 1:
		vup[2] = 1;
		break;
	case 2:
		vup[0] = 1;
		break;
	}

	v = VecDot (vup, p->normal);
	VecMA (vup, -v, p->normal, vup);
	VecNormalize (vup);

	VecMulScalar (p->normal, p->dist, org);

	VecCross (vup, p->normal, vright);

	VecMulScalar (vup, 8192, vup);
	VecMulScalar (vright, 8192, vright);

// project a really big axis aligned box onto the plane
//      w=NewWinding(4);

	VecSub (org, vright, w->points[0]);
	VecAdd (w->points[0], vup, w->points[0]);

	VecAdd (org, vright, w->points[1]);
	VecAdd (w->points[1], vup, w->points[1]);

	VecAdd (org, vright, w->points[2]);
	VecSub (w->points[2], vup, w->points[2]);

	VecSub (org, vright, w->points[3]);
	VecSub (w->points[3], vup, w->points[3]);

	w->numpoints = 4;
}

void MakePlaneFrom3Points (vec3f p1, vec3f p2, vec3f p3, plane_t * plane)
{
	vec3f v1, v2, v3;

	VecSub (p1, p2, v1);
	VecSub (p3, p2, v2);

	VecCross (v1, v2, v3);

	VecNormalize2 (v3, plane->normal);
	plane->dist = VecDot (p1, plane->normal);
}

//orders the points in a clock-wise manner
void CorrectWinding(int type, winding_t *w)
{
	vec3f		*p1 = w->points + 0,
				*p2 = w->points + 1,
				*p3 = w->points + 2,
				t;
	
	winding_t	temp;
	plane_t		plane;
	
	int			i, j;

	VecAdd(p1, w->plane.normal, t);

	MakePlaneFrom3Points(*p1, *p2, t, &plane);
	CopyWinding(w, &temp);
		
	if(((VecDot(*p3, plane.normal) - plane.dist) < 0 && type != BT_SOLID) || ((VecDot(*p3, plane.normal) - plane.dist) >= 0 && type == BT_SOLID)) 
	{
		for(i = 0, j = w->numpoints - 1; i < w->numpoints; i++, j--)
			VecCopy(w->points[j], temp.points[i]);
		
		CopyWinding(&temp, w);
	}
}

#define POINT_TOL 0.01

int SamePoint (vec3f p1, vec3f p2)
{
	//is that a good way to do it?
	if (fabs (p1[0] - p2[0]) > POINT_TOL || fabs (p1[1] - p2[1]) > POINT_TOL || fabs (p1[2] - p2[2]) > POINT_TOL)
		return FALSE;

	return TRUE;
}

int PlanesAreEqual (plane_t * p1, plane_t * p2)
{
	if (SamePoint (p1->normal, p2->normal) && (fabs (p1->dist - p2->dist) <= POINT_TOL))
		return TRUE;

	return FALSE;
}

void PlaneInvert (plane_t * plane)
{
	plane->normal[0] = -plane->normal[0];
	plane->normal[1] = -plane->normal[1];
	plane->normal[2] = -plane->normal[2];

	plane->dist = -plane->dist;
}

int SamePlane (plane_t * p1, plane_t * p2)
{
	plane_t plane;

	CopyPlane (p1, &plane);
	PlaneInvert (&plane);

	if (PlanesAreEqual (p1, p2) || PlanesAreEqual (&plane, p2))
		return TRUE;

	return FALSE;
}


int CompareWindings (winding_t * w1, winding_t * w2)
{
	int i, j, found = 0;

	if (w1->numpoints != w2->numpoints)
		return FALSE;

	for (i = 0; i < w1->numpoints; i++)
		for (j = 0; j < w2->numpoints; j++)
			if (SamePoint (w1->points[i], w2->points[j]))
				found++;

	if (found == w1->numpoints)
		return TRUE;

	return FALSE;
}

void DumbBrush (brush_t * brush)
{
	brushside_t *bs;
	int numside = 0;
	char bs_types[][32] = { "FT_BAD", "FT_FLAT", "FT_MESH", "FT_TRISURF", "FT_SPRITE",
		"FT_AREAPORTAL"
	};

	printf ("bounding box: [%5.3f %5.3f %5.3f] [%5.3f %5.3f %5.3f]\n", brush->mins[0], brush->mins[1], brush->mins[2], brush->maxs[0], brush->maxs[1], brush->maxs[2]);

	for (bs = brush->sides; bs; bs = bs->next, numside++)
	{
		int i;

		printf ("\n** Brushside %d\n", numside);
		printf ("type= %s\n", bs_types[bs->type]);
		printf ("plane eq: [%5.3f %5.3f %5.3f %5.3f]\n\n", bs->winding.plane.normal[0], bs->winding.plane.normal[1], bs->winding.plane.normal[2], bs->winding.plane.dist);

		printf ("shadernum: %d\n", bs->numshader);
		printf ("tex_plane[0]: [%5.3f %5.3f %5.3f %5.3f]\n\n", bs->tex_planes[0].normal[0], bs->tex_planes[0].normal[1], bs->tex_planes[0].normal[2], bs->tex_planes[0].dist);
		printf ("tex_plane[1]: [%5.3f %5.3f %5.3f %5.3f]\n\n", bs->tex_planes[1].normal[0], bs->tex_planes[1].normal[1], bs->tex_planes[1].normal[2], bs->tex_planes[1].dist);

		for (i = 0; i < bs->winding.numpoints; i++)
			printf ("point %2d: [%8.3f %8.3f %8.3f]\n", i, bs->winding.points[i][0], bs->winding.points[i][1], bs->winding.points[i][2]);
	}
}

/*
void MakeCube (brush_t * brush, vec3f origin, int side)
{
	brushside_t *sides[6];
	int i, j;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 3; j++)
		{
			int numside = i * 3 + j;

			sides[numside] = AllocAndAddWindingToBrush (brush);

			sides[numside]->winding.plane.normal[j] = (i ? 1.0 : -1.0);
			sides[numside]->winding.plane.dist = -(side / 2) + (i ? origin[j] : -origin[j]);

			MakeWindingFromPlane (&sides[numside]->winding.plane, &sides[numside]->winding);
		}

	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++)
		{
			if (i == j)
				continue;

			SplitWindingByPlane (&sides[i]->winding, &sides[j]->winding.plane, &sides[i]->winding, NULL, PLANE_EPSILON);
		}

	GetBrushBounds (brush);
}
*/
