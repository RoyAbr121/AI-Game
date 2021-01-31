
#include "Node.h"
#include <math.h>

Node::Node()
{
	this->value = SPACE;
	this->parent = nullptr;
	this->g = 0;
}

Node::Node(Point2D& pt, Point2D* t, int v, double g, Node* pr) {
	this->point = pt;
	this->target = t;
	this->value = v;
	this->parent = pr;
	this->g = g;
	this->h = ComputeH();
}

Node::~Node()
{
}

void Node::SetValue( int value)
{
	this->value = value;
}

int Node::GetValue()
{
	return value;
}

void Node::setItem(int item)
{
	this->item = item;
}

int Node::getItem()
{
	return this->item;
}

void Node::setG(double g)
{
	this->g = g;
}

double Node::getG()
{
	return g;
}

void Node::setH(double h)
{
	this->h = h;
}

double Node::ComputeH()
{
	return sqrt((double)pow(point.getRow() - (double)target->getRow(),2)+
		pow((double)point.getCol() - (double)target->getCol(), 2));
}

double Node::ComputeH(Point2D* target)
{
	return sqrt(pow((double)point.getRow() - (double)target->getRow(), 2) +
		pow((double)point.getCol() - (double)target->getCol(), 2));
}

double Node::getF()
{
	return g+h;
}

void Node::setSafety(double safety)
{
	this->safety = safety;
}

double Node::getSafety()
{
	return this->safety;
}

Point2D& Node::getPoint()
{
	return this->point;
}

Node * Node::getParent()
{
	return parent;
}

Point2D * Node::getTarget()
{
	return target;
}

void Node::setParent(Node* parent)
{
	this->parent = parent;
}