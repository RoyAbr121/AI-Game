
#pragma once
#include <iostream>

class Point2D
{
public:
	Point2D();
	Point2D(int row, int column);
	~Point2D();

	int getRow();
	int getCol();
	void setRow(int r);
	void setCol(int c);

	const bool operator == (const Point2D &other)
	{
		return row == other.row && col == other.col;
	}

private:
	int row, col;
};

