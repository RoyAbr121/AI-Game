
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <queue>
#include "Bullet.h"
#include "GLUT.h"
#include "Granade.h"
#include "Player.h"
#include "Node.h"
#include "CompareNodes.h"
#include "Room.h"

Node maze[MSZ][MSZ];
double map[MSZ][MSZ] = { 0 };
Room rooms[NUM_ROOMS];
int numExistingRooms = 0;
int t1Deaths = 0, t2Deaths = 0;

std::vector<Player*> vector_players;

Bullet* pb = nullptr;
Granade* pg;
bool proceed = false;
int counter = 0;

void SetupMaze();
void GenerateMap();

void init()
{
	srand(time(0)); // pseudo randomizer
	glClearColor(0.7, 0.7, 0.7, 0);
	SetupMaze();
	GenerateMap();
	glOrtho(-1, 1, -1, 1, -1, 1);
}

Room GenerateRoom()
{
	int w, h, ci, cj;
	Room* pr = nullptr;
	bool isOveralaping;

	do
	{
		isOveralaping = false;
		w = ROOM_SIZE + rand() % 10;
		h = ROOM_SIZE + rand() % 10;

		ci = h / 2 + rand() % (MSZ - h);
		cj = w / 2 + rand() % (MSZ - w);

		pr = new Room(ci, cj, w, h);

		for (int i = 0; i < numExistingRooms && !isOveralaping; i++)
		{
			if (rooms[i].CheckOverlapping(pr))
			{
				isOveralaping = true;
			}
		}
	} while (isOveralaping);

	// if the room isn't overlapping, dig it
	for (int i = pr->getLeftTop().getRow(); i <= pr->getRightBottom().getRow(); i++)
	{
		for (int j = pr->getLeftTop().getCol(); j <= pr->getRightBottom().getCol(); j++)
		{
			maze[i][j].SetValue(SPACE);
		}
	}
	return *pr;
}

// check if the node at row, col is white or gray that is better then the previous one and if so add it to pq
void AddNode(int row, int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black, std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq)
{
	Point2D pt;
	Node* pn1;
	std::vector<Node>::iterator gray_it;
	std::vector<Node>::iterator black_it;
	double cost;

	pt.setRow(row);
	pt.setCol(col);

	if (maze[row][col].GetValue() == SPACE)
	{
		cost = 0.1; // space cost
	}
	else if (maze[row][col].GetValue() == WALL)
	{
		cost = 3; //Wall cost
	}
	else
	{
		cost = INT_MAX; 
	}
	pn1 = new Node(pt, pn->getTarget(), maze[pt.getRow()][pt.getCol()].GetValue(), pn->getG() + cost, pn);
	black_it = find(black.begin(), black.end(), *pn1);
	gray_it = find(gray.begin(), gray.end(), *pn1);
	if (black_it == black.end() && gray_it == gray.end()) // it is not black and not gray!
	{// i.e. it is white
		pq.push(pn1);
		gray.push_back(*pn1);
	}
}

void AddNeighbours(Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq)
{
	if (pn->getPoint().getRow() < MSZ - 1)
	{
		AddNode(pn->getPoint().getRow() + 1, pn->getPoint().getCol(), pn, gray, black, pq);
	}
	if (pn->getPoint().getRow() > 0)
	{
		AddNode(pn->getPoint().getRow() - 1, pn->getPoint().getCol(), pn, gray, black, pq);
	}
	if (pn->getPoint().getCol() > 0)
	{
		AddNode(pn->getPoint().getRow(), pn->getPoint().getCol() - 1, pn, gray, black, pq);
	}
	if (pn->getPoint().getCol() < MSZ - 1)
	{
		AddNode(pn->getPoint().getRow(), pn->getPoint().getCol() + 1, pn, gray, black, pq);
	}
}

