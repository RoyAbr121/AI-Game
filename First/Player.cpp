
#include "Player.h"

Player::Player(int team, int variant) : team(team), health(100), bullets(10), variant(variant), currentRoom(nullptr), currentTarget(nullptr)
{
	roomMemory.clear();
}

int Player::getTeam()
{
	return team;
}

double Player::getHealth()
{
	return health;
}

int Player::getBullets()
{
	return bullets;
}

Node* Player::getLocation()
{
	return this->location;
}

Room* Player::getCurrentRoom()
{
	return this->currentRoom;
}

Player* Player::getCurrentTarget()
{
	return currentTarget;
}

int Player::getVariant()
{
	return this->variant;
}

void Player::setLocation(Node* location)
{
	this->location = location;
}

void Player::setHealth(int h)
{
	health = h;
}

void Player::setBullets(int b)
{
	bullets = b;
}

bool Player::behavioralDecision(std::vector<Player*> vector_players)
{
	this->player_priority_queue = std::priority_queue<Node*, std::vector<Node*>, CompareNodes>(); //clear old PQ by creating a new one
	Player* designatedTarget;
	if (this->bullets > MAX_AMMO * AS)	//AS
	{
		if (this->health > (double)MAX_HEALTH * HS)		//HS
		{
			if (this->enemiesInRoom(vector_players))	//RE
			{
				designatedTarget = this->acquireTarget(vector_players);
				
				if (designatedTarget != nullptr)	//V->Fight
				{
					this->currentTarget = designatedTarget;
					designatedTarget->location->setH(distance(this->location, designatedTarget->location));
					this->player_priority_queue.push(designatedTarget->location);
					
					return false;
				}
				else	//~VS ->Spot
				{
					this->acquireTarget(vector_players);
					
					for (auto player : vector_players)
					{
						if (this->getTeam() != player->getTeam())
						{
							if (this->getCurrentRoom() == player->getCurrentRoom())
							{
								player->location->setH(distance(this->location, player->location));
								this->player_priority_queue.push(player->location);
							}
						}
					}
				}
			}
			else	//~RE ->change room
			{
				for (int i = 0; i < NUM_ROOMS; i++)
				{
					if (!roomInMemory(&rooms[i]))
					{
						int x = rooms[i].getCenter().getCol();
						int y = rooms[i].getCenter().getRow();
						maze[y][x].setH(distance(this->location, &maze[y][x]));
						this->player_priority_queue.push(&maze[y][x]);
					}
				}
			}
		}
		else	//~HS
		{
			if (this->health > (double)MAX_HEALTH * HR)		//HR
			{
				if (this->enemiesInRoom(vector_players))	//RE
				{
					designatedTarget = this->acquireTarget(vector_players);
					
					if (designatedTarget != nullptr)	//V->Fight
					{
						this->currentTarget = designatedTarget;
						designatedTarget->location->setH(distance(this->location, designatedTarget->location));
						this->player_priority_queue.push(designatedTarget->location);
						return false;
					}
					else	//~V ->Spot
					{
						this->acquireTarget(vector_players);

						for (auto player : vector_players)
						{
							if (this->getTeam() != player->getTeam())
							{
								if (this->getCurrentRoom() == player->getCurrentRoom())
								{
									player->location->setH(distance(this->location, player->location));
									this->player_priority_queue.push(player->location);
								}
							}
						}
					}
				}
				else	//~RE
				{
					if (this->bullets > MAX_AMMO * AR)	//AR
					{
						if (this->currentRoom->getHealth() > 0)	//RH->Heal
						{
							aquireHealth(vector_players);
						}
						else	//~RH
						{
							if (this->currentRoom->getAmmo() > 0 )	//RA->Rearm
							{
								aquireAmmunition(vector_players);
							}
							else	//~RA ->change room
							{
								for (int i = 0; i < NUM_ROOMS; i++)
								{
									if (!roomInMemory(&rooms[i]))
									{
										int x = rooms[i].getCenter().getCol();
										int y = rooms[i].getCenter().getRow();
										maze[y][x].setH(distance(this->location, &maze[y][x]));
										this->player_priority_queue.push(&maze[y][x]);
									}
								}
							}
						}
					}
					else	//~AR ->Rearm
					{
						aquireAmmunition(vector_players);
					}
				}
			}
			else	//~HR->heal
			{
				aquireHealth(vector_players);
			}
		}
	}
	else	//~AS
	{
		if (this->health > (double)MAX_HEALTH * HS)		//HS
		{
			if (this->bullets > MAX_AMMO * HR)		//AR
			{
				if (this->enemiesInRoom(vector_players))	//RE
				{
					designatedTarget = this->acquireTarget(vector_players);
					
					if (designatedTarget != nullptr)	//V->Fight
					{
						this->currentTarget = designatedTarget;
						designatedTarget->location->setH(distance(this->location, designatedTarget->location));
						this->player_priority_queue.push(designatedTarget->location);
						return false;
					}
					else	//~V ->Spot
					{
						this->acquireTarget(vector_players);

						for (auto player : vector_players)
						{
							if (this->getTeam() != player->getTeam())
							{
								if (this->getCurrentRoom() == player->getCurrentRoom())
								{
									player->location->setH(distance(this->location, player->location));
									this->player_priority_queue.push(player->location);
								}
							}
						}
					}
				}
				else	//~RE ->Change room
				{
					for (int i = 0; i < NUM_ROOMS; i++)
					{
						if (!roomInMemory(&rooms[i]))
						{
							int x = rooms[i].getCenter().getCol();
							int y = rooms[i].getCenter().getRow();
							maze[y][x].setH(distance(this->location, &maze[y][x]));
							this->player_priority_queue.push(&maze[y][x]);
						}
					}
				}
			}
			else	//~AR->Rearm
			{
				aquireAmmunition(vector_players);
			}
		}
		else	//~HS
		{
			if (this->health > (double)MAX_HEALTH * HR)		//HR
			{
				if (this->bullets > MAX_HEALTH * AR)	//AR
				{
					if (this->enemiesInRoom(vector_players))	//RE
					{
						designatedTarget = this->acquireTarget(vector_players);
						if (designatedTarget != nullptr)	//V->Fight
						{
							this->currentTarget = designatedTarget;
							designatedTarget->location->setH(distance(this->location, designatedTarget->location));
							this->player_priority_queue.push(designatedTarget->location);
							return false;
						}
						else	//~V ->Spot
						{
							this->acquireTarget(vector_players);

							for (auto player : vector_players)
							{
								if (this->getTeam() != player->getTeam())
								{
									if (this->getCurrentRoom() == player->getCurrentRoom())
									{
										player->location->setH(distance(this->location, player->location));
										this->player_priority_queue.push(player->location);
									}
								}
							}
						}
					}
					else	//~RE
					{
						if (this->currentRoom->getHealth() > 0)		//RH->Heal
						{
							aquireHealth(vector_players);
						}
						else	//~RH
						{
							if (this->currentRoom->getAmmo() > 0)	//RA->Rearm
							{
								aquireAmmunition(vector_players);
							}
							else	//~RA->change room
							{
								for (int i = 0; i < NUM_ROOMS; i++)
								{
									if (!roomInMemory(&rooms[i]))
									{
										int x = rooms[i].getCenter().getCol();
										int y = rooms[i].getCenter().getRow();
										maze[y][x].setH(distance(this->location, &maze[y][x]));
										this->player_priority_queue.push(&maze[y][x]);
									}
								}
							}
						}
					}
				}
				else	//~AR ->Rearm
				{
					aquireAmmunition(vector_players);
				}
			}
			else	//~HR->Heal
			{
				aquireHealth(vector_players);
			}
		}
	}

	return true;
}

