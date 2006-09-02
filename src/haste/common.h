#ifndef _COMMONZ_H_
#define _COMMONZ_H_

#include <windows.h>
#include <stdarg.h>
#include <string.h>
#include <conio.h>

#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>

#include "../base/managedmemobj.hh"
#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../base/dynobject.hh"
#include "../base/fpscalc.hh"
#include "../base/physfsfile.hh"

#include "../math/matrix.hh"
#include "../math/vectors.hh"

#include "../input/inputdevice.hh"
#include "../input/inputserver.hh"
#include "../input/keyboardsym.hh"

#include "../scene/camera.hh"
#include "../scene/ffprenderer.hh"
#include "../scene/light.hh"
#include "../scene/q3bsp.hh"
#include "../scene/q3shaders.hh"
#include "../scene/group.hh"
#include "../scene/renderablemesh.hh"
#include "../scene/renderablemeshnode.hh"
#include "../scene/renderquery.hh"
#include "../scene/simplespace.hh"
#include "../scene/spritecache.hh"

#include "../video/vertexiterator.hh"
#include "../video/vertexstream.hh"
#include "../video/videodevice.hh"

#include "../audio/audioserver.hh"
#include "../audio/sound.hh"
#include "../audio/voice.hh"

#include "skelapp.hh"

using namespace std;
using namespace ion;
using namespace ion::base;
using namespace ion::math;
using namespace ion::input;
using namespace ion::scene;
using namespace ion::video;
using namespace ion::audio;

#define ENTITY_STATIC_BIND

#include "../extern/newton/Newton.h"

#include "eexport.h"
#include "trace.h"
#include "EntLib/entity.h"

#include "console.h"
#include "input.h"
#include "cvars.hh"
#include "g_entity.h"
#include "r_hud.h"
#include "g_ui.h"
#include "g_game.h"
#include "g_physics.h"
#include "g_audio.h"

#include "globals.h"

#define DEG2RAD(x)	(3.1415926535f * (x) / 180.0f)
#define RAD2DEG(x)	(3.1415926535f / (x) * 180.0f)

void VecSwizzle(Vector3f& v);
void AngleVectors(Vector3f &angles, Vector3f &front, Vector3f &right, Vector3f &up);
void AngleVectorsQ(Vector3f &angles, Quaternion &q);
void AngleVectorsQ2(Vector3f &angles, Quaternion &q);

#endif