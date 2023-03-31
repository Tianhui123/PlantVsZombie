#pragma once
#include"noCopy.h"
#include<memory>
class inPut final: noCopy
{
public:
	static std::shared_ptr<SDL_Event> getInstance()
	{

		if (!isGet_) 
		{
			static inPut temp{};
			event_ = std::shared_ptr<SDL_Event>( new SDL_Event{});
			isGet_ = true;
		}
			
		return event_;
	}

private:
	static std::shared_ptr<SDL_Event>event_;
	static bool isGet_;

	inPut() = default;

	~inPut() = default;
};

bool inPut::isGet_ = false;
std::shared_ptr<SDL_Event> inPut::event_ = {};