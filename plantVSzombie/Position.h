#pragma once
struct Position
{
	Position(int x, int y) :
		x_(x),
		y_(y)
	{}

	~Position() 
	{}

	void set(int x, int y)
	{
		x_ = x;
		y_ = y;
	}

	bool operator ==(const Position& p)noexcept
	{
		if (x_ == p.x_ && y_ == p.y_)
			return true;

		return false;
	}

	int x_;
	int y_;
};