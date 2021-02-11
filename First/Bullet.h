
#pragma once
#include "Node.h"
#include "Player.h"

const double delta = 0.0001;

class Bullet
{
public:
	Bullet(double x, double y);
	Bullet(double x, double y, double angle, bool power);
	~Bullet();
	void showMe();
	void SetIsMoving(bool move);
	bool GetIsMoving();
	void simMove(Node maze[MSZ][MSZ], int team, int otherTeam);
	void move(Node maze[MSZ][MSZ], double speed, int team);
	double getX();
	double getY();
	void SetDir(double angle);
	void SimulateMotion(double map[MSZ][MSZ], Node maze[MSZ][MSZ]);

private:
	double x, y;
	double dirx, diry;
	bool isMoving;
	bool power;
};

