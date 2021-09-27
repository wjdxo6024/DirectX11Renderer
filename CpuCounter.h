#pragma once
#ifndef _CPUCOUNTER_H_
#define _CPUCOUNTER_H_

#pragma comment(lib, "pdh.lib")
#include <Pdh.h>

// CpuCounter�� cpu�� ������ ǥ���� �� �ִ� Ŭ����

class CpuCounter
{
private:
	bool m_canReadCpu = true;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime = 0;
	long m_cpuUsage = 0;
public:
	CpuCounter();
	CpuCounter(const CpuCounter& other);
	~CpuCounter();

	void Init();
	void Quit();
	void Frame();
	int GetCpuPercentage();
};

#endif