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

	// ͼƬ����������
	std::vector<SDL_Texture*>imageArrTexture_;

	// �ڼ���ͼƬ
	unsigned int imageCount_;

	// ͼƬ��·��
	const char* imagePath_;
};
