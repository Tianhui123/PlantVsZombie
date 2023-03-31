#pragma once
#include"noCopy.h"
#include<vector>
#include<string>
#include"Position.h"
#include"imagePlarer.h"


enum class ActMode
{
	Wait,
	HeadMove,
	NoHeadMove,
	HeadAttack,
	NoHeadAttack,
	Die
};

struct Action : noCopy
{
	Action() :
		Move_{},
		Attack_{},
		die_{},
		wait_{},
		pos_{ 1000,45 },
		rect_{pos_.x_,pos_.y_,0,0},
		quit_{}
	{}



	virtual ~Action() = default;

	virtual void waitAction(SDL_Renderer* render, const int& max)
	{
		if (isQuit(ActMode::Wait))
			return;
		action(render, max, ActMode::Wait);
	}

	virtual void headMoveAction(SDL_Renderer*render,const int &max)
	{
		if (isQuit(ActMode::HeadMove))
			return;

		action(render, max, ActMode::HeadMove);

	}

	virtual void noheadMoveAction(SDL_Renderer* render, const int& max)
	{
		if (isQuit(ActMode::NoHeadMove))
			return;

		action(render, max, ActMode::NoHeadMove);

	}

	virtual void headAttackAction(SDL_Renderer* render, const int& max)
	{
		if (isQuit(ActMode::HeadAttack))
			return;

		action(render, max, ActMode::HeadAttack);
	}

	virtual void noheadAttackAction(SDL_Renderer* render, const int& max)
	{
		if (isQuit(ActMode::NoHeadAttack))
			return;

		action(render, max, ActMode::NoHeadAttack);
	}


	virtual void dieAction(SDL_Renderer* render, const int& max)
	{
		if (isQuit(ActMode::Die))
			return;
		action(render, max, ActMode::Die);
	}

	//void setWaitPath(const char* p)
	//{
	//	setPath(p, ActMode::Wait);
	//}
	//void setMovePath(const char* p,const ActMode&mode)
	//{
	//	setPath(p, mode);
	//}
	//void setAttackPath(const char* p)
	//{
	//	setPath(p, ActMode::Attack);
	//}
	//void setDemagePath(const char* p)
	//{
	//	setPath(p, ActMode::Demage);
	//}
	//void setDiePath(const char* p)
	//{
	//	setPath(p, ActMode::Die);
	//}


	int getWide()
	{
		return rect_.w;
	}

	int getHeight()
	{
		return rect_.h;
	}

	Position getPos()
	{
		return pos_;
	}

	void setPos(int x, int y)
	{
		pos_.x_ = x;
		pos_.y_ = y;
		rect_.x = x;
		rect_.y = y;
	}

	void setPos(const Position&p)
	{
		pos_ = p;
		rect_.x = p.x_;
		rect_.y = p.y_;
	}



	unsigned int getMoveCount(const bool& noHead)
	{
		if(noHead)
			return Move_.noheadMove_.imageCount_;
		else
			return Move_.headMove_.imageCount_;
	}

	void setPath(const char* p, const ActMode& mode)
	{

		switch (mode)
		{
		case ActMode::Wait:
			wait_.imagePath_ = p;
			break;

		case ActMode::HeadMove:
			Move_.headMove_.imagePath_ = p;
			break;

		case ActMode::NoHeadMove:
			Move_.noheadMove_.imagePath_ = p;
			break;

		case ActMode::HeadAttack:
			Attack_.headAttack_.imagePath_ = p;
			break;

		case ActMode::NoHeadAttack:
			Attack_.noheadAttack_.imagePath_ = p;
			break;

		case ActMode::Die:
			die_.imagePath_ = p;
			break;

		}


	}

	void setQuit(bool q,const ActMode&mode)
	{
		switch (mode)
		{
		case ActMode::Wait:
			quit_.waitQuit_ = q;

			break;

		case ActMode::HeadMove:
			quit_.headMoveQuit_ = q;

			break;

		case ActMode::NoHeadMove:
			quit_.noheadMoveQuit_ = q;

			break;

		case ActMode::HeadAttack:
			quit_.headAttackQuit_ = q;

			break;

		case ActMode::NoHeadAttack:
			quit_.noheadAttackQuit_ = q;

			break;

		case ActMode::Die:
			quit_.dieQuit_ = q;
			break;

		}

		return;
	}
	
