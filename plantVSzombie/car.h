#pragma once
#include"noCopy.h"
#include"Position.h"
#include"Music.h"

class car : noCopy
{
public:
	car(const Position& p) :
		carTexture_{},
		posCar_{ p },
		played_{}
	{
		music_.setChunk("E:\\GamePicture\\Music\\lawnmower.ogg");
	}

	~car()
	{
		if (carTexture_)
			SDL_DestroyTexture(carTexture_);
	}

	void setPath(SDL_Renderer* render, const char* path)
	{
		if (carTexture_ == nullptr)
			carTexture_ = setTouMing(render, path);
	}

	// 渲染出小车
	void drawCar(SDL_Renderer* render)
	{

		allDraw(render,carTexture_);

	}

	// 通用的在指定位置绘制纹理
	void allDraw(SDL_Renderer*render,SDL_Texture* texture)
	{
		SDL_Rect getRect{};
		SDL_QueryTexture(texture, nullptr, nullptr, &getRect.w, &getRect.h);

		getRect.x = posCar_.x_;
		getRect.y = posCar_.y_;


		SDL_RenderCopy(render, texture, nullptr, &getRect);

	}

	Position getPos()
	{
		return posCar_;
	}

	void moveCar()
	{

		if (!played_)
		{
			music_.playOnce(3, true);
			played_ = true;
		}

		posCar_.x_ += 10;
	}


private:
	// 纹理存储
	SDL_Texture* carTexture_;

	// 位置
	Position posCar_;

	// 判断是否播放过音乐
	bool played_;

	// 音乐
	Music music_;


	SDL_Texture* setTouMing(SDL_Renderer*render,const char* file)
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