void GeneratePath(Point2D start, Point2D target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	std::vector<Node> gray;
	std::vector<Node> black;
	std::vector<Node>::iterator gray_it;
	std::vector<Node>::iterator black_it;

	Node* pn;
	bool stop = false;

	pn = new Node(start, &target, maze[start.getRow()][start.getCol()].GetValue(), 0, nullptr);
	pq.push(pn);
	gray.push_back(*pn);

	while (!pq.empty() && !stop)
	{
		pn = pq.top();	// take the best node from pq
		pq.pop();	// remove top Node from pq
		if (pn->getPoint() == target) // the path has been found
		{
			stop = true;
			// restore path to dig tunnels
			// set SPACE instead of WALL on the path
			while (!(pn->getPoint() == start))
			{
				maze[pn->getPoint().getRow()][pn->getPoint().getCol()].SetValue(SPACE);
				pn = pn->getParent();
			}
			return;
		}
		else // pn is not target
		{
			// remove Node from gray and add it to black
			gray_it = find(gray.begin(), gray.end(), *pn); // operator == must be implemented in Node
			if (gray_it != gray.end())
				gray.erase(gray_it);
			black.push_back(*pn);
			// check the neighbours
			AddNeighbours(pn, gray, black, pq);
		}
	}
}

void DigTunnels()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		std::cout << "Path from " << i << std::endl;
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			std::cout << " to " << j << std::endl;
			GeneratePath(rooms[i].getCenter(), rooms[j].getCenter());
		}
	}
}

void SetupMaze()
{
	unsigned int i, j, k;

	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			maze[i][j].SetValue(WALL);
			maze[i][j].getPoint().setRow(i);
			maze[i][j].getPoint().setCol(j);
		}
	}
	//Create rooms
	for (numExistingRooms = 0; numExistingRooms < NUM_ROOMS; numExistingRooms++)
	{
		rooms[numExistingRooms] = GenerateRoom();
	}

	//Create random walls
	for (k = 0; k < 100; k++)
	{
		i = rand() % MSZ;
		j = rand() % MSZ;
		maze[i][j].SetValue(WALL);
	}

	// create and place players 
	vector_players.push_back(new Player(TEAM1, (rand() % 2) + 1));
	vector_players.push_back(new Player(TEAM2, (rand() % 2) + 1));
	vector_players.push_back(new Player(TEAM1, (rand() % 2) + 1));
	vector_players.push_back(new Player(TEAM2, (rand() % 2) + 1));

	// randomize room and place in room
	for (i = 0; i < vector_players.size(); i++)
	{
		int randomRoom = rand() % NUM_ROOMS;
		int h = rooms[randomRoom].getHeight();
		int w = rooms[randomRoom].getWidth();
		int x = rooms[randomRoom].getLeftTop().getCol();
		int y = rooms[randomRoom].getLeftTop().getRow();
		int randW = 0;
		int randH = 0;
		int row = 0;
		int column = 0;
		
		do
		{
			randW = rand() % w;
			randH = rand() % h;
		} while (maze[rooms[randomRoom].getLeftTop().getRow() + randH][rooms[randomRoom].getLeftTop().getCol() + randW].GetValue() != SPACE);

		row = rooms[randomRoom].getLeftTop().getRow() + randH;
		column = rooms[randomRoom].getLeftTop().getCol() + randW;

		vector_players[i]->setLocation(&maze[row][column]);
		vector_players[i]->findCurrentRoom();
		maze[rooms[randomRoom].getLeftTop().getRow() + randH][rooms[randomRoom].getLeftTop().getCol() + randW].SetValue(vector_players[i]->getTeam());
		int v = vector_players[i]->getVariant();
		if(v == POWERSHOT)
			std::cout << "Player at " << row << "," << column << " using Powershot"<<std::endl;
		else
			std::cout << "Player at " << row << "," << column << " using Doubleshot" << std::endl;
	}

	// create and place health and ammunition
	for (i = 0; i < NUM_ROOMS; i++)
	{
		//add two health packs
		for (int j = 0; j < 2; j++)
		{
			int h = rooms[i].getHeight();
			int w = rooms[i].getWidth();
			int x = rooms[i].getLeftTop().getCol();
			int y = rooms[i].getLeftTop().getRow();
			int randW = 0;
			int randH = 0;

			do
			{
				randW = rand() % w;
				randH = rand() % h;
			} while (maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW].GetValue() != SPACE);

			maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW].setItem(HEALTH);
			rooms[i].setHealth(rooms[i].getHealth() + 1);
			rooms[i].getHealthVector().push_back(&maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW]);
		}

		//add two ammo packs
		for (int j = 0; j < 2; j++)
		{
			int h = rooms[i].getHeight();
			int w = rooms[i].getWidth();
			int x = rooms[i].getLeftTop().getCol();
			int y = rooms[i].getLeftTop().getRow();
			int randW = 0;
			int randH = 0;
			do
			{
				randW = rand() % w;
				randH = rand() % h;
			} while (maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW].GetValue() != SPACE && maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW].getItem() != SPACE);
			maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW].setItem(AMMO);
			rooms[i].setAmmo(rooms[i].getAmmo() + 1);
			rooms[i].getAmmunitionVector().push_back(&maze[rooms[i].getLeftTop().getRow() + randH][rooms[i].getLeftTop().getCol() + randW]);
		}
	}

	// After all variables are added - dig tunnels
	DigTunnels();
}

