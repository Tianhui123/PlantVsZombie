#pragma once
#include<thread>


class Time
{
	using Type_ = std::chrono::steady_clock::time_point;
	using Clock_ = std::chrono::steady_clock;
public:
	Time() :
		lastTime_{ Clock_::now() }
	{}


	/// <summary>
	/// 查询时间是否到了
	/// </summary>
	/// <param name="timeInterval">输入时间间隔，默认2500 ms（单位ms）</param>
	/// <returns>时间到了返回 true </returns>
	bool queryTime(const long long& timeInterval = 2500)noexcept
	{
		Type_ currentTime = Clock_::now();

		auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime_);

		if (temp.count() >= timeInterval)
		{
			lastTime_ = currentTime;
			return true;
		}


		return false;
	}

	//查询现在距离创建时的时间间隔
	long long pushDown()
	{
		Type_ currentTime = Clock_::now();

		return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime_).count();
	}


private:

	Type_ lastTime_;

};

