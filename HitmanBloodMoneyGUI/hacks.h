#ifndef HACKS_H_
#define HACKS_H_

#include <vector>
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>


class Hacks
{
public:
	/**
	 * Enables infinite health, infinite ammo, and one shot kill
	 *
	 * @param bCheatsEnabled
	 * @return void
	 */
	static void GodMode(bool bCheatsEnabled);

	/**
	 * Increases or decreases current entity by providing 1 or -1, respectively, 
	 * as the second function argument.
	 *
	 * @param &current_entity, -1 or 1
	 * @return void
	 */
	static void SetCurrentEntity(int* current_entity, int operation);

	/**
	 * Teleports the player to the overhead camera's target position.
	 *
	 * @param initial offset, offsets, &x_cam, &y_cam, &z_cam
	 * @return void
	 */
	static void TeleportToCam(uintptr_t addr, std::vector<unsigned int> offsets, float x_cam, float y_cam, float z_cam);

	/**
	 * Attemps to teleport the player to entity location if the entity's 
	 * location is not null.
	 *
	 * @param initial offset, offsets, current_entity
	 * @return void
	 */
	static void TeleportToEntity(uintptr_t addr, std::vector<unsigned int> offsets, int current_entity);

	/**
	 * Stores the player's overhead camera target into last three float
	 * parameters.
	 *
	 * @param initial offset, offsets, &x_cam, &y_cam, &z_cam
	 * @return void
	 */
	static void UpdatePlayerCamCoords(uintptr_t addr, std::vector<unsigned int> offsets, float* x_cam, float* y_cam, float* z_cam);
};

#endif