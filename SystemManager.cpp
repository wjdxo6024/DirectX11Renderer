#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::SystemManager(const SystemManager& other)
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{
	m_CpuCounter = new CpuCounter();
	if (m_CpuCounter == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create CpuCounter.", L"Error", MB_OK);
#endif
		return;
	}
	m_CpuCounter->Init();

	m_FpsCounter = new FpsCounter();
	if (m_FpsCounter == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create FpsCounter.", L"Error", MB_OK);
#endif
		return;
	}
	m_FpsCounter->Init();

	m_Timer = new Timer();
	if (m_Timer == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Timer.", L"Error", MB_OK);
#endif
		return;
	}
	if (!m_Timer->Init())
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Timer.", L"Error", MB_OK);
#endif
		return;
	}
}

void SystemManager::Quit()
{
	if (m_CpuCounter != NULL)
	{
		m_CpuCounter->Quit();
		delete m_CpuCounter;
	}

	if (m_FpsCounter != NULL)
	{
		delete m_FpsCounter;
	}

	if (m_Timer != NULL)
	{
		delete m_Timer;
	}
}

CpuCounter* SystemManager::GetCpuCounter()
{
	return m_CpuCounter;
}

FpsCounter* SystemManager::GetFpsCounter()
{
	return m_FpsCounter;
}

Timer* SystemManager::GetTimer()
{
	return m_Timer;
}