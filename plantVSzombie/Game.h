#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include"noCopy.h"
#include"inPut.h"
#include"threadPool.h"
#include"simpleZombie.h"
#include"attackPlant.h"
#include"peaShooter.h"
#include"sunFlower.h"
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
			plants_{},
			succeed_{-2},
			chanziTexture_{},
			carTexture_{},
			card_{},
			sunFlag_{}
		{
			if (!isInit()) {
				Init();
				pool_.start(6);

				//初始化卡片
				const char* path = "E:\\GamePicture\\image\\Card\\cardPlant\\peaShooter.bmp";
				card_[1] = setTouMing(path);

				//初始化铲子
				chanziTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\chanzi.bmp");


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
					SDL_DestroyTexture(chanziTexture_);
					SDL_DestroyTexture(carTexture_);

					window_ = nullptr;
					render_ = nullptr;

					IMG_Quit();
					SDL_Quit();

					init_ = false;
				}
				


			}


			for (auto& i : card_)
			{				
				const auto &text = i.second;
				if (text)
					SDL_DestroyTexture(text);
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

				int size = zombie.second.size() - 1;

				succeed_ = 0;

				for (int i = size; i >= 0; --i)
				{
					succeed_ = 1;
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


			for (int i = 1; i <= size; ++i)
			{
				//int random = rand() % 650;
				int index_ = rand() % 5 + 1;

				int y = 1310 - index_ * 50 + i * 120;
				zombies_[index_].emplace_back(new simpleZombie(y, findZombieY(index_)));

			}

	



		}

		void drawSelectCard(int cardID,const Position&pos)
		{		

			SDL_Rect rect{ pos.x_,pos.y_,0,0 };
			
			SDL_QueryTexture(card_[cardID], nullptr, nullptr, &rect.w, &rect.h);

			rect.x = pos.x_;
			rect.y = pos.y_;
			SDL_RenderCopy(render_, card_[cardID], nullptr, &rect);

		}

		void drawChanzi(const Position& pos)
		{
			SDL_Rect getRect{};
			SDL_QueryTexture(chanziTexture_, nullptr, nullptr, &getRect.w, &getRect.h);

			getRect.x = pos.x_;
			getRect.y = pos.y_;


			SDL_RenderCopy(render_, chanziTexture_, nullptr, &getRect);


		}

		//判断是否选中了铲子
		bool getChanzi(int x1, int y1)
		{
			if (x1 >= 1335 && x1 <= 1381)
				if (y1 >= 12 && y1 <= 67)
					return true;

			return false;
		}

		//判断是否点中了太阳
		bool isSeclctSun(int x1,int y1)
		{
			auto p = findGrid(x1, y1);

			if (x1 >= p.x_ && x1 <= p.x_ + 61)
				if (y1 >= p.y_ + 56 && y1 <= p.y_ + 124)
					return true;

			return false;			

		}
		


		void start()
		{
			
			//创建僵尸
			creatZombie(5);

			//记录鼠标左键点击次数
			int leftCount{};
			//记录铲子鼠标点击次数
			int chanLeftCount{};

			//卡槽里第几个卡片
			int cardID = { -1 };

			//铲子是否铲除植物
			bool chanFlag = false;

			plants_[noCopy::findPlntRode(109)].emplace_back(new sunFlower(findGrid(344, 109)));


			//int e_x{}, e_y{};// 判断鼠标是否移动
			//bool e_move = false;
			while (!isQuit())
			{
				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				compute();
				upDate();
				draw();

				

				// 胜利了
				if (succeed_ == 0)
				{
					//end();quit=true;
					//quit_ = true;
				}
				else if(succeed_ == -1)//输了
				{

				}

				auto res = pool_.submit([this]() {this->clearDie(); });
				res.get();

				//e_move = false;

				//获取鼠标位置
				int x{}, y{};
				auto button = SDL_GetMouseState(&x, &y);

				//找到植物种植点
				auto temp = findGrid(x, y);


				//判断当前位置是否有植物，并设置标志
				bool flagPlantplants = true;
				for (auto& plant : plants_)
				{
					for (auto& i : plant.second)
						if (temp == i->getPos() && !i->isDie())
							flagPlantplants = false;
				}

				
				//画选择的植物
				if (cardID != -1)
				{

					auto res = pool_.submit([this, cardID](const Position& temp) { drawSelectCard(cardID, temp); }, Position(x-30, y-20));
					res.get();					

				}


				//如果第一下点中铲子
				if (chanFlag)
				{
					drawChanzi(Position(x - 40, y - 40));
					// 铲子移动动画
					//quit_ = true;

				}

				// 获取按键事件
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{
					if (e->type == SDL_QUIT)
					{
						quit_ = true;
					}

					//判断鼠标是否移动
					
					/*if (x != e_x || y != e_y)
					{
						e_move = true;
					}
					e_x = x;
					e_y = y;*/

					
					
					
					switch (e->type)
					{
					case SDL_MOUSEMOTION:
					if(true)
					{
						//SDL_RenderClear(render_);
						// 其他渲染代码
						//Position(e->motion.x, e->motion.y));
						//SDL_RenderPresent(render_);
						
						
					}
						break;
					case SDL_MOUSEBUTTONDOWN:

						if (e->button.button == SDL_BUTTON_LEFT)
						{
							++leftCount;	
							++chanLeftCount;

							sunFlag_ = isSeclctSun(x, y);

							

							if (chanLeftCount % 2 == 1)
							{
								if (chanLeftCount >= 1000)
									chanLeftCount = 0;


								
								chanFlag = getChanzi(x, y);

							}
							else
							{

								

								if (!getChanzi(x, y) && chanFlag)
								{
									chanFlag = false;

									//找到铲子要铲除的植物的坐标
									auto pos = findGrid(x, y);

									bool del = false;
									for (auto& i : plants_)
									{
										if (del)
											break;

										auto& plant = i.second;
										for (int j = plant.size() - 1; j >= 0; --j)
										{
											if (plant[j]->getPos() == pos)
											{
												plant.erase(plant.begin() + j);
												del = true;
												break;
											}
										}

									}



							}

							



						}



							//第二次点击鼠标，种植植物
							if (leftCount % 2 == 0)
							{
								if (leftCount >= 1000)
									leftCount = 0;

								//auto temp = findGrid(x, y);
																
								if (flagPlantplants && cardID == 1 && getCardID(x, y) != 1)
								{
									auto& res = plants_[noCopy::findPlntRode(temp.y_)].emplace_back(new peaShooter(temp));
									res.get();
								}





								
							
								//判断点击植物卡片
								cardID = -1;



							}
							else//第一次点击鼠标
							{


								if (cardID == 1)
									cardID = -1;

								//if (!getChanzi(x, y))
								cardID = getCardID(x, y);

								if (cardID == -1)
									++leftCount;

								

								
								
									


								
							}





						}

						break;

					case SDL_MOUSEBUTTONUP:

						if (e->button.button == SDL_BUTTON_LEFT) {

							

						}
						break;


					}

				}









				SDL_Delay(40);
				SDL_RenderPresent(render_);


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

		SDL_Texture* chanziTexture_;

		SDL_Texture* carTexture_;

		bool sunFlag_;

		//存放要挑选的卡片
		std::map<int, SDL_Texture*>card_;

		// 胜利判定
		int succeed_;


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

		SDL_Texture* setTouMing(const char* file)
		{
			auto peopleSurface = IMG_Load(file);
			if (!peopleSurface)
			{
				printf("Error open picture!\n\r");
				return nullptr;
			}

			SDL_SetColorKey(peopleSurface, SDL_TRUE, SDL_MapRGB(peopleSurface->format, 255, 255, 255));

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
			/*texture = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\chanzi.bmp", getRect);
			getRect.x = 1400 - getRect.w;
			getRect.y = 0;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);*/

			drawChanzi(Position(1320, 0));




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

			for (int j = 1; j <= plants_.size(); ++j)
				for (auto& plant : plants_[j])
				{
					std::sort(zombies_[j].begin(), zombies_[j].end(),
						[](std::shared_ptr<Zombie>& a, std::shared_ptr<Zombie>& b) {return a->getPos().x_ < b->getPos().x_; });

					if (zombies_[j].size() == 0)
					{	
						auto restemp = (pool_.submit([&]() {return plant->playImage(render_, false,sunFlag_); }));
						restemp.get();
						
					}

					for (auto& zombie : zombies_[j])						
						if (plant->getPos().y_ == noCopy::findY(zombie->getPos().y_)) {

							if (!plant->isDie()) {
								auto restemp = (pool_.submit([&]() {return plant->draw(render_, zombie->getPos(), zombie->isDie(),sunFlag_); }));
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
					if (!i->isDie())
					{

						//僵尸吃掉了你的脑子
						if (i->getPos().x_ <= 180)
						{
							succeed_ = -1;
							//quit_ = true;
						}

						auto restemp = (pool_.submit([i, j, this]() {return i->attack(render_, plants_[j]); }));
						restemp.get();
					}

			}




		
			

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

