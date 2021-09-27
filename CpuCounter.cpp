#include "CpuCounter.h"

CpuCounter::CpuCounter()
{

}

CpuCounter::CpuCounter(const CpuCounter& other)
{

}

CpuCounter::~CpuCounter()
{

}

void CpuCounter::Init()
{
	// 폴링 : 하나의 장치가 충돌 회피 또는 동기화 처리등을 목적으로
	// 다른 장치의 상태를 주기적으로 검사하여 일정한 조건을 만족할 때
	// 송수신 등의 자료처리를 하는 방식

	// CPU 사용을 폴링하는 쿼리 개체 생성
	PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// 시스템의 모든 CPU를 폴링하도록 쿼리 개체를 설정
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}

void CpuCounter::Quit()
{
	if (m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}
}

void CpuCounter::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}
}

int CpuCounter::GetCpuPercentage()
{
	int usage = 0;

	if (m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}

	return usage;
}