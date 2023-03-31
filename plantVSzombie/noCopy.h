#pragma once
#include <SDL.h>
#include <SDL_image.h>
const int windowsWide = 1400;
const int windowsHeight = 800;

struct noCopy
{
public:
	static int findY(int y1)
	{
		const int Ytemp = 0, Yjian = 130, middleY = 125;
		int y{ middleY };
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

		return y;

	}

	static int findZombieRode(int y1)
	{
		const int Ytemp = 0, Yjian = 130;
		int y{ 1 };
		if (y1 >= Ytemp && y1 <= Ytemp + 5 * Yjian)
		{
			if (y1 >= Ytemp && y1 <= Ytemp + Yjian)
				y = 1;
			else if (y1 > Ytemp + Yjian && y1 <= Ytemp + 2 * Yjian)
				y = 2;
			else if (y1 > Ytemp + 2 * Yjian && y1 <= Ytemp + 3 * Yjian)
				y = 3;
			else if (y1 > Ytemp + 3 * Yjian && y1 <= Ytemp + 4 * Yjian)
				y = 4;
			else if (y1 > Ytemp + 4 * Yjian && y1 <= Ytemp + 5 * Yjian)
				y = 5;

		}

		return y;

	}

	static int findPlntRode(int y1)
	{
		const int Ytemp = 109, Yjian = 130;
		int y{ 1 };
		if (y1 >= Ytemp && y1 <= Ytemp + 5 * Yjian)
		{
			if (y1 >= Ytemp && y1 <= Ytemp + Yjian)
				y = 1;
			else if (y1 > Ytemp + Yjian && y1 <= Ytemp + 2 * Yjian)
				y = 2;
			else if (y1 > Ytemp + 2 * Yjian && y1 <= Ytemp + 3 * Yjian)
				y = 3;
			else if (y1 > Ytemp + 3 * Yjian && y1 <= Ytemp + 4 * Yjian)
				y = 4;
			else if (y1 > Ytemp + 4 * Yjian && y1 <= Ytemp + 5 * Yjian)
				y = 5;

		}

		return y;

	}

protected:
	noCopy() = default;
	~noCopy() = default;

	noCopy(const noCopy&) = delete;
	noCopy(noCopy&&) = delete;
	noCopy& operator=(const noCopy&) = delete;
	noCopy& operator=(noCopy&&) = delete;

};









