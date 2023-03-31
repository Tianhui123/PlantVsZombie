#pragma once
#include"Zombie.h"
#include<string>
class simpleZombie final :public Zombie
{
public:
	using Action::getPos;

	simpleZombie(int x, int y) :
		Zombie(x, y),
		random{}
	{
		random = (SDL_GetTicks()-rand()) % 3;
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
	}

	simpleZombie(const Position& pos) :
		Zombie(pos)
	{
		random = (rand() % 30 + 1) % 3;
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

		
		int temp = getMoveCount(false);
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
				setSpeed(3);
			}

		}
		else if (random == 1)
		{
			

			if (temp >= 5 && temp <= 10) {
				setSpeed(3);
			}
			if (temp >= 18 && temp <= 30) {
				setSpeed(2);
			}

		}
		else
		{
			

			if (temp >= 5 && temp <= 10) {
				setSpeed(2);
			}
			if (temp >= 18 && temp <= 21) {
				setSpeed(3);
			}

		}


		auto newPos = move();
		setPos(newPos);

	}



	//²¥·Å¶¯»­
	virtual int attack(SDL_Renderer* render, std::vector<std::shared_ptr<Plant>>& plants)override
	{

		std::sort(plants.begin(), plants.end(),
			[](std::shared_ptr<Plant>& a, std::shared_ptr<Plant>& b) {return a->getPos().x_ > b->getPos().x_; });

		if (plants.size() == 0 &&! isDie())
		{
			moveSpeed();

			

			if (hp_ >= 25)
				moveAction(render);
			else {

				moveSpeed();
				noheadMoveAction(render, 17);
			}

			return fire_;
		}
		else
		{

			for (const auto& i : plants)
			{

				

				if (!isDie() && !i->isDie())
				{
					

					if (touchJudge(i->getPos())&&!isDie())
					{
						

						setSpeed(0);

						/*auto newPos = move();
						setPos(newPos);*/
						i->setHP(fire_);
						if (hp_ >= 25)
							headAttackAction(render, 20);
						else
						{
							
							noheadAttackAction(render, 10);


						}

						break;
					}





				}


				
				moveSpeed();

				if (hp_ >= 25)
					moveAction(render);
				else
				{					
					noheadMoveAction(render, 17);
				}

				break;

			}


		}
		return fire_;

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
	unsigned int random;
};