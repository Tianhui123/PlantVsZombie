#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include"Game.h"

int main(int argc, char* argv[]) {

#if 0




if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
	return 1;
}

int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
if (!(IMG_Init(imgFlags) & imgFlags)) {
	std::cerr << "SDL_image 初始化失败: " << IMG_GetError() << std::endl;
	return 1;
}
SDL_Window* window = SDL_CreateWindow("透明图片示例", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
if (window == nullptr) {
	std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
	return 1;
}

SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
if (renderer == nullptr) {
	std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
	SDL_DestroyWindow(window);
	return 1;
}


// 4. 加载.bmp图片并设置透明色

SDL_Surface* bmpSurface = SDL_LoadBMP("E:\\GamePicture\\image\\SimpleZombie\\Step1\\Frame0.bmp");
if (bmpSurface == nullptr) {
	std::cerr << "图片加载失败: " << SDL_GetError() << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 1;
}

// 设置透明色为某种颜色，例如白色（RGB: 255, 255, 255）
SDL_SetColorKey(bmpSurface, SDL_TRUE, SDL_MapRGB(bmpSurface->format, 255, 255, 255));

SDL_Texture* bmpTexture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
if (bmpTexture == nullptr) {
	std::cerr << "纹理创建失败: " << SDL_GetError() << std::endl;
	SDL_FreeSurface(bmpSurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 1;
}

SDL_FreeSurface(bmpSurface);
SDL_Event e;
bool quit = false;

while (!quit) {
	// 处理事件
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}

	}
	// 清除渲染器
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	const char* background = "E:\\GamePicture\\image\\background.bmp";
	auto texture = IMG_LoadTexture(renderer, background);
	//		//texture = SDL_CreateTextureFromSurface(renderer, image);
	//
	SDL_RenderCopy(renderer, texture, NULL, NULL);


	// 将图像绘制到背景上
	SDL_Rect dstRect = { 0, 100, bmpSurface->w, bmpSurface->h };
	SDL_RenderCopy(renderer, bmpTexture, nullptr, &dstRect);

	// 更新渲染器
	SDL_RenderPresent(renderer);

	SDL_Delay(350);
}

// 清理资源
SDL_DestroyTexture(bmpTexture);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);

// 退出SDL子系统
IMG_Quit();
SDL_Quit();







//// 初始化SDL
//if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//	std::cerr << "SDL初始化失败: " << SDL_GetError() << std::endl;
//	return 1;
//}

//// 创建窗口
//SDL_Window* window = SDL_CreateWindow("合并图像示例", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
//if (!window) {
//	std::cerr << "创建窗口失败: " << SDL_GetError() << std::endl;
//	SDL_Quit();
//	return 1;
//}

//// 创建渲染器
//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//if (!renderer) {
//	std::cerr << "创建渲染器失败: " << SDL_GetError() << std::endl;
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//	return 1;
//}

//// 加载图像
//SDL_Surface* image1 = IMG_Load("E:\\GamePicture\\image\\background.bmp");
//SDL_Surface* image2 = IMG_Load("E:\\GamePicture\\image\\car.bmp");
//if (!image1 || !image2) {
//	std::cerr << "加载图像失败: " << IMG_GetError() << std::endl;
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//	return 1;
//}

//// 创建一个足够大的空白图像
//SDL_Surface* combinedImage = SDL_CreateRGBSurface(0, image1->w , std::max(image1->h, image2->h), 32, 0, 0, 0, 0);

//// 将重复的图像复制到空白图像中
//SDL_Rect dstRect;
//dstRect.x = 0;
//dstRect.y = 0;
//SDL_BlitSurface(image1, NULL, combinedImage, &dstRect);
//dstRect.x = 0;
//SDL_BlitSurface(image2, NULL, combinedImage, &dstRect);
//dstRect.x = 200;
//SDL_BlitSurface(image2, NULL, combinedImage, &dstRect);

