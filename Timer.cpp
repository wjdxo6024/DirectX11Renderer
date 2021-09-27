#include "Timer.h"

Timer::Timer()
{

}

Timer::Timer(const Timer& other)
{

}

Timer::~Timer()
{

}

bool Timer::Init()
{
	// 시스템에서 고성능 타이머를 지원하는지 확인하는게 필요하다.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	// 빈도 카운터가 매 밀리 초마다 틱하는 횟수를 확인
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void Timer::Frame()
{
	__int64 currentTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	float timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;
}

float Timer::GetTime()
{
	return m_frameTime;
}