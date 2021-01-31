
#include "Point2D.h"

Point2D::Point2D()
{
	this->row = -1;
	this->col = -1;
}

Point2D::Point2D(int row, int column)
{
	this->row = row;
	this->col = column;
}

Point2D::~Point2D()
{
}

int Point2D::getRow()
{
	return row;
}

int Point2D::getCol() {
	return col;
}

void Point2D::setRow(int r) {
	row = r;
}

void Point2D::setCol(int c)
{
	col = c;
}