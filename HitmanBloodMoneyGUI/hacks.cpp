#include <cmath>
#include <string>

#include "includes.hpp"
#include "entity.hpp"
#include "hacks.hpp"
#include "mem.hpp"

#define COORD_MAX 8000
#define COORD_MIN -8000

bool ValidCoordinate(float coordinate)
{
	if (coordinate == 0)
		return false;

	if (std::isnan(coordinate))
		return false;

	if (coordinate > COORD_MAX)
		return false;

	if (coordinate < COORD_MIN)
		return false;

	return true;
}

void Hacks::UpdatePlayerCamCoords(uintptr_t addr, std::vector<unsigned int> offsets, float* x_cam, float* y_cam, float* z_cam)
{ 
	uintptr_t module_base_address = (uintptr_t)GetModuleHandle(NULL);

	PlayerCoords* coords          = (PlayerCoords*)FindDMAddress(module_base_address + addr, offsets);
	if (coords != nullptr && ValidCoordinate(coords->x) && ValidCoordinate(coords->y) && ValidCoordinate(coords->z))
	{
		*x_cam = coords->x;
		*y_cam = coords->y;
		*z_cam = coords->z;
	}

}

void Hacks::TeleportToCam(uintptr_t addr, std::vector<unsigned int> offsets, float x_cam, float y_cam, float z_cam)
{
	uintptr_t module_base_addr = (uintptr_t)GetModuleHandle(NULL);

	PlayerCoords* xyz          = (PlayerCoords*)FindDMAddress(module_base_addr + addr, offsets);
	//if (xyz != nullptr && ValidCoordinate(xyz->x) && ValidCoordinate(xyz->y) && ValidCoordinate(xyz->z))
	//{
		xyz->x = x_cam;
		xyz->y = y_cam;
		xyz->z = z_cam;
	//}

}

void Hacks::TeleportToEntity(uintptr_t addr, std::vector<unsigned int> offsets, int current_entity)
{
	uintptr_t module_base_addr = (uintptr_t)GetModuleHandle(NULL);
	EntityList* entity_list    = *(EntityList**)(module_base_addr + 0x41F83C);
	Entity* entity             = entity_list->ents[current_entity].entity;

	PlayerCoords* xyz          = (PlayerCoords*)FindDMAddress(module_base_addr + addr, offsets);
	if (!isnan(entity->x) && !isnan(entity->y) && !isnan(entity->z) && entity != nullptr && (&entity->x != nullptr && &entity->y != nullptr && &entity->z != nullptr))
	{
		xyz->x = entity->x + 10;
		xyz->y = entity->y + 10;
		xyz->z = entity->z + 10;
	}

}

void Hacks::GodMode(bool bCheatsEnabled)
{
	char* AMMO_OP_CODES         = (char*)"\x49";
	char* HEALTH_OP_CODES       = (char*)"\xD8\x64\x24\x08";
	char* ONE_SHOT_OP_CODES	    = (char*)"\xD8\x64\x24\x04\xD9\x91\x28\x09\x00\x00";

	char* AMMO_OP_CODES_NEW     = (char*)"\x90";
	char* HEALTH_OP_CODES_NEW   = (char*)"\x90\x90\x90\x90";
	char* ONE_SHOT_OP_CODES_NEW = (char*)"\xD8\xA1\x28\x09\x00\x00\x90\x90\x90\x90";

	char* AMMO_ADDRESS			= (char*)0x005140E7;
	char* HEALTH_ADDRESS		= (char*)0x005FB973;
	char* ONE_SHOT_ADDRESS		= (char*)0x00632EA6;

	size_t AMMO_OP_CODE_LENGTH		= 1;
	size_t HEALTH_OP_CODE_LENGTH	= 4;
	size_t ONE_SHOT_OP_CODE_LENGTH	= 10;

	if (bCheatsEnabled)
	{
		Patch(AMMO_ADDRESS, AMMO_OP_CODES_NEW, AMMO_OP_CODE_LENGTH);
		Patch(HEALTH_ADDRESS, HEALTH_OP_CODES_NEW, HEALTH_OP_CODE_LENGTH);
		Patch(ONE_SHOT_ADDRESS, ONE_SHOT_OP_CODES_NEW, ONE_SHOT_OP_CODE_LENGTH);
	}
	else
	{
		Patch(AMMO_ADDRESS, AMMO_OP_CODES, AMMO_OP_CODE_LENGTH);
		Patch(HEALTH_ADDRESS, HEALTH_OP_CODES, HEALTH_OP_CODE_LENGTH);
		Patch(ONE_SHOT_ADDRESS, ONE_SHOT_OP_CODES, ONE_SHOT_OP_CODE_LENGTH);
	}
}

void Hacks::SetCurrentEntity(int* current_entity, int operation)
{
	uintptr_t module_base_addr = (uintptr_t)GetModuleHandle(NULL);
	EntityList* entity_list    = *(EntityList**)(module_base_addr + 0x41F83C);

	switch (operation)
	{
	case 1:
		*current_entity += 1;
		if (*current_entity > entity_list->n_entities - 1)
		{
			*current_entity = 0;
		}
		break;
	case -1:
		*current_entity -= 1;
		if (*current_entity < 0)
		{
			*current_entity = entity_list->n_entities - 1;
		}
		break;
	default:
		break;
	}

}
