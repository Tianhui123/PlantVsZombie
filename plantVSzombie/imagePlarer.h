#pragma once
#include<SDL.h>
#include<vector>

struct imagePlarer
{
	imagePlarer() :
		imageArrTexture_{},
		imageCount_(0),
		imagePath_(nullptr)
	{}

	~imagePlarer()
	{
		for (auto& i : imageArrTexture_)
			if (i)
			{
				SDL_DestroyTexture(i);
				i = nullptr;
			}

		imageArrTexture_.clear();

	}

	std::vector<SDL_Texture*>imageArrTexture_;
	unsigned int imageCount_;
	const char* imagePath_;
};