bool Player::checkWithinRoom(Room* room)
{
	int h = room->getHeight();
	int w = room->getWidth();
	int x = room->getLeftTop().getCol();
	int y = room->getLeftTop().getRow();
	int playerX = this->getLocation()->getPoint().getCol();
	int playerY = this->getLocation()->getPoint().getRow();

	if (playerX >= x && playerX <= x + w)
	{
		if (playerY >= y && playerY <= y + h)
		{
			return true;
		}
	}

	return false;
}

Node* Player::moveToTarget(Node& start, Node& target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	std::vector<Node*> gray;
	std::vector<Node*> black;
	std::vector<Node*>::iterator gray_it;
	std::vector<Node*>::iterator black_it;

	bool stop = false;

	Node* pn = nullptr;

	pq.push(&start);
	gray.push_back(&start);

	if (target.getPoint() == start.getPoint())
	{
		return &target;
	}

	while (!pq.empty() && !stop)
	{
		pn = pq.top();	// take the best Node from pq
		pq.pop();	// remove top Node from pq

		if (pn->getPoint() == target.getPoint())	// the path has been found
		{
			while (!(pn->getParent()->getPoint() == start.getPoint()))
			{
				pn = pn->getParent();
			}

			stop = true;
		}
		else	// pn is not target
		{
			gray_it = find(gray.begin(), gray.end(), pn);

			if (gray_it != gray.end())
			{
				gray.erase(gray_it);
			}

			black.push_back(pn);
			this->CheckNeighbours(pn, gray, black, pq, &target);
		}
	}

	return pn;
}

