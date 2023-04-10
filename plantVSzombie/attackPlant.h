#pragma once
#include"Plant.h"
#include"Bullet.h"
#include"Time.h"

struct attackPlant :public Plant
{
	attackPlant(int x, int y) :
		bullet_{ },
		front_{},
		attackedTexture_{}
	{

		setPos(x, y);

		//bullet_->setPos(this->pos_.x_ +15, this->pos_.y_);
	}

	attackPlant(const Position&pos) :
		bullet_{ },
		front_{},
		attackedTexture_{}
	{		

		setPos(pos);

		//bullet_->setPos(this->pos_.x_ + 15, this->pos_.y_);
	}


	virtual ~attackPlant()
	{
		if (attackedTexture_)
			SDL_DestroyTexture(attackedTexture_);

	}

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
				if (time_.queryTime(4000)&&front_)
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
					// ���Ż��н�ʬ�Ļ������Ч
					if (attackedTexture_ == nullptr)
						attackedTexture_ = setAttackedTexture(render);

					player(render,attackedTexture_);
					

					//�Ƿ񲥷��ӵ�����
					bulletFlag = false;

					temp = bullet_->getFire();

					bullet_->setPos(-1000, -100);

					//��һ���ӵ��Ӵ�����ʬ
					front_ = true;
				}
				

			}

			

			

			playImage(render, bulletFlag);



		}
	


		return temp;
	}


	int draw(SDL_Renderer* render, const Position& zombie, bool die)override
	{
		return attack(render, zombie, die);
	}

	virtual void playImage(SDL_Renderer* render, bool startPlay)override
	{

	}

	// ������Ч�ͻ���
	virtual void player(SDL_Renderer * ,SDL_Texture* texture) = 0;


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

	virtual SDL_Texture* setAttackedTexture(SDL_Renderer* render) = 0;


protected:
	// �ӵ�
	std::shared_ptr< Bullet> bullet_;

	// �ж���һ���ӵ��Ƿ���н�ʬ
	bool front_;


private:
	// ���н�ʬ�Ķ���
	SDL_Texture* attackedTexture_;

	// ʱ����
	Time time_;
};