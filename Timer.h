#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <Windows.h>

class Timer
{
private:
	__int64 m_frequency = 0;
	float m_ticksPerMs = 0;
	__int64 m_startTime = 0;
	float m_frameTime = 0;

public:
	Timer();
	Timer(const Timer& other);
	~Timer();

	bool Init();
	void Frame();
	float GetTime();
};

#endif