void Player::CheckNeighbours(Node* pn, std::vector<Node*>& gray, std::vector<Node*>& black, std::priority_queue<Node*, std::vector<Node*>, CompareNodes>& pq, Node* target)
{
	// try down
	if (pn->getPoint().getRow() < MSZ - 1)
	{
		AddNode(pn->getPoint().getRow() + 1, pn->getPoint().getCol(), pn, gray, black, pq, target);
	}
	// try up
	if (pn->getPoint().getRow() > 0)
	{
		AddNode(pn->getPoint().getRow() - 1, pn->getPoint().getCol(), pn, gray, black, pq, target);
	}
	// try left
	if (pn->getPoint().getCol() > 0)
	{
		AddNode(pn->getPoint().getRow(), pn->getPoint().getCol() - 1, pn, gray, black, pq, target);
	}
	// try right
	if (pn->getPoint().getCol() < MSZ - 1)
	{
		AddNode(pn->getPoint().getRow(), pn->getPoint().getCol() + 1, pn, gray, black, pq, target);
	}
}

void Player::AddNode(int row, int col, Node* pn, std::vector<Node*>& gray, std::vector<Node*>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq, Node* target)
{
	std::vector<Node*>::iterator gray_it;
	std::vector<Node*>::iterator black_it;
	double cost;
	Node* next = &maze[row][col];
	
	if (next->GetValue() == SPACE)
	{
		cost = next->getSafety() * SAFETY_COEFFICIENT;		// get the Node's safety value
	}
	else if ((next->GetValue() == TEAM1 || next->GetValue() == TEAM2) && this->getCurrentRoom() == nullptr)
	{
		cost = next->getSafety() * SAFETY_COEFFICIENT;
	}
	else if (next->GetValue() != WALL)
	{
		cost = next->getSafety() * SAFETY_COEFFICIENT+5;	// +10 limits the rate of overlapping error
	}
	else
	{
		cost = INT_MAX;
	}

	// update the G value on Node
	next->setG(pn->getG() + cost);
	next->setH(next->ComputeH(&target->getPoint()));

	black_it = find(black.begin(), black.end(), next);
	gray_it = find(gray.begin(), gray.end(), next);

	if (black_it == black.end() && gray_it == gray.end())	// it is not black and not gray!
	{
		pq.push(next);
		gray.push_back(next);
		next->setParent(pn);
	}
}

Room* Player::findCurrentRoom()
{
	if (this->roomMemory.size() > NUM_ROOMS / 2)
	{
		this->roomMemory.pop_front();
	}

	for (int i = 0; i < NUM_ROOMS; i++)
	{
		if (this->checkWithinRoom(&rooms[i]))
		{

			if (!roomInMemory(&rooms[i]))
			{
				this->roomMemory.push_back(&rooms[i]);
			}
			return  &rooms[i];
		}
	}

	return nullptr;
}

