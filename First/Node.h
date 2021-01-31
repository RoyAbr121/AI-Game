
#pragma once
#include "Point2D.h"

const int MSZ = 100;
const int NUM_ROOMS = 10;
const int ROOM_SIZE = 10;
const int SAFETY_COEFFICIENT = 5;
const int SPACE = 0;
const int WALL = 1;
const int HEALTH = 2;
const int AMMO = 3;
const int TEAM1 = 4;
const int TEAM2 = 5; 
const int NONE = 100;
const double PI = 3.14159265358979323846;
const int W = 700; // window width
const int H = 700; // window height

class Node
{
public:
	Node();
	Node(Point2D& pt, Point2D* t, int v, double g, Node* pr);
	~Node();

private:
	int value;
	int item;
	double h, g, safety;
	Node* parent;
	Point2D* target;
	Point2D point;

public:
	void SetValue(int value);
	int GetValue();
	void setItem(int item);
	int getItem();
	void setG(double g);
	double getG();
	void setH(double h);
	double ComputeH();
	double ComputeH(Point2D* target);
	double getF();
	void setSafety(double safety);
	double getSafety();
	Point2D& getPoint();
	Node* getParent();
	Point2D* getTarget();

	void setParent(Node* parent);
	
	bool operator==(const Node &other)  
	{
		return this->point == other.point;
	}
};