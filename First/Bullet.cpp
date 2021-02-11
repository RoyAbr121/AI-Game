
#include "Bullet.h"
#include "GLUT.H"
#include <math.h>

Bullet::Bullet(double x, double y)
{	
	this->x = x;
	this->y = y;
	isMoving = false;
}

Bullet::Bullet(double x, double y, double angle, bool power)
{
	double len;
	this->power = power; //Some bullets are more powerful than others
	this->x = x+0.005; //centers bullet creation 
	this->y = y+0.005; //centers bullet creation 
	dirx = cos(angle);
	diry = sin(angle);
	len = sqrt(dirx * dirx + diry * diry);
	dirx /= len;
	diry /= len;
	isMoving = true;
}

Bullet::~Bullet()
{
}

void Bullet::showMe()
{
	if (power)
	{
		glColor3d(1, 0, 0); // power bullets are something else 
	}
	else
	{
		glColor3d(0, 0, 0); //normal bullets are black
	}
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.01, y);
	glVertex2d(x, y + 0.01);
	glVertex2d(x + 0.01, y);
	glVertex2d(x, y - 0.01);
	glEnd();
}

void Bullet::SetIsMoving(bool move)
{
	isMoving = move;
}

bool Bullet::GetIsMoving()
{
	return isMoving;
}


void Bullet::simMove(Node maze[MSZ][MSZ], int team, int otherTeam)
{
	int i = MSZ * (y + 1) / 2;
	int j = MSZ * (x + 1) / 2;

	if (isMoving && (maze[i][j].GetValue() == SPACE || maze[i][j].GetValue() == team))
	{
		//ii and jj ensures the bullet won't pass the target due to diagonal angles
		double newX = x + 0.005 * dirx;
		double newY = y + 0.005 * diry;
		int ii = MSZ * (newY + 1) / 2;
		int jj = MSZ * (newX + 1) / 2;
		if (maze[ii][j].GetValue() == otherTeam)
		{
			y = newY;
			return;
		}
		else if (maze[i][jj].GetValue() == otherTeam)
		{
			x = newX;
			return;
		}
		x = newX;
		y = newY;

	}
	else
	{
		this->SetIsMoving(false);
	}
}

void Bullet::move(Node maze[MSZ][MSZ], double speed, int team)
{
	int i = MSZ * (y + 1) / 2;
	int j = MSZ * (x + 1) / 2;

	if (isMoving && (maze[i][j].GetValue() == SPACE || maze[i][j].GetValue() == team))
	{
		x += speed * dirx;
		y += speed * diry;
	}
	else
	{
		this->SetIsMoving(false);
	}
}

double Bullet::getX()
{
	return x;
}

double Bullet::getY()
{
	return y;
}

void Bullet::SetDir(double angle)
{
	dirx = cos(angle);
	diry = sin(angle);
}

void Bullet::SimulateMotion(double map[MSZ][MSZ], Node maze[MSZ][MSZ])
{
	int i, j;

	i = MSZ * (y + 1) / 2;
	j = MSZ * (x + 1) / 2;

	while (maze[i][j].GetValue() == SPACE)
	{
		//map[i][j] += delta;
		map[i][j] += 0.0001;
		x += 0.001 * dirx;
		y += 0.001 * diry;
		i = MSZ * (y + 1) / 2;
		j = MSZ * (x + 1) / 2;
	}
}