//// 使用新的合并图像替代原来的多个图像
//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, combinedImage);

//// 渲染合并后的图像
//SDL_RenderClear(renderer);
//SDL_RenderCopy(renderer, texture, NULL, NULL);
//SDL_RenderPresent(renderer);

//// 主循环
//SDL_Event event;
//bool quit = false;
//while (!quit) {
//	while (SDL_PollEvent(&event)) {
//		if (event.type == SDL_QUIT) {
//			quit = true;
//		}
//	}
//}

//// 清理资源
//SDL_FreeSurface(image1);
//SDL_FreeSurface(image2);
//SDL_FreeSurface(combinedImage);
//SDL_DestroyTexture(texture);
//SDL_DestroyRenderer(renderer);
//SDL_DestroyWindow(window);
//SDL_Quit();

#endif // 0


{
	TianHui::Game g{};
	g.start();
}

std::cin.get();
return 0;


}


#if 0

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Mouse Movement Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* image_surface = IMG_Load("E:\\GamePicture\\image\\Card\\cardPlant\\peaShooter.bmp");
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface);
	if (!image_texture) {
		std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Rect image_rect;
	SDL_QueryTexture(image_texture, NULL, NULL, &image_rect.w, &image_rect.h);
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEMOTION) {
				image_rect.x = e.motion.x - image_rect.w / 2;
				image_rect.y = e.motion.y - image_rect.h / 2;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, image_texture, NULL, &image_rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(image_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}


# endif




/*
#include<iostream>
#include <SDL.h>
#include <SDL_ttf.h>
*/
//int main(int argc, char* argv[]) {
//	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//		return 1;
//	}
//
//	if (TTF_Init() < 0) {
//		printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
//		return 1;
//	}
//
//
//	SDL_Window* window = SDL_CreateWindow("Mouse Movement Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
//	if (!window) {
//		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
//		return 1;
//	}
//
//	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//	if (!renderer) {
//		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
//		return 1;
//	}
//
//
//
//	TTF_Font* font = TTF_OpenFont("D:\\include\\fonts\\GameFont.ttf", 250);
//	if (font == nullptr) {
//		printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
//		return 1;
//	}
//
//	SDL_Color textColor = { 255, 255, 255 }; // 设定文字颜色为白色
//
//	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello, World!", textColor);
//
//	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//
//	int textWidth = textSurface->w;
//	int textHeight = textSurface->h;
//
//	SDL_FreeSurface(textSurface);
//
//	SDL_Rect textRect{};
//
//	textRect.x = 100;
//	textRect.y = 200;
//	textRect.w = textWidth;
//	textRect.h = textHeight;
//
//	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
//	TTF_CloseFont(font);
//	SDL_DestroyTexture(textTexture);
//
//	std::cin.get();
//
//	return 0;
//}

#if 0
int main(int argc, char* argv[]) {
	// 初始化SDL2
	SDL_Init(SDL_INIT_VIDEO);

	// 初始化TTF
	TTF_Init();

	// 加载字体
	TTF_Font* font = TTF_OpenFont("D:\\include\\fonts\\simsun.ttf", 25);

	// 创建窗口
	SDL_Window* window = SDL_CreateWindow("Text Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	// 创建渲染器
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// 渲染文本为纹理
	/*SDL_Color color = { 255, 255, 255 };
	const char* tet = "你好，世界";
	SDL_Surface* surface = TTF_RenderUTF8_Solid(font, tet, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);*/

	const char* tet = "你好，世界";
	SDL_Color textColor = { 255, 255, 255 }; // 设定文字颜色为白色
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, tet, textColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;
	SDL_FreeSurface(textSurface);

	// 渲染纹理
	SDL_Rect dest = { 0, 0, 150, 100 };

	SDL_RenderCopy(renderer, texture, nullptr, &dest);
	SDL_RenderPresent(renderer);

	// 等待用户退出
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	// 清理资源
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}

#endif