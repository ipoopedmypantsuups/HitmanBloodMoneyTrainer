#ifndef OFFSETS_H_
#define OFFSETS_H_

#include <vector>


namespace offsets
{
	const unsigned int AMMO_ADDRESS			= 0x005140E7;
	const unsigned int ENTITY_COUNT			= 0x0059B864;
	const unsigned int HEALTH_ADDRESS		= 0x005FB973;
	const unsigned int ONE_SHOT_ADDRESS		= 0x00632EA6;
	const unsigned int XYZ_BASE_ADDR_CAM	= 0x000E6868;
	const unsigned int XYZ_BASE_ADDR_PLAYER	= 0x0008B610;

	std::vector<unsigned int> ENTITY_OFFSETS		= { 0x40 };
	std::vector<unsigned int> XYZ_OFFSETS_PLAYER	= { 0xC8, 0x74, 0x24 };
	std::vector<unsigned int> XYZ_OFFSETS_CAM		= { 0x78, 0x60, 0x48, 0x2C };
}

#endif