bool Player::checkSide(Room* currentRoom, int playerX, int playerY, Node* target, int item)
{
	if (maze[playerX][playerY].getItem() == target->getItem())
	{
		target->setItem(SPACE);
		if (item == HEALTH)
		{
			for (unsigned int i = 0; i < currentRoom->getHealthVector().size(); i++)
			{
				if (*target == *currentRoom->getHealthVector()[i])
				{
					currentRoom->getHealthVector().erase(currentRoom->getHealthVector().begin() + i);
					break;
				}
			}

			this->setHealth(100);	//set to full health
		}

		else if (item == AMMO)
		{
			for (unsigned int i = 0; i < currentRoom->getAmmunitionVector().size(); i++)
			{
				if (*target == *currentRoom->getAmmunitionVector()[i])
				{
					currentRoom->getAmmunitionVector().erase(currentRoom->getAmmunitionVector().begin() + i); //
					break;
				}
			}
			this->setBullets(10);	//set to full ammo
		}

		return true;
	}

	return false;
}

void Player::setRoom(Room* room)
{
	this->currentRoom = room;
}

void Player::checkSurroundings(Node* target)
{
	int playerX = this->getLocation()->getPoint().getCol();
	int playerY = this->getLocation()->getPoint().getRow();
	Room* currentRoom = this->getCurrentRoom();
	int item = target->getItem();

	bool cont;
	cont = checkSide(currentRoom, playerX - 1, playerY, target, item);
	if (!cont)
	{
		cont = checkSide(currentRoom, playerX + 1, playerY, target, item);
	}
	if (!cont)
	{
		cont = checkSide(currentRoom, playerX, playerY - 1, target, item);
	}
	if (!cont)
	{
		cont = checkSide(currentRoom, playerX, playerY + 1, target, item);
	}
}

bool Player::takeTurn(std::vector<Player*> vector_players)
{
	bool decision;

	if (this->health <= 0)
	{
		return false;
	}

	this->setRoom(this->findCurrentRoom());
	decision = behavioralDecision(vector_players);
	
	if (decision)
	{
		Node* target = this->player_priority_queue.top();
		Node* moveTo = moveToTarget(*this->location, *target);
		this->location->SetValue(SPACE);
		this->location = moveTo;
		this->location->SetValue(this->getTeam());

		if (target->getItem() != 0 && target->getPoint() == this->getLocation()->getPoint())
		{
			std::vector<Node*>::iterator it;

			if (target->getItem() == HEALTH)
			{
				this->setHealth(MAX_HEALTH);
				it = find(currentRoom->getHealthVector().begin(), currentRoom->getHealthVector().end(), target);
				this->currentRoom->getHealthVector().erase(it);
				this->currentRoom->setHealth(this->currentRoom->getHealth() - 1);
			}
			else if (target->getItem() == AMMO)
			{
				this->setBullets(MAX_AMMO);
				it = find(currentRoom->getAmmunitionVector().begin(), currentRoom->getAmmunitionVector().end(), target);
				this->currentRoom->getAmmunitionVector().erase(it);
				this->currentRoom->setAmmo(this->currentRoom->getAmmo() - 1);
			}

			target->setItem(SPACE);
		}
		return false;
	}
	else // feuer frei
	{
		this->location->SetValue(this->getTeam());

		return true;
	}
}

bool Player::enemiesInRoom(std::vector<Player*> vector_players)
{
	for (auto player : vector_players)
	{
		if (this->getTeam() != player->getTeam())
		{
			if (this->getCurrentRoom() == player->getCurrentRoom() && this->getCurrentRoom() != nullptr)
			{
				return true;
			}
		}
	}

	return false;
}

void Player::aquireHealth(std::vector<Player*> vector_players)
{
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		if (rooms[i].getHealth() != 0)
		{
			if (this->checkWithinRoom(&rooms[i]))
			{
				for (auto it : rooms[i].getHealthVector())
				{
					int itemX = it->getPoint().getCol();
					int itemY = it->getPoint().getRow();

					maze[itemY][itemX].setH(distance(this->location, &maze[itemY][itemX]));
					this->player_priority_queue.push(it);
				}

				continue;
			}

			int x = rooms[i].getCenter().getCol();
			int y = rooms[i].getCenter().getRow();
			maze[y][x].setH(distance(this->location, &maze[y][x]));
			this->player_priority_queue.push(&maze[y][x]);
		}
	}

	if (this->player_priority_queue.empty())
	{
		emergancy_decision(vector_players);		//Go kill or die
	}
}

