
#pragma once
#include <queue>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

#include "Point2D.h"
#include "Room.h"
#include "CompareNodes.h"
#include "Bullet.h"
#include "Node.h"

const int MAX_HEALTH = 100;
const int MAX_AMMO = 10;
const double AS = 0.8;
const double HS = 0.8;
const double HR = 0.4;
const double AR = 0;
const int POWERSHOT = 1;
const int DOUBLESHOT = 2;

const double HEALTH_BONUS_FACTOR = 0.5;
const double AMMO_BONUS_FACTOR = 2;


extern Node maze[MSZ][MSZ];
extern Room rooms[NUM_ROOMS];
extern bool proceed;

class Player
{
private:
	int team, bullets, variant;
	double health;
	Node* location;
	Room* currentRoom;
	Player* currentTarget;
	std::priority_queue<Node*, std::vector<Node*>, CompareNodes> player_priority_queue;
	std::deque<Room*> roomMemory;

public:
	Player(int team, int variant);

	int getTeam();
	double getHealth();
	int getBullets();
	Node* getLocation();
	Room* getCurrentRoom();
	Player* getCurrentTarget();
	int getVariant();

	void setHealth(int h);
	void setBullets(int a);
	void setLocation(Node* location);
	void setRoom(Room* room);

	void CheckNeighbours(Node * pn, std::vector<Node*>& gray, std::vector<Node*>& black, std::priority_queue<Node*, std::vector<Node*>, CompareNodes>& pq, Node * target);

	Room* findCurrentRoom();

	bool checkSide(Room* currentRoom, int playerX, int playerY, Node* target, int item);

	void checkSurroundings(Node* target);

	bool takeTurn(std::vector<Player*> vector_players);
	bool enemiesInRoom(std::vector<Player*> vector_players);
	Player* acquireTarget(std::vector<Player*> vector_players);
	void aquireHealth(std::vector<Player*> vector_players);
	void emergancy_decision(std::vector<Player*> vector_players);
	void aquireAmmunition(std::vector<Player*> vector_players);
	bool checkLineOfSight(Player* target, double angle);
	bool roomInMemory(Room* room);
	double distance(Node* node1, Node* node2);
	bool behavioralDecision(std::vector<Player*> vector_players);
	bool checkWithinRoom(Room* room);
	Node * moveToTarget(Node & start, Node & target);
	void AddNode(int row, int col, Node * pn, std::vector<Node*>& gray, std::vector<Node*>& black, std::priority_queue<Node*, std::vector<Node*>, CompareNodes>& pq, Node * target);
};

