#pragma once
#include"Action.h"

class Plant : public Action
{
public:
	Plant():
		hp_{100},
		sunCost_{0}
	{}
	virtual ~Plant() = default;

	virtual int getSunPrice() = 0;

	virtual int draw(SDL_Renderer* render, const Position& zombie, bool die) = 0;

	virtual int attack(SDL_Renderer* render, const Position& zombie, bool die)
	{
		return 1;
	}

	virtual void playImage(SDL_Renderer* render, bool startPlay) = 0;


	void setSun(const int& s)
	{
		sunCost_ = s;
	}

	bool isDie()
	{
		return hp_ <= 0;
	}

	void setHP(int fire)override
	{
		if (hp_ <= 0)
			return;
		hp_ -= fire;
	}


protected:
	int hp_;

	int sunCost_;


};

