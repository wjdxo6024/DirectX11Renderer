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
	// �ý��ۿ��� ���� Ÿ�̸Ӹ� �����ϴ��� Ȯ���ϴ°� �ʿ��ϴ�.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	// �� ī���Ͱ� �� �и� �ʸ��� ƽ�ϴ� Ƚ���� Ȯ��
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