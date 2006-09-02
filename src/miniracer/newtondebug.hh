#ifndef MINIRACER_NEWTONDEBUG_HH
#define MINIRACER_NEWTONDEBUG_HH

#include "Newton.h"
#include "../scene/camera.hh"
#include "../video/vcache.hh"

void newtonShowCollisionGeometry(NewtonWorld *pWorld,const ion::scene::Camera &rCamera,ion::video::VCache &rVCache);

#endif
