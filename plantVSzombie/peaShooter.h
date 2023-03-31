#pragma once
#include"Plant.h"
#include"Bullet.h"

class peaShooter final : public Plant
{
public:

	peaShooter(int x,int y):
		Plant(x,y)
	{
		setPath("E:\\GamePicture\\image\\Pea\\Peashooter\\Frame",ActMode::Wait);
		//bullet_.setAttackPath("E:\\GamePicture\\image\\Bullet\\PeaBullet");
		bullet_->setPath("E:\\GamePicture\\image\\Bullet\\fire\\Frame",ActMode::HeadAttack);

		bullet_->setFire(10);

		//bullet_->setPos(this->pos_.x_ +15, this->pos_.y_);
	}

	peaShooter(const Position&pos) :
		Plant(pos)
	{
		setPath("E:\\GamePicture\\image\\Pea\\Peashooter\\Frame", ActMode::Wait);
		//bullet_.setAttackPath("E:\\GamePicture\\image\\Bullet\\PeaBullet");
		bullet_->setPath("E:\\GamePicture\\image\\Bullet\\fire\\Frame", ActMode::HeadAttack);

		bullet_->setFire(10);
		//bullet_->setPos(pos);
	}


	~peaShooter() = default;

	virtual void playImage(SDL_Renderer* render,bool startPlay)override
	{

		waitAction(render, 12);

		if(startPlay)
			bullet_->headAttackAction(render, 2);
	}

	//virtual int attack(SDL_Renderer* render, Action* zombie, bool flag)override
	//{

	//	if (!isDie())
	//	{
	//		Plant::attack(render, zombie, flag);

	//		waitAction(render, 12);


	//		if (hp_ > 0 && zombie->getPos().x_ <= 900 && !zombie->isDie())
	//		{
	//			bullet_.headAttackAction(render, 2);

	//		}

	//	}

	//	return fire_;
	//}

	virtual void waitAction(SDL_Renderer* render, const int& max)override
	{
		Action::waitAction(render, max);
	}




	//int attack(SDL_Renderer*render,const Position& zombie)override
	//{		
	//	auto newPos = bullet_.move();
	//	bullet_.setPos(newPos);
	//	this->waitAction(render, 12);
	//	bullet_.attackAction(render, 2);
	//	
	//	return 1;
	//}

	int demage()override
	{
		return 1;
	}

	int getSunPrice()override
	{
		return 1;
	}

	bool shoot(SDL_Renderer*render)override
	{
		bullet_ = std::shared_ptr<Bullet>{ new Bullet{} };

		bullet_->setPath("E:\\GamePicture\\image\\Bullet\\fire\\Frame", ActMode::HeadAttack);
		bullet_->setPos(this->pos_.x_ + 15, this->pos_.y_);

		return false;
	}


private:

};
