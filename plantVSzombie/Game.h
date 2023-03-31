#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include"noCopy.h"
#include"inPut.h"
#include"threadPool.h"
#include"simpleZombie.h"
#include"Plant.h"
#include"peaShooter.h"
#include<algorithm>
#include<map>

namespace TianHui
{

	class Game final : noCopy
	{
	public:
		Game() :
			window_(nullptr),
			render_(nullptr),
			quit_(false),
			init_(false),
			pool_{},
			zombies_{},
			plants_{}
		{
			if (!isInit()) {
				Init();
				pool_.start(6);
			}

			
		}

		~Game()
		{
			if (isQuit())
			{
				if (isInit())
				{
					SDL_DestroyWindow(window_);
					SDL_DestroyRenderer(render_);
					window_ = nullptr;
					render_ = nullptr;

					IMG_Quit();
					SDL_Quit();

					init_ = false;
				}

			}




		}

		// 根据鼠标点击位置找到植物应该种植的位置。
		static Position findGrid(int x1, int y1)
		{
			// row:5  col:9
			// 
			//y:pea 60--710
			//120
			//180--310  310--440=130
			// 
			// x:pea 310--1264
			// 350
			// 310--416=106



			const int Xtemp = 343, Xjian = 107, middleX = 365, middleY = 125;

			int x{ middleX }, y{ middleY };
			if (x1 >= Xtemp && x1 <= Xtemp + 9 * Xjian)
			{
				if (x1 >= Xtemp && x1 <= Xtemp + Xjian)
					x = middleX;
				else if (x1 > Xtemp + Xjian && x1 <= Xtemp + 2 * Xjian)
					x = middleX + 1 * Xjian;
				else if (x1 > Xtemp + 2 * Xjian && x1 <= Xtemp + 3 * Xjian)
					x = middleX + 2 * Xjian;
				else if (x1 > Xtemp + 3 * Xjian && x1 <= Xtemp + 4 * Xjian)
					x = middleX + 3 * Xjian;
				else if (x1 > Xtemp + 4 * Xjian && x1 <= Xtemp + 5 * Xjian)
					x = middleX + 4 * Xjian;
				else if (x1 > Xtemp + 5 * Xjian && x1 <= Xtemp + 6 * Xjian)
					x = middleX + 5 * Xjian;
				else if (x1 > Xtemp + 6 * Xjian && x1 <= Xtemp + 7 * Xjian)
					x = middleX + 6 * Xjian;
				else if (x1 > Xtemp + 7 * Xjian && x1 <= Xtemp + 8 * Xjian)
					x = middleX + 7 * Xjian;
				else if (x1 > Xtemp + 8 * Xjian && x1 <= Xtemp + 9 * Xjian)
					x = middleX + 8 * Xjian;


			}

			const int Ytemp = 109, Yjian = 130;
			if (y1 >= Ytemp && y1 <= Ytemp + 5 * Yjian)
			{
				if (y1 >= Ytemp && y1 <= Ytemp + Yjian)
					y = middleY;
				else if (y1 > Ytemp + Yjian && y1 <= Ytemp + 2 * Yjian)
					y = middleY + 1 * Yjian;
				else if (y1 > Ytemp + 2 * Yjian && y1 <= Ytemp + 3 * Yjian)
					y = middleY + 2 * Yjian;
				else if (y1 > Ytemp + 3 * Yjian && y1 <= Ytemp + 4 * Yjian)
					y = middleY + 3 * Yjian;
				else if (y1 > Ytemp + 4 * Yjian && y1 <= Ytemp + 5 * Yjian)
					y = middleY + 4 * Yjian;

			}

			return Position(x, y);
		}

		//输入僵尸在第几个格子，返回僵尸的Y坐标。
		static int findZombieY(int i)
		{
			int index_ = 50 + (i - 1) * 130;
			return noCopy::findY(index_-1) - 70;
		}


