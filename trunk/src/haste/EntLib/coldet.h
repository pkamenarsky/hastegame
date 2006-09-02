/*
==============================================================================
COLDET.H
==============================================================================
*/

#ifndef _COLDET_H_
#define _COLDET_H_

#include "typedefs.h"
#include "vec.h"

#define MAX_CD_AREAS	64

typedef struct area_ref_c
{
	struct area_ref_c	*prev, *next;
}area_ref_t;

typedef struct
{
	vec3f	start;
	vec3f	end;
	
	float	frac;
	
	byte	allsolid;
	byte	startsolid;
	
	plane_t	plane;
	int		areanum;
	
	int		contents;
	
	//for some opimizations
	vec3f	old_end;
	int		old_end_area;
	
	vec3f	old_start;
	int		old_start_area;

	int		old_end_areas[MAX_CD_AREAS];
	int		old_start_areas[MAX_CD_AREAS];
	
	void	*current_areas[MAX_CD_AREAS];
	int		num_current_areas;
	
	area_ref_t	area_refs[MAX_CD_AREAS];
	area_ref_t	*area_ref_list;
}trace_t;

#endif
