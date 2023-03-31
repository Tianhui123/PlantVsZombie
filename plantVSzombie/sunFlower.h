#pragma once
#include"producerPlant.h"
#include<string>
#include"Sun.h"

class sunFlower : public producePlant
{
public:
	sunFlower(const Position& p) :
		sun_{ new Sun(Position(pos_.x_ , pos_.y_ )) },
		producePlant(p),
		count_{0}
	{

		producerCount_ = 50;



	}

	~sunFlower()
	{

		for (auto& i : arrTexture_)
		{
			if (i)
				SDL_DestroyTexture(i);

		}

		arrTexture_.clear();

	}

	int getSunPrice()
	{
		return 25;
	}

	void setPos(const Position& p)
	{
		pos_ = p;
	}

	void set(SDL_Renderer* render)
	{

		for (int i = 0; i <= 17; ++i)
		{
			std::string path{ "E:\\GamePicture\\image\\SunFlower\\sunFlower\\Frame" };
			path += (std::to_string(i) + ".bmp");

			arrTexture_.push_back(setTouMing(render,path.c_str()));
		}

	}

	virtual void playImage(SDL_Renderer* render, bool startPlay, bool isPointSun)
	{
		draw(render, Position(0, 0), false,isPointSun);
	}

	int draw(SDL_Renderer* render, const Position& zombie, bool die,bool isPointSun)override
	{
		if (arrTexture_.size() == 0)
		{
			set(render);
		}

		if (!isDie()) 
		{


			if (count_ >= 18)
				count_ = 0;

			SDL_Rect rect{};

			SDL_QueryTexture(arrTexture_[count_], nullptr, nullptr, &rect.w, &rect.h);

			rect.x = pos_.x_;
			rect.y = pos_.y_;

			SDL_RenderCopy(render, arrTexture_[count_++], nullptr, &rect);


		}

		if (isPointSun)
			sun_->live_ = false;

		//sun_->drawSun(render);
		produce(render);

		return 1;
	}

	int produce(SDL_Renderer* render)override
	{
		if (isDie())
			return -1;

		

		//º‰∏Ù ±º‰
		if (produceTime_.queryTime(8000))
		{
			sun_ = std::shared_ptr<Sun>(new Sun(Position(pos_.x_ + 5, pos_.y_ - 5)));

			return -1;
		}
		else
		{

			sun_->drawSun(render);

			return producerCount_;

		}


		

	}


	bool isSunLive()
	{
		if (sun_.get() != nullptr)
			return sun_->getSunLive();

		return false;
	}

private:
	std::shared_ptr<Sun> sun_;

	std::vector<SDL_Texture*>arrTexture_;

	unsigned int count_;

	using producePlant::producerCount_;

	using producePlant::produceTime_;
	
	using producePlant::pos_;

	using producePlant::hp_;



	SDL_Texture* setTouMing(SDL_Renderer* render, const char* file)
	{
		auto peopleSurface = IMG_Load(file);
		if (!peopleSurface)
		{
			printf("Error open picture!\n\r");
			return nullptr;
		}

		SDL_SetColorKey(peopleSurface, SDL_TRUE, SDL_MapRGB(peopleSurface->format, 255, 255, 255));

		auto temp = SDL_CreateTextureFromSurface(render, peopleSurface);
		if (!temp)
		{
			printf("Error Create Texture!\n\r");
			return nullptr;
		}

		SDL_FreeSurface(peopleSurface);

		return temp;
	}


};
