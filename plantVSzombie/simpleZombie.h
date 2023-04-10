#pragma once
#include"Zombie.h"
#include"Music.h"
#include<string>
class simpleZombie final :public Zombie
{
public:
	using Action::getPos;

	simpleZombie(int x, int y) :
		Zombie(x, y),
		random{},
		diePlay_{},
		headPlay_{}
	{
		random = (SDL_GetTicks()-rand()) % 3 ;
		if (random  == 0)
		{
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move3\\Frame", ActMode::HeadMove);
		}
		else if(random  == 1)
		{
			//"E:\GamePicture\image\SimpleZombie\move2\Frame26.bmp"
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move2\\Frame", ActMode::HeadMove);
		}
		else
		{
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move1\\Frame", ActMode::HeadMove);
		}
		
		setPath("E:\\GamePicture\\image\\SimpleZombie\\attack\\Frame", ActMode::HeadAttack);

		setPath("E:\\GamePicture\\image\\SimpleZombie\\noHeadMove\\Frame", ActMode::NoHeadMove);
		setPath("E:\\GamePicture\\image\\SimpleZombie\\noHeadAttack\\Frame", ActMode::NoHeadAttack);

		setPath("E:\\GamePicture\\image\\SimpleZombie\\die\\Frame", ActMode::Die);

		chunk_.setChunk("E:\\GamePicture\\Music\\chomp.ogg");
	}

	simpleZombie(const Position& pos) :
		Zombie(pos),
		random{},
		diePlay_{},
		headPlay_{}
	{
		random = (rand() % 30) % 3;
		if (random  == 0)
		{
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move3\\Frame", ActMode::HeadMove);
		}
		else if (random == 1)
		{
			//"E:\GamePicture\image\SimpleZombie\move2\Frame26.bmp"
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move2\\Frame", ActMode::HeadMove);
		}
		else
		{
			setPath("E:\\GamePicture\\image\\SimpleZombie\\move1\\Frame", ActMode::HeadMove);
		}

		setPath("E:\\GamePicture\\image\\SimpleZombie\\attack\\Frame", ActMode::HeadAttack);

		setPath("E:\\GamePicture\\image\\SimpleZombie\\noHeadMove\\Frame", ActMode::NoHeadMove);
		setPath("E:\\GamePicture\\image\\SimpleZombie\\noHeadAttack\\Frame", ActMode::NoHeadAttack);

		setPath("E:\\GamePicture\\image\\SimpleZombie\\die\\Frame", ActMode::Die);

		chunk_.setChunk("E:\\GamePicture\\Music\\chomp.ogg");
	}

	~simpleZombie()
	{


	};

	void moveSpeed()override
	{
		//move1
		/*int temp = getMoveCount();
		setSpeed(0);

		if (temp >= 5 && temp <= 10) {
			setSpeed(1);
		}
		if (temp >= 18 && temp <= 21) {
			setSpeed(2);
		}*/

		
		/*int temp = getMoveCount(false);
		setSpeed(0);
		if (random == 0)
		{		

			if (temp >= 3 && temp <= 5) {
				setSpeed(1);
			}
			if (temp >= 6 && temp <= 10) {
				setSpeed(2);
			}
			else
			{
				setSpeed(1);
			}

		}
		else if (random == 1)
		{
			

			if (temp >= 5 && temp <= 10) {
				setSpeed(2);
			}
			if (temp >= 18 && temp <= 30) {
				setSpeed(1);
			}

		}
		else
		{
			

			if (temp >= 5 && temp <= 10) {
				setSpeed(1);
			}
			if (temp >= 18 && temp <= 21) {
				setSpeed(2);
			}

		}*/

		setSpeed(1);



	}



	//播放动画
	virtual int attack(SDL_Renderer* render, std::vector<std::shared_ptr<Plant>>& plants)override
	{

		if (!isDie())
		{

				for (const auto& i : plants)
				{

					if (!i->isDie())
					{


						if (touchJudge(i->getPos()))
						{


							setSpeed(0);

							// 播放音效
							
							chunk_.playOnce(3, true);

							i->setHP(fire_);
							if (hp_ >= 25)
							{
								headAttackAction(render, 20);
							}
							else
							{

								noheadAttackAction(render, 10);


							}

							return fire_;
						}
						else 
						{
							continue;
						}




					}


				}


				
				// 移动动画
				moveSpeed();

				if (hp_ >= 25)
					moveAction(render);
				else 
				{



					noheadMoveAction(render, 17);
				}

			
		}
		else
		{
			if (!diePlay_) 
			{

				setSpeed(0);

				dieAction(render, 9);

				//播放死亡音效
				if (getCount(ActMode::Die) == 1)
					chunk_.setChunk("E:\\GamePicture\\Music\\zombie_falling_1.ogg");

				chunk_.playOnce(5,true);

				if (getCount(ActMode::Die) == 9) {					


					diePlay_ = true;
				}
					

			}


		}

		auto newPos = move();
		setPos(newPos);

		return fire_;

	}

	bool getDiePlay()override
	{
		return diePlay_;
	}

	virtual int demage(int fire)override
	{
		return fire;
	}

	virtual void moveAction(SDL_Renderer* render)
	{
		int temp{};
		if (random == 0)
		{
			temp = 17;
		}
		else if (random == 1)
		{
			temp = 30;
		}
		else
		{
			temp = 21;
		}

		Action::headMoveAction(render, temp);

	}





private:

	// 是否播放过死亡动画
	bool diePlay_;

	// 是否播放过掉头动画
	bool headPlay_;

	unsigned int random;

	Music chunk_;
};