#pragma once
#include"Plant.h"
#include"Time.h"
#include"Position.h"

class producePlant : public Plant
{
public:
	producePlant(const Position&p) :
		produceTime_{},
		producerCount_{}
	{
		setPos(p);
	}
	
	virtual ~producePlant() = default;

	virtual int produce(SDL_Renderer* render) = 0;

	


protected:
	Time produceTime_;//������ʱ��

	int producerCount_;//��Ʒ������

};

