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
	/// ��ѯʱ���Ƿ���
	/// </summary>
	/// <param name="timeInterval">����ʱ������Ĭ��2500 ms����λms��</param>
	/// <returns>ʱ�䵽�˷��� true </returns>
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

	//��ѯ���ھ��봴��ʱ��ʱ����
	long long pushDown()
	{
		Type_ currentTime = Clock_::now();

		return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime_).count();
	}


private:

	Type_ lastTime_;

};

