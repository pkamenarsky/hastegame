#include "ent_info_player_deathmatch.h"

EntInfoPlayerDeathmatch::EntInfoPlayerDeathmatch(EngineExport *EE) : Entity(EE)
{
	ClassName = "info_player_deathmatch";

	SetAABB(Vector3f(-50, 0, -50), Vector3f(50, 50, 50));

//	Link();
}

EntInfoPlayerDeathmatch::EntInfoPlayerDeathmatch()
{
	Light(NULL);
}

EntInfoPlayerDeathmatch::~EntInfoPlayerDeathmatch()
{
}

void EntInfoPlayerDeathmatch::Init(void)
{
}

EXPORT_ENTITY(EntInfoPlayerDeathmatch)
