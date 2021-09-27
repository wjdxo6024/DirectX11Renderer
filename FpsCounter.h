#pragma once
#ifndef _FPSCOUNTER_H_
#define _FPSCOUNTER_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>

class FpsCounter
{
private:
	int m_fps, m_count;
	unsigned long m_startTime;

public:
	FpsCounter();
	FpsCounter(const FpsCounter& other);
	~FpsCounter();

	void Init();
	void Frame();
	int GetFps();
};

#endif

