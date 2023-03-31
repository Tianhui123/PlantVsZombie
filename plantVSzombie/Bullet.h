#pragma once
#include"Action.h"

class Bullet :public Action
{
public:

	Bullet() :
		speed_(16),
		fire_(10),
		time_(5),
		touch_(false)
	{
	};

	~Bullet() = default;



	Position move()
	{
		return Position(pos_.x_+=speed_,pos_.y_);
	}

	bool getTouch()
	{
		return touch_;
	}

	int getFire()
	{
		return fire_;
	}

	void setFire(int f)
	{
		fire_ = f;
	}

	void setTouch(const bool& fla)
	{
		touch_ = fla;
	}

private:

	int speed_;

	int fire_;

	int time_;

	bool touch_;

};

