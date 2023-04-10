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
#include"Music.h"
#include"Cactus.h"
#include"car.h"
#include<algorithm>
#include<map>
#include<vector>

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
			succeed_{ -2 },
			chanziTexture_{},
			backTexture_{},
			toolTexture_{},
			card_{},
			music_{},
			tempMusic_{},
			readyArrTexture_{},
			chanKuangTexture_{},
			plantTableTexture_{},
			arrSelectID_{},
			arrPath_{},
			carArr_{}
		{
			if (!isInit()) {
				Init();
				pool_.start(6);

				// 隐藏鼠标
				SDL_ShowCursor(false);


				const int X = 270, Y = 90;
				Position posArr[5] = { {X,Y + 50},{X,Y + 170},{X,Y + 300},{X - 4,Y + 420},{X - 12,Y + 560} };
				for (int i = 1; i <= 5; ++i)
				{
					// 僵尸、植物初始化
					plants_[i];
					zombies_[i];

					// 创建小车
					carArr_[i] = std::shared_ptr<car>{ new car{posArr[i - 1]} };
					carArr_[i]->setPath(render_,"E:\\GamePicture\\image\\car.bmp");

				}


				//创建僵尸
				creatZombie(10);


			}


		}
		
		// 释放资源操作
		~Game()
		{
			// 程序退出
			if (isQuit())
			{

				if (isInit())
				{
					// 释放资源并关闭窗口
					if(window_)
						SDL_DestroyWindow(window_);

					// 释放渲染器
					if(render_)
						SDL_DestroyRenderer(render_);

					// 释放铲子资源
					if(chanziTexture_)
						SDL_DestroyTexture(chanziTexture_);

					// 释放铲框
					if (chanKuangTexture_)
						SDL_DestroyTexture(chanKuangTexture_);

					// 释放植物选择表
					if (plantTableTexture_)
						SDL_DestroyTexture(plantTableTexture_);

					//// 释放小车
					//if(carTexture_)
					//	SDL_DestroyTexture(carTexture_);

					// 释放背景资源
					if(backTexture_)
						SDL_DestroyTexture(backTexture_);

					// 释放背景资源
					if(toolTexture_)
						SDL_DestroyTexture(toolTexture_);

					// 释放鼠标资源
					if(mouseTexture_)
						SDL_DestroyTexture(mouseTexture_);

					window_ = nullptr;
					render_ = nullptr;

					// 关闭IMG库
					IMG_Quit();

					// 关闭SDL库
					SDL_Quit();



					init_ = false;

				}



			}

			// 释放准备阶段的资源
			for (auto& i : readyArrTexture_)
			{
				if (i)
					SDL_DestroyTexture(i);
			}

			// 释放卡槽里卡片的资源
			for (auto& i : card_)
			{
				const auto& text = i.second;
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
			return noCopy::findY(index_ - 1) - 70;
		}

		// 清理死亡的植物
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

				// 赢
				succeed_ = 0;

				for (int i = size; i >= 0; --i)
				{
					// 输
					succeed_ = 1;
					if (zombie.second[i]->isDie() && zombie.second[i]->getDiePlay())
					{
						int temp = i;
						--i;
						zombie.second.erase(begin + temp);
					}

				}


			}



		}

		// 向僵尸列表里添加僵尸
		void creatZombie(int size)
		{


			for (int i = 1; i <= size; ++i)
			{
				//int random = rand() % 650;
				int index_ = rand() % 5 +1;

				int y = 1310 - index_ * 50 + i * 120;
				zombies_[index_].emplace_back(new simpleZombie(y, findZombieY(index_)));

			}





		}

		// 渲染出选择的卡片
		void drawSelectCard(int cardID, const Position& pos)
		{

			allDraw(card_[cardID], pos);

		}

		// 渲染出铲子
		void drawChanzi(const Position& pos,bool isDraw)
		{
			if (!isDraw)
				return;

			allDraw(chanziTexture_, pos);

		}

		// 渲染出铲框
		void drawChanKuang(const Position& pos)
		{
			allDraw(chanKuangTexture_, pos);
		}

		//// 渲染出小车
		//void drawCar(const Position&pos)
		//{

		//	allDraw(carTexture_, pos);

		//}

		// 渲染出背景图片
		void drawBackground()
		{

			int imageW{}, imageH{};
			SDL_QueryTexture(backTexture_, nullptr, nullptr, &imageW, &imageH);


			SDL_Rect windowRect{};

			windowRect.w = 3 * imageW / 4;
			windowRect.h = imageH;

			SDL_RenderCopy(render_, backTexture_, &windowRect, nullptr);

		}

		// 渲染出卡槽
		void drawTool()
		{

			allDraw(toolTexture_, Position(330, 0));

		}

		// 渲染出选择区
		void drawSelectTable()
		{
			allDraw(plantTableTexture_, Position( 330, 100));
		}

		// 判断是否选中了铲子
		bool getChanzi(int x1, int y1)
		{
			if (x1 >= 1335 && x1 <= 1381)
				if (y1 >= 12 && y1 <= 67)
					return true;

			return false;
		}

		// 判断是否点中了太阳
		bool isSeclctSun(int x1, int y1)
		{
			auto p = findGrid(x1, y1);

			if (x1 >= p.x_ && x1 <= p.x_ + 61)
				if (y1 >= p.y_ + 56 && y1 <= p.y_ + 124)

					return true;

			return false;

		}

		// 根据位置绘制鼠标图案
		void drawMouse(const Position&e,bool isDraw)
		{
			if (!isDraw)
				return;

			allDraw(mouseTexture_, e);
		}

		// 判断是否点击到确认按钮
		bool buttonACK(const int &eX,const int &eY)
		{
			if (eX >= 654 && eX <= 705)
				if (eY >= 657 && eY <= 676)
					return true;

			return false;
		}

		//找到卡槽里卡片的id
		int getSelectID(const int x1, const int y1)
		{

			const int Xtemp = 354, Xjian = 73;
			int x{ -1 };

			// 对第一行进行判断
			if (y1 >= 141 && y1 <= 209)
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



				}

			}

			return x;
		}

		// 画选择区的植物
		void drawSelectID(const char *path,const int& index_)
		{

			int posX{ 265 }, posY{ 130 };
			SDL_Rect getRect{ posX,posY,0,0 };
			// 卡片40*40
			auto texture = setTouMing(path, getRect);

			getRect.x = posX + 80 + 73 * (index_);
			getRect.y = posY + 10;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);

		}

		// 添加植物路径
		void pushPath()
		{
			// 太阳花 0
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\sunFlower.bmp");

			// 豌豆射手 1
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\peaShooter.bmp");

			// 仙人掌 2
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\cactus.bmp");

		}

		// 开始前的挑选植物阶段
		void selectPlant()
		{
			///////////////////////////////////////
			// 设置卡片选择路径
			decltype(arrPath_) Path_{};
			std::vector<bool>flagArr{};

			// Flower
			Path_.push_back("E:\\GamePicture\\image\\SunFlower\\sunFlower\\Frame2.bmp");
			bool flagF = false;
			flagArr.push_back(false);

			// Pea
			Path_.push_back("E:\\GamePicture\\image\\Pea\\Peashooter\\Frame0.bmp");
			bool flagPea = false;
			flagArr.push_back(false);

			// cactus
			Path_.push_back("E:\\GamePicture\\image\\cactus\\Cactus\\Frame0.bmp");
			bool flagC = false;
			flagArr.push_back(false);
			
			//////////////////////////////////////			 			


			// 播放音乐
			music_.setMusic("E:\\GamePicture\\Music\\mainmusic.ogg");
			music_.playMusic();

			// 选择音效
			tempMusic_.setChunk("E:\\GamePicture\\Music\\seedlift.ogg");
			

			// 设置图形大小
			SDL_Rect rect{ 0,0,windowsWide,windowsHeight }, windowRect{ 0,0,windowsWide*3/4,windowsHeight };

			// 鼠标位置
			int eX{}, eY{};

			//选择的卡片
			int ID{ -1 };
			
			while (!isQuit())
			{
				// 获取鼠标位置
				SDL_GetMouseState(&eX, &eY);
				
				// 清理画布
				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				
				// 绘制移动背景
				int size = windowsWide / 4;
				if (windowRect.x <= size)
				{
					windowRect.x += 2;

					if (windowRect.x > size)
						windowRect.x = size;

				}
				SDL_RenderCopy(render_, backTexture_, &windowRect, nullptr);

				// 画面移动停止
				if (windowRect.x == size)
				{
					auto res = pool_.submit([&,this]()
						{
							// 画出卡槽
							drawTool();

							// 画出选择框 131 * 100
							drawSelectTable();




							///////////////////////////////////////////////////////////////

							if (ID != -1)
							{								

								bool flag{ false };
								for (auto& i : arrSelectID_)
								{
									if (i == ID)
										flag = true;
								}

								if(!flag)
									arrSelectID_.push_back(ID);

							}

							// 画出待选择的植物 280,145
							// Flower
							int FID = 0;
							if (ID == FID)
							{
								Path_[FID] = "E:\\GamePicture\\image\\Card\\cardPlant\\sunFlower.bmp";
								flagArr[FID] = true;
							}							
							drawSelectID(Path_[FID], FID);

							// Pea
							int PeaID = 1;
							if (ID == PeaID)
							{
								Path_[PeaID] = "E:\\GamePicture\\image\\Card\\cardPlant\\peaShooter.bmp";
								flagArr[PeaID] = true;
							}
							drawSelectID(Path_[PeaID], PeaID);

							// cactus
							int cactusID = 2;
							if (ID == cactusID)
							{
								Path_[cactusID] = "E:\\GamePicture\\image\\Card\\cardPlant\\cactus.bmp";
								flagArr[cactusID] = true;
							}
							drawSelectID(Path_[cactusID], cactusID);


							// 按顺序在卡槽里画出卡片
							for (int i = 0; i < arrSelectID_.size(); ++i)
							{
								if (flagArr[FID] && arrSelectID_[i] == FID)
									drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\sunFlower.bmp", i);

								else if (flagArr[PeaID] && arrSelectID_[i] == PeaID)
									drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\peaShooter.bmp", i);

								else if (flagArr[cactusID] && arrSelectID_[i] == cactusID)
									drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\cactus.bmp", i);

							}

							
							
							//drawCard(pathF, 0, 280, 145);

								

						});
					


					res.get();

				}
				
				

				// 获取按键事件
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{


					// 获取键盘按键
					switch (e->key.keysym.sym)
					{
					case SDLK_ESCAPE:
						return;
						break;

					default:
						break;
					}



					// 获取鼠标按键
					switch (e->type)
					{

					case SDL_QUIT:
						return;
						break;

					case SDL_MOUSEMOTION:


						break;


					case SDL_MOUSEBUTTONDOWN:
						if (e->button.button == SDL_BUTTON_LEFT)
						{
							ID = getSelectID(eX, eY);

							if (ID != -1)
							{
								// 播放音效
								tempMusic_.playOnce(3);

							}


						}
						else if (e->button.button == SDL_BUTTON_RIGHT)
						{


						}

						break;

					case SDL_MOUSEBUTTONUP:

						if (e->button.button == SDL_BUTTON_LEFT)
						{



						}
						else if (e->button.button == SDL_BUTTON_RIGHT)
						{


						}

						break;


					}





				}







				// 绘制鼠标
				drawMouse(Position(eX, eY), true);

				SDL_RenderPresent(render_);
				SDL_Delay(5);


			}



			// 释放植物选择表
			if (plantTableTexture_)
				SDL_DestroyTexture(plantTableTexture_);

			plantTableTexture_ = nullptr;


		}

		// 进入游戏准备界面
		void ready()
		{
		

			// 播放音乐
			music_.setMusic("E:\\GamePicture\\Music\\mainmusic.ogg");
			music_.playMusic();
			

			// 设置图形大小
			SDL_Rect rect{ windowsWide / 4,windowsHeight / 4,0,0 };
			SDL_Rect* rectPtr{ &rect };

			// 下标
			int index_{};

			// 鼠标位置
			int eX{}, eY{};

			while (!isQuit())
			{
				
				// 清理画布
				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				// 获取鼠标位置
				SDL_GetMouseState(&eX, &eY);
				
				
				SDL_QueryTexture(readyArrTexture_[index_], nullptr, nullptr, &rect.w, &rect.h);

				//SDL_Rect cai{ 0,0,rect.w,rect.h / 2 };
				int size = readyArrTexture_.size();


				if (index_ == size - 1)
					rectPtr = nullptr;

				if (!rectPtr || !time_.queryTime(5000))
					SDL_RenderCopy(render_, readyArrTexture_[index_], nullptr, rectPtr);
				else if (index_ < size - 1)
				{
					//readyArrTexture_[index_] = setTouMing("E:\\GamePicture\\image\\ready.bmp");
					++index_;										

				}

				// 获取按键事件
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{


					// 获取键盘按键
					switch (e->key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit_ = true;
						break;

					case SDLK_RETURN:
					{
						if(!rectPtr)
							return;
					}
					break;

					default:
						break;
					}




					// 获取鼠标按键
					switch (e->type)
					{
						
					case SDL_QUIT :
						quit_ = true;
						break;

					case SDL_MOUSEMOTION:						
						// 移动到"确认"按钮时
						if (buttonACK(eX, eY)&& !rectPtr)
						{
							// 显示鼠标
							//SDL_ShowCursor(true);

						}
						else
						{
							
							
						}

						
						break;


					case SDL_MOUSEBUTTONDOWN:
						if (e->button.button == SDL_BUTTON_LEFT)
						{

							// 点击到"确认"按钮
							if (buttonACK(eX, eY))
							{
								if (!rectPtr)
									return;
							}


							Position temp{ eX,eY };
							int a = 0;


						}
						else if (e->button.button == SDL_BUTTON_RIGHT)
						{


						}

						break;

					case SDL_MOUSEBUTTONUP:

						if (e->button.button == SDL_BUTTON_LEFT)
						{



						}
						else if (e->button.button == SDL_BUTTON_RIGHT)
						{


						}

						break;


					}


				}



				// 画鼠标
				auto ttmouse = pool_.submit([&]() {drawMouse(Position(eX - 5, eY - 1), true); });
				ttmouse.get();

				SDL_RenderPresent(render_);

				SDL_Delay(50);


			}


			for (auto& i : readyArrTexture_)
			{
				if (i)
					SDL_DestroyTexture(i);

				i = nullptr;
			}

			readyArrTexture_.clear();


		}

		// 开始游戏
		void start()
		{
			// 播放音乐
			music_.setMusic("E:\\GamePicture\\Music\\KitanaiSekai.ogg");
			music_.playMusic();

			//记录鼠标左键点击次数
			int leftCount{};
			//记录铲子鼠标点击次数
			int chanLeftCount{};

			//卡槽里第几个卡片
			int cardID = { -1 };

			//铲子是否铲除植物
			bool chanFlag = false;

			//plants_[noCopy::findPlntRode(109)].emplace_back(new sunFlower(findGrid(344, 109)));




			while (!isQuit())
			{
				music_.playMusic();

				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				compute();
				upDate();
				draw();


				/*for (int i = 0; i < 5; ++i)
				{
					drawCar(posArr[i]);
				}*/

				


				// 胜利了
				if (succeed_ == 0)
				{
					//end();quit=true;
					//quit_ = true;
				}
				else if (succeed_ == -1)//输了
				{

				}

				//clearDie();

				{
					auto res = pool_.submit([this]() {this->clearDie(); });
					res.get();
				}

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

					pool_.submit([this, cardID](const Position& temp) {
						if (cardID < arrSelectID_.size() && cardID != -1)
						{
							drawSelectCard(arrSelectID_[cardID], temp);
						}

						}, Position(x - 40, y - 40));
					

				}


				// 画铲子和铲框
				pool_.submit([this, chanFlag,x,y]() 
					{
						// 画出铲框
						drawChanKuang(Position(1320, 0));

						// 画出铲子
						drawChanzi(Position(1320 - 5, -10), !chanFlag);

						//如果第一下点中铲子
						if (chanFlag)
						{


							// 铲子移动动画
							drawChanzi(Position(x - 40, y - 40), true);


						}

					});


				


				
				
				bool sunFlag_ = false;


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
						if (true)
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
							tempMusic_.setChunk("E:\\GamePicture\\Music\\bleep.ogg");
							tempMusic_.playOnce();


							++leftCount;
							++chanLeftCount;

							sunFlag_ = isSeclctSun(x, y);

							/// <summary>
							/// 点中太阳
							/// </summary>
							for (auto& plant : plants_)
							{
								// 没有点中
								if (!sunFlag_)
									break;

								for (auto& i : plant.second)
								{
									
									/*
									auto p = findGrid(x1, y1);

									if (x1 >= p.x_ && x1 <= p.x_ + 61)
									if (y1 >= p.y_ + 56 && y1 <= p.y_ + 124)
									*/

									// 点中太阳找到生产这个太阳的花
									auto tmp = findGrid(x, y - 34);
									

									// 如果是sunFlower
									if (typeid(*i) == typeid(sunFlower))
									{
										auto temp = dynamic_cast<sunFlower*> (i.get());
										if (!temp)
										{
											throw std::runtime_error("不安全转换，转换失败");
										}

										// 找到了被点太阳的向日葵
										if (tmp == i->getPos()) 
										{
											if (music_.isChunkNULL())
												music_.setChunk("E:\\GamePicture\\Music\\points.ogg");

											if(temp->isSunLive())
												music_.playOnce(4);

											temp->setShowSun(!sunFlag_);

										}

									}


								}


							}



							if (chanLeftCount % 2 == 1)
							{
								if (chanLeftCount >= 1000)
									chanLeftCount = 0;



								chanFlag = getChanzi(x, y);

								if (!chanFlag)
									++chanLeftCount;

							}
							else
							{

								


								if (!getChanzi(x, y) && chanFlag)
								{
									chanFlag = false;

									tempMusic_.setChunk("E:\\GamePicture\\Music\\shovel.ogg");
									tempMusic_.playOnce(2);

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

								

								if (flagPlantplants && getCardID(x, y) != 1 && cardID < arrSelectID_.size())
								{

									tempMusic_.setChunk("E:\\GamePicture\\Music\\seedlift.ogg");
									tempMusic_.playOnce(2);

									if (cardID != -1)
									{
										// 判断点击的是那一株植物
										switch (arrSelectID_[cardID])
										{
										case 0:
										{
											plants_[noCopy::findPlntRode(temp.y_)].emplace_back(new sunFlower(temp));
										}
										break;

										case 1:
										{
											plants_[noCopy::findPlntRode(temp.y_)].emplace_back(new peaShooter(temp));
										}
										break;

										case 2:
										{
											plants_[noCopy::findPlntRode(temp.y_)].emplace_back(new cactus(temp));
										}
										break;

										}
									}
									
								
								
									
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

								if (cardID == -1 || cardID >= arrSelectID_.size())
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





				

				// 画鼠标
				auto ttmouse = pool_.submit([&]() {drawMouse(Position(x - 5, y - 1), true); });
				ttmouse.get();

				
				SDL_RenderPresent(render_);

				SDL_Delay(50);

			}

		}


	private:

		// 通用的在指定位置绘制纹理
		void allDraw(SDL_Texture* texture, const Position& pos)
		{
			SDL_Rect getRect{};
			SDL_QueryTexture(texture, nullptr, nullptr, &getRect.w, &getRect.h);

			getRect.x = pos.x_;
			getRect.y = pos.y_;


			SDL_RenderCopy(render_, texture, nullptr, &getRect);

		}



		// 将图片背景边框设置成透明
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
		
		// 重载函数
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

		// 判断是否退出
		inline bool isQuit()
		{
			return quit_;
		}

		// 判断是否初始化
		inline bool isInit()
		{
			return init_;
		}

		// 初始化待选择的卡片
		void initCard()
		{

			const char* path[] = { "E:\\GamePicture\\image\\Card\\cardPlant\\sunFlower.bmp",
				"E:\\GamePicture\\image\\Card\\cardPlant\\peaShooter.bmp", "E:\\GamePicture\\image\\Card\\cardPlant\\cactus.bmp" };

			card_[0] = setTouMing(path[0]);
			if (!card_[0])
			{
				throw std::runtime_error("Error cardTexture");
			}

			card_[1] = setTouMing(path[1]);
			if (!card_[1])
			{
				throw std::runtime_error("Error cardTexture");
			}

			card_[2] = setTouMing(path[2]);
			if (!card_[1])
			{
				throw std::runtime_error("Error cardTexture");
			}



		}


		// Game的初始化操作在此进行
		void Init()
		{
			// 初始化SDL库
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
				throw std::runtime_error("SDL Error");
			}

			// 初始化IMG库
			int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
				throw std::runtime_error("SDL_image Error");
			}

			// 创建窗口
			window_ = SDL_CreateWindow("TianHui", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowsWide, windowsHeight, SDL_WINDOW_SHOWN);
			if (!window_)
			{
				std::cerr << "Error Create Windows" << std::endl;
				throw std::runtime_error("Error Create Windows");
			}

			// 创建渲染器
			render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!render_)
			{
				std::cerr << "Error Create Renderer" << std::endl;
				throw std::runtime_error("Error Create Renderer");
			}


			// 加载并设置游戏图标
			SDL_Surface* icon = IMG_Load("E:\\GamePicture\\image\\PlantsVsZombies.ico");

			SDL_SetWindowIcon(window_, icon);

			SDL_FreeSurface(icon);

			// 初始化待选择的卡片						
			initCard();

			// 初始化鼠标
			mouseTexture_ = setTouMing("E:\\GamePicture\\image\\mouse.bmp");
			if (!mouseTexture_)
			{
				throw std::runtime_error("Error MouseTexture");
			}

			// 初始化铲子
			chanziTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\chanzi.bmp");
			if (!chanziTexture_)
			{
				throw std::runtime_error("Error chanziTexture");
			}

			// 初始化铲框
			chanKuangTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\ShovelBank.bmp");
			if (!chanKuangTexture_)
			{
				throw std::runtime_error("Error chanKuangTexture");
			}

			//// 初始化小车
			//const char* carPath = "E:\\GamePicture\\image\\car.bmp";
			//carTexture_ = setTouMing(carPath);
			//if (!carTexture_)
			//{
			//	throw std::runtime_error("Error carTexture");
			//}

			// 初始化背景
			const char* background = "E:\\GamePicture\\image\\background.bmp";
			backTexture_ = IMG_LoadTexture(render_, background);
			if (!backTexture_)
			{
				throw std::runtime_error("Error BackGroundTexture");
			}

			// 初始化卡槽
			toolTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\long.bmp");
			if (!toolTexture_)
			{
				throw std::runtime_error("Error CardToolTexture");
			}


			// 设置开始界面图片
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\PvZ.bmp"));
			// 警告图片
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\warning.bmp"));
			// 开始界面
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\ready.bmp"));
			
			// 初始化植物选择表
			plantTableTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\cardSelect.bmp");
			if (!plantTableTexture_)
			{
				throw std::runtime_error("Error plantTableTexture_");
			}

			// 添加卡槽里植物的路径
			pushPath();

			init_ = true;
		}

		//找到卡槽里卡片的id
		int getCardID(const int x1, const int y1)
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

		// 渲染卡片边框和卡片
		void drawCard(const char* cardPath, const int index_, const int& posX = 330, const int& posY = 5)
		{

			SDL_Rect getRect{};
			//卡框50*70
			auto texture = setTouMing("E:\\GamePicture\\image\\Card\\cardPicture\\cardBackground.bmp", getRect);

			getRect.x = posX + 75 + 50 * (index_);
			getRect.y = posY;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);

			// 卡片40*40
			texture = setTouMing(cardPath, getRect);

			getRect.x = posX + 80 + 50 * (index_);
			getRect.y = posY + 10;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);


		}

		// 渲染整个背景
		void backGround()
		{

			// 画出背景图片
			drawBackground();

			// 画出卡槽
			drawTool();


		}


		void upDate()
		{





		}

		void drawPlant()
		{

			// 绘制植物
			for (int j = 1; j <= plants_.size(); ++j)
			{
				for (auto& plant : plants_[j])
				{
					/*std::sort(zombies_[j].begin(), zombies_[j].end(),
						[](std::shared_ptr<Zombie>& a, std::shared_ptr<Zombie>& b) {return a->getPos().x_ < b->getPos().x_; });*/
					bool allZombieDie = false;
					for (auto& zombie : zombies_[j]) {
						allZombieDie = false;
						if (zombie->isDie())
						{
							allZombieDie = true;
						}
					}
						


					if (zombies_[j].size() == 0 || allZombieDie)
					{
						/// <summary>
						/// 判断是否绘制阳光
						/// </summary>
						auto restemp = (pool_.submit([&]() {return plant->playImage(render_, false); }));

						restemp.get();

					}

					for (auto& zombie : zombies_[j])
						if (plant->getPos().y_ == noCopy::findY(zombie->getPos().y_)) {
							
							if (zombie->isDie())
							{
								continue;
							}

							if (!plant->isDie()) {
								auto restemp = (pool_.submit([&]() {return plant->draw(render_, zombie->getPos(), zombie->isDie()); }));
								int fire = restemp.get();

								if (fire != -1 && !plant->isDie() && plant->getPos().x_ <= zombie->getPos().x_ + 40)
									zombie->setHP(fire);

								break;
							}

	

						}

				}



			}


		}

		// 画出卡槽里的卡片
		void cardTool()
		{
			// 绘制卡片
			/*auto resCard1 = pool_.submit([this]() {this->drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\sunFlower.bmp", 0); });
			auto resCard = pool_.submit([this]() {this->drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\peaShooter.bmp", 1); });			
			auto resCard2 = pool_.submit([this]() {this->drawCard("E:\\GamePicture\\image\\Card\\cardPicture\\cactus.bmp", 2); });

			resCard.get();
			resCard1.get();
			resCard2.get();*/

			for (int i = 0; i < arrSelectID_.size(); ++i)
			{
				auto temp = pool_.submit([this,&i]() {drawCard(arrPath_[arrSelectID_[i]], i); });
				temp.get();
			}



		}


		// 渲染工作的主函数
		void draw()
		{

			// 绘制背景
			auto resBackGround = pool_.submit([this]()->void {
				this->backGround();
				
				});
			resBackGround.get();

			// 绘制卡槽和卡片
			this->cardTool();
			

			// 绘制小车
			for (auto& Car : carArr_)
			{
				

				const auto& i = Car.second;
				const auto& index_ = Car.first;

				if(i)
					i->drawCar(render_);
			}


			// 绘制植物
			drawPlant();

			// 绘制僵尸
			for (int j = 1; j <= zombies_.size(); ++j)
			{			

				for (auto& i : zombies_[j])
				{

					const int& xx = i->getPos().x_;
					const int& carX = carArr_[j]->getPos().x_;

					if (carArr_[j].get())
						if (carX - 80 >= xx && xx <= 1250)
							i->setHP(1000);

						//僵尸吃掉了你的脑子
						if (xx <= 200)
						{														
							kill_[j] = true;													

							succeed_ = -1;
							//quit_ = true;
						}

						//i->attack(render_, plants_[j]);

						auto temp = (pool_.submit([&i, &j, this]() {return i->attack(render_, plants_[j]); }));
						temp.get();
				}				

			}



			for (int j = 1; j <= 5; ++j) 
			{
				if (!carArr_[j].get())
					continue;

				const int& carX = carArr_[j]->getPos().x_;

				if (kill_[j])
					carArr_[j]->moveCar();

				if (carX >= 1500)
					carArr_[j] = std::shared_ptr<car>{};
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

	private:

		// 创建窗口
		SDL_Window* window_;

		// 渲染器指针
		SDL_Renderer* render_;

		// 创建线程池
		threadpool pool_;

		// 创建僵尸和其所在的路牌号
		std::map<int, std::vector<std::shared_ptr<Zombie>>>zombies_;

		// 创建植物和其所在的路牌号
		std::map<int, std::vector<std::shared_ptr<Plant>>>plants_;

		// 程序是否退出
		bool quit_;

		// Game是否已经初始化过
		bool init_;

		// 准备阶段纹理
		std::vector< SDL_Texture*> readyArrTexture_;

		// 鼠标的纹理
		SDL_Texture* mouseTexture_;

		// 背景的纹理存储
		SDL_Texture* backTexture_;

		// 卡槽的纹理存储
		SDL_Texture* toolTexture_;

		// 铲子的纹理存储
		SDL_Texture* chanziTexture_;

		// 铲子框
		SDL_Texture* chanKuangTexture_;

		// 是否杀死僵尸
		bool kill_[6]{};

		std::map<int, std::shared_ptr<car>>carArr_;

		//存放要挑选的卡片
		std::map<int, SDL_Texture*>card_;

		// 胜利判定
		int succeed_;

		// 音乐播放器
		Music music_;

		// 临时的音乐播放器
		Music tempMusic_;

		// 计时器
		Time time_;

		/////////////////////////////
		// 待开始

		// 卡片选择纹理
		SDL_Texture* plantTableTexture_;

		// 植物序号数组
		std::vector<int>arrSelectID_;

		// 植物存储的路径
		std::vector<const char*>arrPath_;

	};






}

