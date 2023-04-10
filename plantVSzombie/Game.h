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

				// �������
				SDL_ShowCursor(false);


				const int X = 270, Y = 90;
				Position posArr[5] = { {X,Y + 50},{X,Y + 170},{X,Y + 300},{X - 4,Y + 420},{X - 12,Y + 560} };
				for (int i = 1; i <= 5; ++i)
				{
					// ��ʬ��ֲ���ʼ��
					plants_[i];
					zombies_[i];

					// ����С��
					carArr_[i] = std::shared_ptr<car>{ new car{posArr[i - 1]} };
					carArr_[i]->setPath(render_,"E:\\GamePicture\\image\\car.bmp");

				}


				//������ʬ
				creatZombie(10);


			}


		}
		
		// �ͷ���Դ����
		~Game()
		{
			// �����˳�
			if (isQuit())
			{

				if (isInit())
				{
					// �ͷ���Դ���رմ���
					if(window_)
						SDL_DestroyWindow(window_);

					// �ͷ���Ⱦ��
					if(render_)
						SDL_DestroyRenderer(render_);

					// �ͷŲ�����Դ
					if(chanziTexture_)
						SDL_DestroyTexture(chanziTexture_);

					// �ͷŲ���
					if (chanKuangTexture_)
						SDL_DestroyTexture(chanKuangTexture_);

					// �ͷ�ֲ��ѡ���
					if (plantTableTexture_)
						SDL_DestroyTexture(plantTableTexture_);

					//// �ͷ�С��
					//if(carTexture_)
					//	SDL_DestroyTexture(carTexture_);

					// �ͷű�����Դ
					if(backTexture_)
						SDL_DestroyTexture(backTexture_);

					// �ͷű�����Դ
					if(toolTexture_)
						SDL_DestroyTexture(toolTexture_);

					// �ͷ������Դ
					if(mouseTexture_)
						SDL_DestroyTexture(mouseTexture_);

					window_ = nullptr;
					render_ = nullptr;

					// �ر�IMG��
					IMG_Quit();

					// �ر�SDL��
					SDL_Quit();



					init_ = false;

				}



			}

			// �ͷ�׼���׶ε���Դ
			for (auto& i : readyArrTexture_)
			{
				if (i)
					SDL_DestroyTexture(i);
			}

			// �ͷſ����￨Ƭ����Դ
			for (auto& i : card_)
			{
				const auto& text = i.second;
				if (text)
					SDL_DestroyTexture(text);
			}


		}

		// ���������λ���ҵ�ֲ��Ӧ����ֲ��λ�á�
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

		//���뽩ʬ�ڵڼ������ӣ����ؽ�ʬ��Y���ꡣ
		static int findZombieY(int i)
		{
			int index_ = 50 + (i - 1) * 130;
			return noCopy::findY(index_ - 1) - 70;
		}

		// ����������ֲ��
		void clearDie()
		{


			// ��������ֲ��
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


			//����������ʬ
			for (auto& zombie : zombies_)
			{

				auto begin = zombie.second.begin();

				int size = zombie.second.size() - 1;

				// Ӯ
				succeed_ = 0;

				for (int i = size; i >= 0; --i)
				{
					// ��
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

		// ��ʬ�б�����ӽ�ʬ
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

		// ��Ⱦ��ѡ��Ŀ�Ƭ
		void drawSelectCard(int cardID, const Position& pos)
		{

			allDraw(card_[cardID], pos);

		}

		// ��Ⱦ������
		void drawChanzi(const Position& pos,bool isDraw)
		{
			if (!isDraw)
				return;

			allDraw(chanziTexture_, pos);

		}

		// ��Ⱦ������
		void drawChanKuang(const Position& pos)
		{
			allDraw(chanKuangTexture_, pos);
		}

		//// ��Ⱦ��С��
		//void drawCar(const Position&pos)
		//{

		//	allDraw(carTexture_, pos);

		//}

		// ��Ⱦ������ͼƬ
		void drawBackground()
		{

			int imageW{}, imageH{};
			SDL_QueryTexture(backTexture_, nullptr, nullptr, &imageW, &imageH);


			SDL_Rect windowRect{};

			windowRect.w = 3 * imageW / 4;
			windowRect.h = imageH;

			SDL_RenderCopy(render_, backTexture_, &windowRect, nullptr);

		}

		// ��Ⱦ������
		void drawTool()
		{

			allDraw(toolTexture_, Position(330, 0));

		}

		// ��Ⱦ��ѡ����
		void drawSelectTable()
		{
			allDraw(plantTableTexture_, Position( 330, 100));
		}

		// �ж��Ƿ�ѡ���˲���
		bool getChanzi(int x1, int y1)
		{
			if (x1 >= 1335 && x1 <= 1381)
				if (y1 >= 12 && y1 <= 67)
					return true;

			return false;
		}

		// �ж��Ƿ������̫��
		bool isSeclctSun(int x1, int y1)
		{
			auto p = findGrid(x1, y1);

			if (x1 >= p.x_ && x1 <= p.x_ + 61)
				if (y1 >= p.y_ + 56 && y1 <= p.y_ + 124)

					return true;

			return false;

		}

		// ����λ�û������ͼ��
		void drawMouse(const Position&e,bool isDraw)
		{
			if (!isDraw)
				return;

			allDraw(mouseTexture_, e);
		}

		// �ж��Ƿ�����ȷ�ϰ�ť
		bool buttonACK(const int &eX,const int &eY)
		{
			if (eX >= 654 && eX <= 705)
				if (eY >= 657 && eY <= 676)
					return true;

			return false;
		}

		//�ҵ������￨Ƭ��id
		int getSelectID(const int x1, const int y1)
		{

			const int Xtemp = 354, Xjian = 73;
			int x{ -1 };

			// �Ե�һ�н����ж�
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

		// ��ѡ������ֲ��
		void drawSelectID(const char *path,const int& index_)
		{

			int posX{ 265 }, posY{ 130 };
			SDL_Rect getRect{ posX,posY,0,0 };
			// ��Ƭ40*40
			auto texture = setTouMing(path, getRect);

			getRect.x = posX + 80 + 73 * (index_);
			getRect.y = posY + 10;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);

		}

		// ���ֲ��·��
		void pushPath()
		{
			// ̫���� 0
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\sunFlower.bmp");

			// �㶹���� 1
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\peaShooter.bmp");

			// ������ 2
			arrPath_.push_back("E:\\GamePicture\\image\\Card\\cardPicture\\cactus.bmp");

		}

		// ��ʼǰ����ѡֲ��׶�
		void selectPlant()
		{
			///////////////////////////////////////
			// ���ÿ�Ƭѡ��·��
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


			// ��������
			music_.setMusic("E:\\GamePicture\\Music\\mainmusic.ogg");
			music_.playMusic();

			// ѡ����Ч
			tempMusic_.setChunk("E:\\GamePicture\\Music\\seedlift.ogg");
			

			// ����ͼ�δ�С
			SDL_Rect rect{ 0,0,windowsWide,windowsHeight }, windowRect{ 0,0,windowsWide*3/4,windowsHeight };

			// ���λ��
			int eX{}, eY{};

			//ѡ��Ŀ�Ƭ
			int ID{ -1 };
			
			while (!isQuit())
			{
				// ��ȡ���λ��
				SDL_GetMouseState(&eX, &eY);
				
				// ������
				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				
				// �����ƶ�����
				int size = windowsWide / 4;
				if (windowRect.x <= size)
				{
					windowRect.x += 2;

					if (windowRect.x > size)
						windowRect.x = size;

				}
				SDL_RenderCopy(render_, backTexture_, &windowRect, nullptr);

				// �����ƶ�ֹͣ
				if (windowRect.x == size)
				{
					auto res = pool_.submit([&,this]()
						{
							// ��������
							drawTool();

							// ����ѡ��� 131 * 100
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

							// ������ѡ���ֲ�� 280,145
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


							// ��˳���ڿ����ﻭ����Ƭ
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
				
				

				// ��ȡ�����¼�
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{


					// ��ȡ���̰���
					switch (e->key.keysym.sym)
					{
					case SDLK_ESCAPE:
						return;
						break;

					default:
						break;
					}



					// ��ȡ��갴��
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
								// ������Ч
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







				// �������
				drawMouse(Position(eX, eY), true);

				SDL_RenderPresent(render_);
				SDL_Delay(5);


			}



			// �ͷ�ֲ��ѡ���
			if (plantTableTexture_)
				SDL_DestroyTexture(plantTableTexture_);

			plantTableTexture_ = nullptr;


		}

		// ������Ϸ׼������
		void ready()
		{
		

			// ��������
			music_.setMusic("E:\\GamePicture\\Music\\mainmusic.ogg");
			music_.playMusic();
			

			// ����ͼ�δ�С
			SDL_Rect rect{ windowsWide / 4,windowsHeight / 4,0,0 };
			SDL_Rect* rectPtr{ &rect };

			// �±�
			int index_{};

			// ���λ��
			int eX{}, eY{};

			while (!isQuit())
			{
				
				// ������
				SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
				SDL_RenderClear(render_);

				// ��ȡ���λ��
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

				// ��ȡ�����¼�
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{


					// ��ȡ���̰���
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




					// ��ȡ��갴��
					switch (e->type)
					{
						
					case SDL_QUIT :
						quit_ = true;
						break;

					case SDL_MOUSEMOTION:						
						// �ƶ���"ȷ��"��ťʱ
						if (buttonACK(eX, eY)&& !rectPtr)
						{
							// ��ʾ���
							//SDL_ShowCursor(true);

						}
						else
						{
							
							
						}

						
						break;


					case SDL_MOUSEBUTTONDOWN:
						if (e->button.button == SDL_BUTTON_LEFT)
						{

							// �����"ȷ��"��ť
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



				// �����
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

		// ��ʼ��Ϸ
		void start()
		{
			// ��������
			music_.setMusic("E:\\GamePicture\\Music\\KitanaiSekai.ogg");
			music_.playMusic();

			//��¼�������������
			int leftCount{};
			//��¼�������������
			int chanLeftCount{};

			//������ڼ�����Ƭ
			int cardID = { -1 };

			//�����Ƿ����ֲ��
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

				


				// ʤ����
				if (succeed_ == 0)
				{
					//end();quit=true;
					//quit_ = true;
				}
				else if (succeed_ == -1)//����
				{

				}

				//clearDie();

				{
					auto res = pool_.submit([this]() {this->clearDie(); });
					res.get();
				}

				//e_move = false;

				//��ȡ���λ��
				int x{}, y{};
				auto button = SDL_GetMouseState(&x, &y);

				//�ҵ�ֲ����ֲ��
				auto temp = findGrid(x, y);


				//�жϵ�ǰλ���Ƿ���ֲ������ñ�־
				bool flagPlantplants = true;
				for (auto& plant : plants_)
				{
					for (auto& i : plant.second)
						if (temp == i->getPos() && !i->isDie())
							flagPlantplants = false;
				}


				//��ѡ���ֲ��
				if (cardID != -1)
				{

					pool_.submit([this, cardID](const Position& temp) {
						if (cardID < arrSelectID_.size() && cardID != -1)
						{
							drawSelectCard(arrSelectID_[cardID], temp);
						}

						}, Position(x - 40, y - 40));
					

				}


				// �����ӺͲ���
				pool_.submit([this, chanFlag,x,y]() 
					{
						// ��������
						drawChanKuang(Position(1320, 0));

						// ��������
						drawChanzi(Position(1320 - 5, -10), !chanFlag);

						//�����һ�µ��в���
						if (chanFlag)
						{


							// �����ƶ�����
							drawChanzi(Position(x - 40, y - 40), true);


						}

					});


				


				
				
				bool sunFlag_ = false;


				// ��ȡ�����¼�
				decltype(inPut::getInstance()) e = inPut::getInstance();
				while (SDL_PollEvent(e.get()) != 0)
				{
					if (e->type == SDL_QUIT)
					{
						quit_ = true;
					}

					//�ж�����Ƿ��ƶ�

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
							// ������Ⱦ����
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
							/// ����̫��
							/// </summary>
							for (auto& plant : plants_)
							{
								// û�е���
								if (!sunFlag_)
									break;

								for (auto& i : plant.second)
								{
									
									/*
									auto p = findGrid(x1, y1);

									if (x1 >= p.x_ && x1 <= p.x_ + 61)
									if (y1 >= p.y_ + 56 && y1 <= p.y_ + 124)
									*/

									// ����̫���ҵ��������̫���Ļ�
									auto tmp = findGrid(x, y - 34);
									

									// �����sunFlower
									if (typeid(*i) == typeid(sunFlower))
									{
										auto temp = dynamic_cast<sunFlower*> (i.get());
										if (!temp)
										{
											throw std::runtime_error("����ȫת����ת��ʧ��");
										}

										// �ҵ��˱���̫�������տ�
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

									//�ҵ�����Ҫ������ֲ�������
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



							//�ڶ��ε����꣬��ֲֲ��
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
										// �жϵ��������һ��ֲ��
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





								//�жϵ��ֲ�￨Ƭ
								cardID = -1;



							}
							else//��һ�ε�����
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





				

				// �����
				auto ttmouse = pool_.submit([&]() {drawMouse(Position(x - 5, y - 1), true); });
				ttmouse.get();

				
				SDL_RenderPresent(render_);

				SDL_Delay(50);

			}

		}


	private:

		// ͨ�õ���ָ��λ�û�������
		void allDraw(SDL_Texture* texture, const Position& pos)
		{
			SDL_Rect getRect{};
			SDL_QueryTexture(texture, nullptr, nullptr, &getRect.w, &getRect.h);

			getRect.x = pos.x_;
			getRect.y = pos.y_;


			SDL_RenderCopy(render_, texture, nullptr, &getRect);

		}



		// ��ͼƬ�����߿����ó�͸��
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
		
		// ���غ���
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

		// �ж��Ƿ��˳�
		inline bool isQuit()
		{
			return quit_;
		}

		// �ж��Ƿ��ʼ��
		inline bool isInit()
		{
			return init_;
		}

		// ��ʼ����ѡ��Ŀ�Ƭ
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


		// Game�ĳ�ʼ�������ڴ˽���
		void Init()
		{
			// ��ʼ��SDL��
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
				throw std::runtime_error("SDL Error");
			}

			// ��ʼ��IMG��
			int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
				throw std::runtime_error("SDL_image Error");
			}

			// ��������
			window_ = SDL_CreateWindow("TianHui", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowsWide, windowsHeight, SDL_WINDOW_SHOWN);
			if (!window_)
			{
				std::cerr << "Error Create Windows" << std::endl;
				throw std::runtime_error("Error Create Windows");
			}

			// ������Ⱦ��
			render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!render_)
			{
				std::cerr << "Error Create Renderer" << std::endl;
				throw std::runtime_error("Error Create Renderer");
			}


			// ���ز�������Ϸͼ��
			SDL_Surface* icon = IMG_Load("E:\\GamePicture\\image\\PlantsVsZombies.ico");

			SDL_SetWindowIcon(window_, icon);

			SDL_FreeSurface(icon);

			// ��ʼ����ѡ��Ŀ�Ƭ						
			initCard();

			// ��ʼ�����
			mouseTexture_ = setTouMing("E:\\GamePicture\\image\\mouse.bmp");
			if (!mouseTexture_)
			{
				throw std::runtime_error("Error MouseTexture");
			}

			// ��ʼ������
			chanziTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\chanzi.bmp");
			if (!chanziTexture_)
			{
				throw std::runtime_error("Error chanziTexture");
			}

			// ��ʼ������
			chanKuangTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\ShovelBank.bmp");
			if (!chanKuangTexture_)
			{
				throw std::runtime_error("Error chanKuangTexture");
			}

			//// ��ʼ��С��
			//const char* carPath = "E:\\GamePicture\\image\\car.bmp";
			//carTexture_ = setTouMing(carPath);
			//if (!carTexture_)
			//{
			//	throw std::runtime_error("Error carTexture");
			//}

			// ��ʼ������
			const char* background = "E:\\GamePicture\\image\\background.bmp";
			backTexture_ = IMG_LoadTexture(render_, background);
			if (!backTexture_)
			{
				throw std::runtime_error("Error BackGroundTexture");
			}

			// ��ʼ������
			toolTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\long.bmp");
			if (!toolTexture_)
			{
				throw std::runtime_error("Error CardToolTexture");
			}


			// ���ÿ�ʼ����ͼƬ
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\PvZ.bmp"));
			// ����ͼƬ
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\warning.bmp"));
			// ��ʼ����
			readyArrTexture_.push_back(setTouMing("E:\\GamePicture\\image\\ready.bmp"));
			
			// ��ʼ��ֲ��ѡ���
			plantTableTexture_ = setTouMing("E:\\GamePicture\\image\\Card\\cardSelect\\cardSelect.bmp");
			if (!plantTableTexture_)
			{
				throw std::runtime_error("Error plantTableTexture_");
			}

			// ��ӿ�����ֲ���·��
			pushPath();

			init_ = true;
		}

		//�ҵ������￨Ƭ��id
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

		// ��Ⱦ��Ƭ�߿�Ϳ�Ƭ
		void drawCard(const char* cardPath, const int index_, const int& posX = 330, const int& posY = 5)
		{

			SDL_Rect getRect{};
			//����50*70
			auto texture = setTouMing("E:\\GamePicture\\image\\Card\\cardPicture\\cardBackground.bmp", getRect);

			getRect.x = posX + 75 + 50 * (index_);
			getRect.y = posY;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);

			// ��Ƭ40*40
			texture = setTouMing(cardPath, getRect);

			getRect.x = posX + 80 + 50 * (index_);
			getRect.y = posY + 10;
			SDL_RenderCopy(render_, texture, nullptr, &getRect);

			SDL_DestroyTexture(texture);


		}

		// ��Ⱦ��������
		void backGround()
		{

			// ��������ͼƬ
			drawBackground();

			// ��������
			drawTool();


		}


		void upDate()
		{





		}

		void drawPlant()
		{

			// ����ֲ��
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
						/// �ж��Ƿ��������
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

		// ����������Ŀ�Ƭ
		void cardTool()
		{
			// ���ƿ�Ƭ
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


		// ��Ⱦ������������
		void draw()
		{

			// ���Ʊ���
			auto resBackGround = pool_.submit([this]()->void {
				this->backGround();
				
				});
			resBackGround.get();

			// ���ƿ��ۺͿ�Ƭ
			this->cardTool();
			

			// ����С��
			for (auto& Car : carArr_)
			{
				

				const auto& i = Car.second;
				const auto& index_ = Car.first;

				if(i)
					i->drawCar(render_);
			}


			// ����ֲ��
			drawPlant();

			// ���ƽ�ʬ
			for (int j = 1; j <= zombies_.size(); ++j)
			{			

				for (auto& i : zombies_[j])
				{

					const int& xx = i->getPos().x_;
					const int& carX = carArr_[j]->getPos().x_;

					if (carArr_[j].get())
						if (carX - 80 >= xx && xx <= 1250)
							i->setHP(1000);

						//��ʬ�Ե����������
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

		// ��������
		SDL_Window* window_;

		// ��Ⱦ��ָ��
		SDL_Renderer* render_;

		// �����̳߳�
		threadpool pool_;

		// ������ʬ�������ڵ�·�ƺ�
		std::map<int, std::vector<std::shared_ptr<Zombie>>>zombies_;

		// ����ֲ��������ڵ�·�ƺ�
		std::map<int, std::vector<std::shared_ptr<Plant>>>plants_;

		// �����Ƿ��˳�
		bool quit_;

		// Game�Ƿ��Ѿ���ʼ����
		bool init_;

		// ׼���׶�����
		std::vector< SDL_Texture*> readyArrTexture_;

		// ��������
		SDL_Texture* mouseTexture_;

		// ����������洢
		SDL_Texture* backTexture_;

		// ���۵�����洢
		SDL_Texture* toolTexture_;

		// ���ӵ�����洢
		SDL_Texture* chanziTexture_;

		// ���ӿ�
		SDL_Texture* chanKuangTexture_;

		// �Ƿ�ɱ����ʬ
		bool kill_[6]{};

		std::map<int, std::shared_ptr<car>>carArr_;

		//���Ҫ��ѡ�Ŀ�Ƭ
		std::map<int, SDL_Texture*>card_;

		// ʤ���ж�
		int succeed_;

		// ���ֲ�����
		Music music_;

		// ��ʱ�����ֲ�����
		Music tempMusic_;

		// ��ʱ��
		Time time_;

		/////////////////////////////
		// ����ʼ

		// ��Ƭѡ������
		SDL_Texture* plantTableTexture_;

		// ֲ���������
		std::vector<int>arrSelectID_;

		// ֲ��洢��·��
		std::vector<const char*>arrPath_;

	};






}

