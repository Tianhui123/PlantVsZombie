#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<vector>
#include"Position.h"
#include"Time.h"

class Sun
{
public:
	Sun(const Position&p):
		count_{},
		pos_(p),
		live_{true},
		jiang_{}

	{

		


	}

	~Sun()
	{
		live_ = false;
		for (auto& i : sunArrTexture_)
		{
			if (i)
				SDL_DestroyTexture(i);

		}

		sunArrTexture_.clear();

	}

	constexpr bool getSunLive()
	{
		return live_;
	}

	void setSunPos(const Position&p)
	{
		pos_ = p;
	}

	void setPath(SDL_Renderer* render)
	{
		for (int i = 0; i < 3; ++i)
		{
			std::string path{ "E:\\GamePicture\\image\\Sun\\Sun" };
			path += (std::to_string(i) + ".png");

			sunArrTexture_.push_back(setTouMing( render,path.c_str()));

		}
	}

	void drawSun(SDL_Renderer * render)
	{
		if (sunArrTexture_.size() == 0)
		{
			setPath(render);
		}

		if (live_)
		{
			SDL_Rect rect{};

			SDL_QueryTexture(sunArrTexture_[2], nullptr, nullptr, &rect.w, &rect.h);

			

			if (jiang_++ <= 2)
				pos_.y_ -= 40;
			else if (jiang_++ <= 6)
				pos_.y_ += 90;
			else
				jiang_ = 100;


			rect.x = pos_.x_ -25;
			rect.y = pos_.y_ -25;
			
				

			SDL_RenderCopy(render, sunArrTexture_[2], nullptr, &rect);


		}



	}

	bool live_;

private:
	std::vector<SDL_Texture*>sunArrTexture_;

	unsigned int count_;

	Position pos_;

	int jiang_;





	SDL_Texture* setTouMing(SDL_Renderer *render,const char* file)
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