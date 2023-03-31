#pragma once
#include"Position.h"
#include"Action.h"
#include"noCopy.h"
#include"attackPlant.h"

struct Zombie :public Action
{
	Zombie(int x,int y):
		hp_(100),
		speed_(1),
		fire_(2)
	{
		setPos(x,y);
	}

	Zombie(const Position&pos) :
		hp_(100),
		speed_(1),
		fire_(2)
	{
		setPos(pos);
	}

	// µôÍ· 
	virtual void headAction(SDL_Renderer* render, const int& max=11)
	{
		//void action(SDL_Renderer * render, const int& max, const ActMode & mode)

		setPath("E:\\GamePicture\\image\\SimpleZombie\\head\\Frame",ActMode::Die);
		action(render, max, ActMode::Die);

	}

	
	//Ë¤µ¹
	virtual void dieAction(SDL_Renderer* render, const int& max = 9)
	{
		//void action(SDL_Renderer * render, const int& max, const ActMode & mode)

		setPath("E:\\GamePicture\\image\\SimpleZombie\\die\\Frame", ActMode::Die);
		action(render, max, ActMode::Die);

	}


	virtual int attack(SDL_Renderer* render, std::vector<std::shared_ptr<Plant>>& plants)
	{
	
		return fire_;
	}

	void move_demage_die()
	{

	}

	bool touchJudge(const Position& pos)
	{

		if (pos.x_ >= pos_.x_ && noCopy::findY(pos_.y_) == pos.y_)
		{
			return true;
		}

		return false;
	}

	virtual int demage(int dem) = 0;

	virtual void moveSpeed() = 0;

	Position move()
	{		
		return Position(pos_.x_ -= speed_, pos_.y_);
	}

	void setSpeed(int speed)
	{
		speed_ = speed;
	}

	bool isDie()override
	{
		return hp_<=0;
	}

	void setHP(int fire)override
	{
		if (hp_ <= 0)
			return;
		hp_ -= fire;
	}

protected:

	int hp_;

	int speed_;

	int fire_;



};