		void clearDie()
		{
			// 清理死亡植物
			for (auto& plant : plants_)
			{
				auto begin = plant.second.begin();
				for (int i = plant.second.size() - 1; i >= 0; --i)
				{
					if (plant.second[i]->isDie())
					{
						int temp = i;
						--i;
						plant.second.erase(begin + temp);
					}

				}

			}


			//清理死亡僵尸
			for (auto& zombie : zombies_)
			{
				auto begin = zombie.second.begin();
				for (int i = zombie.second.size() - 1; i >= 0; --i)
				{
					if (zombie.second[i]->isDie())
					{
						int temp = i;
						--i;
						zombie.second.erase(begin + temp);
					}

				}

			}

		}


		void creatZombie(int size)
		{

			pool_.submit([&]() {
				for (int i = 0; i < size; ++i)
				{
					//int random = rand() % 650;
					int index_ = rand() % 5 + 1;

					int y = 1310 - index_ * 50 + i*120 ;
					zombies_[index_].emplace_back(new simpleZombie(y, findZombieY(index_)));

				}

				});

		}

		void drawSelectCard(int cardID,const Position&pos)
		{
			if (cardID != 1)
				return;

			const char* path = "E:\\GamePicture\\image\\Card\\cardPlant\\peaShooter.bmp";
			SDL_Rect rect{ pos.x_,pos.y_,0,0 };

			auto texture = setTouMing(path, rect);

			SDL_RenderCopy(render_, texture, nullptr, &rect);

			SDL_DestroyTexture(texture);

		}


		void start()
		{
			
			//创建僵尸
			creatZombie(5);

			//记录鼠标左键点击次数
			int leftCount{};

			//plants_[noCopy::findPlntRode(109)].emplace_back(new peaShooter(findGrid(344, 109)));


			int e_x{}, e_y{};// 判断鼠标是否移动
			bool e_move = false;
			while (!isQuit())
			{

				compute();
				upDate();
				draw();



				auto res = pool_.submit([this]() {this->clearDie(); });
				res.get();

				e_move = false;

				// 获取按键事件
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{
					if (e->type == SDL_QUIT)
					{
						quit_ = true;
					}

					//判断鼠标是否移动
					int x{}, y{};
					auto button = SDL_GetMouseState(&x, &y);
					if (x != e_x || y != e_y)
					{
						e_move = true;
					}
					e_x = x;
					e_y = y;

					

					


				

					
					switch (e->type)
					{
					case SDL_MOUSEMOTION:
						
					{
						auto temp = findGrid(x, y);

						int cardID = getCardID(x, y);
						auto res = pool_.submit([this, cardID, temp]() { drawSelectCard(cardID, temp); });
						res.get();

						break;
					}

					case SDL_MOUSEBUTTONDOWN:

						if (e->button.button == SDL_BUTTON_LEFT)
						{
							++leftCount;


							if (leftCount % 2 == 1)
							{
								


								



							}


						}

						break;

					case SDL_MOUSEBUTTONUP:

						if (e->button.button == SDL_BUTTON_LEFT) {

							//种植植物
							if (leftCount % 2 == 0)
							{
								if(leftCount>=1000)
									leftCount = 0;

								auto temp = findGrid(x, y);
								bool flagPlantplants = true;

								//判断当前位置是否有植物，并设置标志
								for (auto& plant : plants_)
								{
									for (auto& i : plant.second)
										if (temp == i->getPos() && !i->isDie())
											flagPlantplants = false;
								}


								if (flagPlantplants)
								{
									plants_[noCopy::findPlntRode(temp.y_)].emplace_back(new peaShooter(temp));

								}


							}

						}
						break;


					}

				}











			}

		}

	private:
		SDL_Window* window_;

		SDL_Renderer* render_;

		threadpool pool_;

		std::map<int, std::vector<std::shared_ptr<Zombie>>>zombies_;

		std::map<int, std::vector<std::shared_ptr<Plant>>>plants_;

		bool quit_;

		bool init_;




	private:

