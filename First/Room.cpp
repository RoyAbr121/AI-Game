
#include "Room.h"
#include <math.h>
#include <iostream>

using namespace std;

Room::Room()
{
}

Room::~Room()
{
}

Room::Room(int ci, int cj, int w, int h) 
{
	center.setCol(cj);
	center.setRow(ci);
	leftTop.setCol(cj - w / 2);
	leftTop.setRow(ci - h / 2);
	rightBottom.setCol(cj + w / 2);
	rightBottom.setRow(ci + h / 2);
	width = w;
	height = h;
}

int Room::getWidth()
{
	return width;
}

int Room::getHeight()
{
	return height;
}

Point2D Room::getLeftTop()
{
	return leftTop;
}
Point2D Room::getRightBottom() {
	return rightBottom;
}

bool Room::CheckOverlapping(Room* pother)
{
	int horiz_dist, vert_dist, vsz,hsz;
	horiz_dist = abs(center.getCol() - pother->center.getCol());
	vert_dist = abs(center.getRow() - pother->center.getRow());
	vsz = height / 2 + pother->height / 2;
	hsz = width / 2 + pother->width / 2;

	return horiz_dist <= hsz+2 && vert_dist <= vsz+2;
}

void Room::toString()
{
	cout << "check new Room " << "center: (" << center.getRow() << "," << center.getCol()
		<< "), width: " << width << ", height" << height << endl;
}

Point2D Room::getCenter()
{
	return center;
}

int Room::getHealth()
{
	if (this)
		return healthPack;
	else
		return 0;
}

void Room::setHealth(int healthPacks)
{
	this->healthPack = healthPacks;
}

int Room::getAmmo()
{
	if (this)
	{
		return ammoPack;
	}
	else
	{
		return 0;
	}
}

void Room::setAmmo(int ammoPacks)
{
	this->ammoPack = ammoPacks;
}

std::vector<Node*>& Room::getAmmunitionVector()
{
	return this->room_ammunition;
}

std::vector<Node*>& Room::getHealthVector()
{
	return this->room_health;
}
