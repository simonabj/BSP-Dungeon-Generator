#pragma once

#include "SMath.h"
#include <vector>

static enum Room_Type {
	DEFAULT_ROOM,
	TREASURE_ROOM,
	BOSS_ROOM,
	TRAP_ROOM
};

class Room
{
	
public:
	static std::vector<Room *> rooms;

	SMath::Region area;
	SMath::Vector2D getSize();
	void connect_room(Room *);

	Room_Type type = Room_Type::DEFAULT_ROOM;

	std::vector<Room *> connected_rooms = std::vector<Room *>(1);
	

	Room(SMath::Region region);
	~Room();
};