void DrawMaze()
{
	int i, j;
	double sz, x, y;

	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			// set color
			switch (maze[i][j].GetValue())
			{
			case TEAM1:
				glColor3d(1, 0, 0);			// red 
				break;
			case TEAM2:
				glColor3d(0, 1, 0);			// Green
				break;
			case SPACE:
				switch (maze[i][j].getItem())
				{
				case AMMO:
					glColor3d(0.4, 0, 0);	// dark red
					break;
				case HEALTH:
					glColor3d(0, 0.8, 1);	// azure
					break;
				default: 
					glColor3d(1, 1, 1);		// white
					break;
				}
				break;
			case WALL:
				glColor3d(0, 0, 0);			// black
				break;
			}

			// draw rectangle
			sz = 2.0 / MSZ;
			x = j * sz - 1;
			y = i * sz - 1;
			
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x + sz, y);
			glVertex2d(x + sz, y + sz);
			glVertex2d(x, y + sz);
			glEnd();
		}
	}
}

void createMap()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			if (maze[i][j].GetValue() == SPACE)
			{
				maze[i][j].setSafety(map[i][j]);
			}
		}
	}
}

void GenerateMap()
{
	int num_tries = 1000;
	int i;
	int col, row;
	double x, y, sz;
	Granade* pg = nullptr;

	for (i = 0; i < num_tries; i++)
	{
		do
		{
			col = rand() % MSZ;
			row = rand() % MSZ;
		} while (maze[row][col].GetValue() != SPACE);
		
		sz = 2.0 / MSZ;
		x = col * sz - 1;
		y = row * sz - 1;
		pg = new Granade(x, y);
		pg->SimulateExplosion(map, maze);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	DrawMaze();
	
	if (pb != nullptr)
	{
		pb->showMe();
	}

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

bool CheckIsSpace(double dx, double dy)
{
	int i, j;

	i = MSZ * (dy + 1) / 2;
	j = MSZ * (dx + 1) / 2;

	return  maze[i][j].GetValue() == SPACE;
}

void printPlayerData()
{
	for (unsigned int i = 0; i < vector_players.size(); i++)
	{
		char* variant = vector_players[i]->getVariant() == DOUBLESHOT ? "Doubleshot" : "Powershot";
		char* team = vector_players[i]->getTeam() == TEAM1 ? "Team 1" : "Team 2";
		std::cout << vector_players[i] << ", Team = "<< team<<", Variant = " <<  variant << ", Health= " << vector_players[i]->getHealth() << ", Ammo = " << vector_players[i]->getBullets() << " Timestamp = "<< counter/10<<std::endl;
	}
	std::cout << "" << std::endl;
}

void idle()
{
	int everyNTurns = 10;
	if (proceed)
	{
		counter++;
		if (counter % everyNTurns == 0)
		{
			printPlayerData();
		}
		for (unsigned int i = 0; i < vector_players.size(); i++)
		{
			bool takeFire = false;
			if (vector_players[i]->getHealth() <= 0)
			{
				if (vector_players[i]->getTeam() == TEAM1)
				{
					t1Deaths++;
				}
				if (vector_players[i]->getTeam() == TEAM2)
				{
					t2Deaths++;
				}
				vector_players[i]->getLocation()->SetValue(SPACE);
				vector_players.erase(vector_players.begin() + i);
				std::cout << "Only " << vector_players.size() << " left" << std::endl;
				if (t1Deaths == 2 || t2Deaths == 2)
				{
					std::cout << "Game Over. Someone won." << std::endl;
					display();
					proceed = false;
				}
			}
			else
			{
				takeFire = vector_players[i]->takeTurn(vector_players);
				int row = vector_players[i]->getLocation()->getPoint().getRow();
				int column = vector_players[i]->getLocation()->getPoint().getCol();

				if (takeFire)
				{
					Player* target = vector_players[i]->getCurrentTarget();
					int x1 = vector_players[i]->getLocation()->getPoint().getCol();
					int x2 = target->getLocation()->getPoint().getCol();
					int y1 = vector_players[i]->getLocation()->getPoint().getRow();
					int y2 = target->getLocation()->getPoint().getRow();

					double currentDistance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
					double angleInRadian = atan2(((double)y1 - (double)y2), ((double)x1 - (double)x2));
					angleInRadian += PI;

					double xx = (x1 / (double)MSZ) * 2 - 1;
					double yy = (y1 / (double)MSZ) * 2 - 1;
					double damage = 8;
					bool isPowerful = false;
					double changeRate = 0.01; 
					if (vector_players[i]->getVariant() == POWERSHOT)
					{
						damage *= 1.5;
						isPowerful = true; //changes bullet color to indicates it's more powerful
					}

					if (!(target->getLocation() == vector_players[i]->getLocation())) //They're on top of one another. Don't draw bullet
					{
						pb = new Bullet(xx, yy, angleInRadian, isPowerful);
						while (pb->GetIsMoving())
						{
							pb->move(maze, changeRate, vector_players[i]->getTeam());
							display();
							if (damage > 2)
							{
								damage -= changeRate * 10;
							}
						}
					}
					vector_players[i]->setBullets(vector_players[i]->getBullets() - 1);
					target->setHealth(target->getHealth() - damage);

					//doubleshot takes another shot if has ammo
					changeRate = 0.01; //reset changeRate
					if (vector_players[i]->getVariant() == DOUBLESHOT && vector_players[i]->getBullets() > 0)
					{
						double damage = 8;
						bool isPowerful = false;
						if (vector_players[i]->getVariant() == POWERSHOT)
						{
							damage *= 1.6;
							isPowerful = true; //changes bullet color
						}

						if (!(target->getLocation() == vector_players[i]->getLocation())) //They're on top of one another. Don't draw bullet
						{
							pb = new Bullet(xx, yy, angleInRadian, isPowerful);
							double changeRate = 0.01;
							while (pb->GetIsMoving())
							{
								pb->move(maze, changeRate, vector_players[i]->getTeam());
								display();
								if (damage > 2)
								{
									damage -= changeRate * 10;
								}
							}
						}
						vector_players[i]->setBullets(vector_players[i]->getBullets() - 1);
						target->setHealth(target->getHealth() - damage);
					}
					if (target->getHealth() <= 0)
					{
						std::cout << "Target from team " << target->getTeam() << " died." << std::endl;
					}
					pb = nullptr; //ensure old bullets disappear
				}
			}

		}
	}

	Sleep(50);	//sleep to make the players appear slower

	glutPostRedisplay();	// calls indirectly to display
}

void Menu(int choice)
{
	if (choice == 1)	// let the games begin
	{
		proceed = true;
		glutDisplayFunc(display);
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Dungeon");

	// display and game logic 
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	// menu
	glutCreateMenu(Menu);
	glutAddMenuEntry("Start Game", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}