		SDL_Texture* setTouMing(const char* file, SDL_Rect& rect)
		{
			auto peopleSurface = IMG_Load(file);
			if (!peopleSurface)
			{
				printf("Error open picture!\n\r");
				return nullptr;
			}

			SDL_SetColorKey(peopleSurface, SDL_TRUE, SDL_MapRGB(peopleSurface->format, 255, 255, 255));

			rect = peopleSurface->clip_rect;

			auto temp = SDL_CreateTextureFromSurface(render_, peopleSurface);
			if (!temp)
			{
				printf("Error Create Texture!\n\r");
				return nullptr;
			}

			SDL_FreeSurface(peopleSurface);

			return temp;
		}

		inline bool isQuit()
		{
			return quit_;
		}

		inline bool isInit()
		{
			return init_;
		}

		void Init()
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
				return;
			}

			int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
				return;
			}

			window_ = SDL_CreateWindow("TianHui", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowsWide, windowsHeight, SDL_WINDOW_SHOWN);
			if (!window_)
			{
				std::cerr << "Error Creat Windows" << std::endl;
				return;
			}

			render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!render_)
			{
				std::cerr << "Error Creat Renderer" << std::endl;
				return;
			}



			SDL_Surface* icon = IMG_Load("E:\\GamePicture\\image\\PlantsVsZombies.ico");

			SDL_SetWindowIcon(window_, icon);

			SDL_FreeSurface(icon);


			init_ = true;
		}

		//找到卡槽里卡片的id
		int getCardID(const int x1,const int y1)
		{
			
			const int Xtemp = 410, Xjian = 50;
			int x{ -1 };

			if (y1 >= 4 && y1 <= 72)
			{

				if (x1 >= Xtemp && x1 <= Xtemp + 13 * Xjian)
				{
					if (x1 >= Xtemp && x1 <= Xtemp + Xjian)
						x = 0;
					else if (x1 > Xtemp + Xjian && x1 <= Xtemp + 2 * Xjian)
						x = 1;
					else if (x1 > Xtemp + 2 * Xjian && x1 <= Xtemp + 3 * Xjian)
						x = 2;
					else if (x1 > Xtemp + 3 * Xjian && x1 <= Xtemp + 4 * Xjian)
						x = 3;
					else if (x1 > Xtemp + 4 * Xjian && x1 <= Xtemp + 5 * Xjian)
						x = 4;
					else if (x1 > Xtemp + 5 * Xjian && x1 <= Xtemp + 6 * Xjian)
						x = 5;
					else if (x1 > Xtemp + 6 * Xjian && x1 <= Xtemp + 7 * Xjian)
						x = 6;
					else if (x1 > Xtemp + 7 * Xjian && x1 <= Xtemp + 8 * Xjian)
						x = 7;
					else if (x1 > Xtemp + 8 * Xjian && x1 <= Xtemp + 9 * Xjian)
						x = 8;
					else if (x1 > Xtemp + 9 * Xjian && x1 <= Xtemp + 10 * Xjian)
						x = 9;
					else if (x1 > Xtemp + 10 * Xjian && x1 <= Xtemp + 11 * Xjian)
						x = 10;
					else if (x1 > Xtemp + 11 * Xjian && x1 <= Xtemp + 12 * Xjian)
						x = 11;
					else if (x1 > Xtemp + 12 * Xjian && x1 <= Xtemp + 13 * Xjian)
						x = 12;


				}

			}
			
			return x;
		}

		void drawCard(const char* cardPath, const int index_)
		{

			SDL_Rect getRect{};
			//卡框50*70
			auto texture = setTouMing("E:\\GamePicture\\image\\Card\\cardPicture\\cardBackground.bmp", getRect);

			getRect.x = 330 + 75 + 50 * (index_);
			getRect.y = 5;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);

			// 卡片40*40
			texture = setTouMing(cardPath, getRect);

			getRect.x = 330 + 80 + 50 * (index_);
			getRect.y = 15;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);


		}

		void backGround()
		{

			SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
			SDL_RenderClear(render_);

			const char* background = "E:\\GamePicture\\image\\background.bmp";
			auto texture = IMG_LoadTexture(render_, background);
			int imageW{}, imageH{};
			SDL_QueryTexture(texture, nullptr, nullptr, &imageW, &imageH);

			if (!texture)
			{
				std::cerr << "Error background" << std::endl;
				return;
			}


			SDL_Rect windowRect{};
			if (true)
			{
				windowRect.w = 3 * imageW / 4;
				windowRect.h = imageH;


			}
			else
			{
				windowRect.x = 1 * imageW / 4;
				windowRect.w = 3 * imageW / 4;
				windowRect.h = imageH;


			}


			SDL_RenderCopy(render_, texture, &windowRect, nullptr);
			//SDL_RenderPresent(render_);

			SDL_DestroyTexture(texture);


			const char* carPath = "E:\\GamePicture\\image\\car.bmp";

			SDL_Rect getRect{};
			texture = setTouMing(carPath, getRect);

			const int X = 270, Y = 90;
			int W = getRect.w, H = getRect.h;

			SDL_Rect rect[5] = { {X,Y + 50,W,H},{X,Y + 170,W,H},{X,Y + 300,W,H},{X - 4,Y + 420,W,H},{X - 12,Y + 560,W,H} };

			for (int i = 0; i < 5; ++i)
			{
				SDL_RenderCopy(render_, texture, NULL, &rect[i]);
			}

			SDL_DestroyTexture(texture);

			// 卡槽
			texture = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\long.bmp", getRect);

			getRect.x = 330;
			getRect.y = 0;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);





			//

			//铲子
			texture = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\chanzi.bmp", getRect);
			getRect.x = 1400 - getRect.w;
			getRect.y = 0;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);





		}


		void upDate()
		{





		}


		void draw()
		{
			


			// 绘制背景
			auto resBackGround = pool_.submit([this]() {this->backGround(); });


			// 绘制卡片
			auto resCard = pool_.submit([this]() {this->drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\peaShooter.bmp", 1); });
			auto resCard1 = pool_.submit([this]() {this->drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\sunFlower.bmp", 0); });


			resBackGround.get();

			resCard.get();
			resCard1.get();



			//std::vector < std::future<int>>resPlant{};

			for (int j = 0; j < plants_.size(); ++j)
				for (auto& plant : plants_[j])
				{
					std::sort(zombies_[j].begin(), zombies_[j].end(),
						[](std::shared_ptr<Zombie>& a, std::shared_ptr<Zombie>& b) {return a->getPos().x_ < b->getPos().x_; });

					if (zombies_[j].size() == 0)
					{	
						auto restemp = (pool_.submit([&]() {return plant->playImage(render_, false); }));
						restemp.get();
						
					}

					for (auto& zombie : zombies_[j])						
						if (plant->getPos().y_ == noCopy::findY(zombie->getPos().y_)) {

							if (!plant->isDie()) {
								auto restemp = (pool_.submit([&]() {return plant->attack(render_, zombie->getPos(), zombie->isDie()); }));
								int fire = restemp.get();
								if (fire != -1&& !plant->isDie()&&plant->getPos().x_<=zombie->getPos().x_+40)
									zombie->setHP(fire);

								break;
							}

							
						}										


				}

			//std::vector< std::future<int>>resZombie{};
			for (int j = 0; j < zombies_.size(); ++j)
			{
				

				for (auto& i : zombies_[j])
					if (!i->isDie()) {

						auto restemp = (pool_.submit([i, j, this]() {return i->attack(render_, plants_[j]); }));
						restemp.get();
					}

			}





		
			SDL_Delay(50);
			SDL_RenderPresent(render_);

		}

		void compute()
		{

		}

		// x1 attack x2
		bool touch(const Position& plant, const Position& zombie)
		{
			if (plant.x_ >= zombie.x_)
				return true;

			return false;
		}

	};




}

