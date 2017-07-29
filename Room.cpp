#include "stdafx.h"
#include "Room.h"

std::vector<Room *> Room::rooms = std::vector<Room *>();

Room::Room(SMath::Region region)
	: area(region)
{
	if ((area.getSize().x > 5 || area.getSize().y > 5) && rand() % 3 == 1) { // Boss room : Min 6 && 33%
		this->type = Room_Type::BOSS_ROOM;
	}
	else if (area.getSize().x > 2 || area.getSize().y > 2) { // Treasure room : Min 3
		this->type = Room_Type::TREASURE_ROOM;
	}
	else {
		this->type = Room_Type::DEFAULT_ROOM;
	}

	if (rand() % 10 == 0 && this->type != Room_Type::BOSS_ROOM) { // 10% chance of trap room if not boss room!
		this->type = Room_Type::TRAP_ROOM;
	}
	Room::rooms.push_back(this);
}

SMath::Vector2D Room::getSize() {
	return this->area.getSize();
}

void Room::connect_room(Room * room) {
	this->connected_rooms.push_back(room);
}

Room::~Room()
{
}
