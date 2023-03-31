#pragma once
#include"Action.h"
#include"Bullet.h"
#include"threadPool.h"


struct Plant :public Action
{
	Plant(int x, int y) :
		hp_(100),
		sun_(0),
		bullet_{ new Bullet() },
		shootInterval_{ 2500 },//ms
		lastShootTime_{ SDL_GetTicks() },
		front_{}
	{
		//setRect(SDL_Rect{ 0,0,60,60 });
		setPos(x, y);

		bullet_->setPos(this->pos_.x_ +15, this->pos_.y_);
	}

	Plant(const Position&pos) :
		hp_(100),
		sun_(0),
		bullet_{ new Bullet() },
		shootInterval_{ 2500 },//ms
		lastShootTime_{ SDL_GetTicks() },
		front_{}
	{
		//setRect(SDL_Rect{ 0,0,60,60 });
		setPos(pos);

		bullet_->setPos(this->pos_.x_ + 15, this->pos_.y_);
	}


	virtual ~Plant() = default;

	bool rodeJudge(const Position& zombiePos)
	{
		if (zombiePos.x_ <= 1250 && pos_.y_ == noCopy::findY(zombiePos.y_))
			return true;

		return false;
	}

	virtual int attack(SDL_Renderer* render, const Position& zombie, bool die)
	{

		int temp = -1;


		if (!isDie())
		{	
			uint32_t currentTime = SDL_GetTicks();
			//bullet_->setTouch(false);

			// �жϽ�ʬ�Ƿ��ڹ�����Χ�����������ӵ�λ��
			bool bulletFlag = false;
			if (rodeJudge(zombie) && !die)
			{
				
				//���ü��ʱ��
				bulletFlag = true;
				if (currentTime - lastShootTime_ >= shootInterval_&&front_)
				{
					shoot(render);
					lastShootTime_ = currentTime;
					
					front_ = false;
				}

				if (!bullet_->isQuit(ActMode::HeadAttack)&&!isDie())
				{
					auto newPos = bullet_->move();
					bullet_->setPos(newPos);

				}

				// �жϽӴ�����ʬ
				if (bullet_->getPos().x_ >= zombie.x_ + 40)
				{

					//�Ƿ񲥷��ӵ�����
					bulletFlag = false;

					temp = bullet_->getFire();

					bullet_->setPos(-1000, -100);

					//��һ���ӵ��Ӵ�����ʬ
					front_ = true;
				}
				

			}

			

			

			this->playImage(render, bulletFlag);



		}
	


		return temp;
	}
	virtual void playImage(SDL_Renderer* render, bool startPlay) = 0;

	virtual int demage() = 0;

	virtual int getSunPrice() = 0;

	virtual bool shoot(SDL_Renderer*) = 0;

	void setSun(const int& s)
	{
		sun_ = s;
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

	Position getBulletPos()
	{
		return bullet_->getPos();
	}

	bool getBulletTouch()
	{
		return bullet_->getTouch();
	}

protected:
	int hp_;

	int sun_;

	std::shared_ptr< Bullet> bullet_;

	bool front_;


private:
	uint32_t shootInterval_;
	uint32_t lastShootTime_;
};