void Player::emergancy_decision(std::vector<Player*> vector_players)	//Do or die ->give the player omnivision and let him rush the enemy team or die trying.
{
	for (auto player : vector_players)
	{
		if (player->team != this->team)
		{
			int playerX = player->getLocation()->getPoint().getCol();
			int playerY = player->getLocation()->getPoint().getRow();
			maze[playerY][playerX].setH(distance(this->location, &maze[playerY][playerX]));
			this->player_priority_queue.push(player->location);
		}
	}
}

void Player::aquireAmmunition(std::vector<Player*> vector_players)
{
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		if (rooms[i].getAmmo() != 0)
		{
			if (this->checkWithinRoom(&rooms[i]))
			{
				for (auto it : rooms[i].getAmmunitionVector())
				{
					int itemX = it->getPoint().getCol();
					int itemY = it->getPoint().getRow();
					maze[itemY][itemX].setH(distance(this->location, &maze[itemY][itemX]));
					this->player_priority_queue.push(it);
				}
				continue;
			}
			int x = rooms[i].getCenter().getCol();
			int y = rooms[i].getCenter().getRow();
			maze[y][x].setH(distance(this->location, &maze[y][x]));
			this->player_priority_queue.push(&maze[y][x]);
		}
	}

	if (this->player_priority_queue.empty())
	{
		emergancy_decision(vector_players); //Calculate surrender terms
	}
}

Player* Player::acquireTarget(std::vector<Player*> vector_players)
{
	double minDistance = INT_MAX;
	double currentDistance;
	Player* ptr = nullptr;
	double angleInRadian;

	for (auto player : vector_players)
	{
		if (this->getTeam() != player->getTeam())
		{
			if (this->getCurrentRoom() == player->getCurrentRoom())
			{
				int x1 = this->location->getPoint().getCol();
				int y1 = this->location->getPoint().getRow();
				int x2 = player->location->getPoint().getCol();
				int y2 = player->location->getPoint().getRow();

				currentDistance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
				angleInRadian = atan2(y1 - y2, x1 - x2);
				angleInRadian += PI;

				bool hasVision = checkLineOfSight(player, angleInRadian);

				if (currentDistance < minDistance && hasVision)
				{
					currentDistance = minDistance;
					ptr = player;
				}
			}
		}
	}

	return ptr;
}

bool Player::checkLineOfSight(Player* target, double angle)
{
	int otherTeam = target->getTeam();

	int  x = this->getLocation()->getPoint().getCol();
	int  y = this->getLocation()->getPoint().getRow();

	double xx = (x / (double)MSZ) * 2 - 1;
	double yy = (y / (double)MSZ) * 2 - 1;

	Bullet sightFinder(xx, yy, angle, false);
	int i=0, j=0;

	while (sightFinder.GetIsMoving())
	{
		i = (int)(MSZ * (sightFinder.getY() + 1) / 2);
		j = (int)(MSZ * (sightFinder.getX() + 1) / 2);

		if (maze[i][j].GetValue() == otherTeam)
		{
			return true;
		}

		sightFinder.simMove(maze, this->team, otherTeam);
	}

	return maze[i][j].GetValue() == otherTeam;
}

bool Player::roomInMemory(Room* room)
{
	std::deque<Room*>::iterator where = std::find(this->roomMemory.begin(), this->roomMemory.end(), room);

	if (where != this->roomMemory.end())
	{
		return true;
	}

	return false;
}

double Player::distance(Node* node1, Node* node2)
{
	double x1 = node1->getPoint().getCol();
	double x2 = node2->getPoint().getCol();
	double y1 = node1->getPoint().getRow();
	double y2 = node2->getPoint().getRow();

	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}