	inline bool isQuit(const ActMode& mode)
	{
		switch (mode)
		{
		case ActMode::Wait:
			return quit_.waitQuit_;
			break;

		case ActMode::HeadMove:
			return quit_.headMoveQuit_;
			break;

		case ActMode::NoHeadMove:
			return quit_.noheadMoveQuit_;
			break;

		case ActMode::HeadAttack:
			return quit_.headAttackQuit_;
			break;

		case ActMode::NoHeadAttack:
			return quit_.noheadAttackQuit_;
			break;


		case ActMode::Die:
			return quit_.dieQuit_;
			break;

		}

	}

	virtual void setHP(int fire)
	{

	};

	virtual bool isDie()
	{
		return false;
	}

protected:
	Position pos_;



	struct Quit
	{
		Quit():
			waitQuit_{},
			headMoveQuit_{},
			noheadMoveQuit_{},
			headAttackQuit_{},
			noheadAttackQuit_{},
			dieQuit_{}
		{}

		~Quit() = default;

		bool waitQuit_;

		bool headMoveQuit_;

		bool noheadMoveQuit_;

		bool headAttackQuit_;

		bool noheadAttackQuit_;

		bool dieQuit_;

	};



private:
	imagePlarer wait_;

	struct Move
	{
		Move() :headMove_{}, noheadMove_{} {};
		~Move() {};
		imagePlarer headMove_;
		imagePlarer noheadMove_;
	};
	
	struct Attack
	{
		Attack() :headAttack_{}, noheadAttack_{} {};
		~Attack() {};

		imagePlarer headAttack_;
		imagePlarer noheadAttack_;

	};

	Move Move_;

	Attack Attack_;

	imagePlarer die_;

	Quit quit_;

	SDL_Rect rect_;

	

	void setTouMing(const char* file, SDL_Renderer*& render, std::vector<SDL_Texture*>*& arr)
	{
		auto peopleSurface = IMG_Load(file);
		if (!peopleSurface)
		{
			printf("Error open picture!\n\r");
			printf("%s\n\r", file);
			return;
		}

		rect_.w = peopleSurface->clip_rect.w;
		rect_.h = peopleSurface->clip_rect.h;

		SDL_SetColorKey(peopleSurface, SDL_TRUE, SDL_MapRGB(peopleSurface->format, 255, 255, 255));

		auto temp = SDL_CreateTextureFromSurface(render, peopleSurface);
		if (!temp)
		{
			printf("Error Create Texture!\n\r");
			return;
		}

		SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_BLEND);

		arr->push_back(temp);
		temp = nullptr;

		SDL_FreeSurface(peopleSurface);

	}

protected:
	void action(SDL_Renderer* render,const int &max,const ActMode&mode)
	{

		unsigned int *count=nullptr;
		const char* Path = nullptr;
		std::vector<SDL_Texture*>* arr = nullptr;
		SDL_Rect* rect = nullptr;

		switch (mode)
		{
		case ActMode::Wait:
			arr = &wait_.imageArrTexture_;
			count = &wait_.imageCount_;
			Path = wait_.imagePath_;

			rect = &rect_;

			break;

		case ActMode::HeadMove:

			arr = &Move_.headMove_.imageArrTexture_;
			count = &Move_.headMove_.imageCount_;
			Path = Move_.headMove_.imagePath_;

			rect = &rect_;

			break;

		case ActMode::NoHeadMove:
			arr = &Move_.noheadMove_.imageArrTexture_;
			count = &Move_.noheadMove_.imageCount_;
			Path = Move_.noheadMove_.imagePath_;

			rect = &rect_;

			break;

		case ActMode::HeadAttack:
			arr = &Attack_.headAttack_.imageArrTexture_;
			count = &Attack_.headAttack_.imageCount_;
			Path = Attack_.headAttack_.imagePath_;
			
			rect = &rect_;

			break;

		case ActMode::NoHeadAttack:
			arr = &Attack_.noheadAttack_.imageArrTexture_;
			count = &Attack_.noheadAttack_.imageCount_;
			Path = Attack_.noheadAttack_.imagePath_;

			rect = &rect_;

			break;

		case ActMode::Die:
			arr = &die_.imageArrTexture_;
			count = &die_.imageCount_;
			Path = die_.imagePath_;

			rect = &rect_;

			break;

		}

		if (count!=nullptr&&*count >= max)
			*count = 0;

		if (Path && count) 
		{
			std::string path = Path;
			path += std::to_string(*count) + ".bmp";

			if(arr->size()<= max)
				setTouMing(path.c_str(), render,arr);
			
			SDL_RenderCopy(render, (*arr)[(*count)++], nullptr, rect);
		}
		
		

		
	}




};