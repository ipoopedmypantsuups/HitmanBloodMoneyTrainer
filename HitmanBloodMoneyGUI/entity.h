#ifndef ENTITY_H_
#define ENTITY_H_


struct Entity
{
	int _0x3DC[247];
	float x, y, z;
	int _0x928[336];
	float hp;
};

class EntityObject
{
public:
	struct Entity* entity;
};

class EntityList
{
public:
	int _0x8[2];
	int n_entities;
	EntityObject ents[71];
	int _0x5D00[5952];
	bool ingame;
};

struct PlayerCoords
{
	float x, y, z;
};

#endif
