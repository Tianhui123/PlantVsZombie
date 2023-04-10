#pragma once
#include"Action.h"
#include"Music.h"

class Bullet :public Action
{
public:

	Bullet(const char*bulletMusic) :
		speed_(20),
		fire_(10),
		time_(5),
		touch_(false),
		play_{true},
		bulletMusic_{ }
	{
		bulletMusic_.setChunk(bulletMusic);
		
	};

	~Bullet() = default;

	void headAttackAction(SDL_Renderer* render, const int& max)override
	{
		if (play_) {
			bulletMusic_.playOnce();
			play_ = false;
		}
		
		Action::headAttackAction(render, max);
		
	}

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

	bool play_;

	Music bulletMusic_;

};

