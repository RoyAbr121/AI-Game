
#pragma once
#include "Point2D.h"
#include "Node.h"
#include <vector>

class Room
{
private:
	Point2D leftTop, rightBottom, center;
	int width, height;
	int healthPack, ammoPack;
	std::vector<Node*> room_ammunition;
	std::vector<Node*> room_health;

public:
	Room();
	~Room();
	Room(int ci, int cj, int w, int h);
	int getWidth();
	int getHeight();
	Point2D getLeftTop();
	Point2D getRightBottom();
	bool CheckOverlapping(Room * pother);
	void toString();
	Point2D getCenter();
	int getHealth();
	void setHealth(int healthPacks);
	int getAmmo();
	void setAmmo(int ammoPacks);

	bool operator == (const Room& room)
	{
		return this->center == room.center;
	}

	std::vector<Node*>& getAmmunitionVector();
	std::vector<Node*>& getHealthVector();
};

