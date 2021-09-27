#pragma once
#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_

#include "CpuCounter.h"
#include "FpsCounter.h"
#include "Timer.h"

class SystemManager
{
private:
	CpuCounter* m_CpuCounter;
	FpsCounter* m_FpsCounter;
	Timer* m_Timer;

public:
	SystemManager();
	SystemManager(const SystemManager& other);
	~SystemManager();

	void Init();
	void Quit();

	CpuCounter* GetCpuCounter();
	FpsCounter* GetFpsCounter();
	Timer* GetTimer();

};

#endif