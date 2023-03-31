#pragma once
#include"Plant.h"
#include"Bullet.h"
#include"Time.h"

struct attackPlant :public Plant
{
	attackPlant(int x, int y) :
		bullet_{ new Bullet() },
		front_{}
	{
		setPos(x, y);

		bullet_->setPos(this->pos_.x_ +15, this->pos_.y_);
	}

	attackPlant(const Position&pos) :
		bullet_{ new Bullet() },
		front_{}
	{
		setPos(pos);

		bullet_->setPos(this->pos_.x_ + 15, this->pos_.y_);
	}


	virtual ~attackPlant() = default;

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

			// �жϽ�ʬ�Ƿ��ڹ�����Χ�����������ӵ�λ��
			bool bulletFlag = false;
			if (rodeJudge(zombie) && !die)
			{
				
				//���ü��ʱ��
				bulletFlag = true;
				if (time_.queryTime(3000))
				{
					shoot(render);
					
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

			

			

			playImage(render, bulletFlag,false);



		}
	


		return temp;
	}


	int draw(SDL_Renderer* render, const Position& zombie, bool die, bool isPointSun)override
	{
		return attack(render, zombie, die);
	}

	virtual void playImage(SDL_Renderer* render, bool startPlay, bool isPointSun)override
	{

	}


	virtual int demage() = 0;

	virtual bool shoot(SDL_Renderer*) = 0;

	Position getBulletPos()
	{
		return bullet_->getPos();
	}

	bool getBulletTouch()
	{
		return bullet_->getTouch();
	}

protected:

	std::shared_ptr< Bullet> bullet_;

	bool front_;


private:

	Time time_;
};