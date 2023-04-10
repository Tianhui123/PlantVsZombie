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

	// 图片的纹理容器
	std::vector<SDL_Texture*>imageArrTexture_;

	// 第几张图片
	unsigned int imageCount_;

	// 图片的路径
	const char* imagePath_;
};
