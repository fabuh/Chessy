#include "pch.h"
#include "Logger.h"

Logger::Logger() : m_size(0), m_maxSize(2)
{
	m_container = new LogEntry[m_maxSize];
}

void Logger::Read()
{

}

void Logger::Write()
{

}

Logger::~Logger()
{
	delete[] m_container;
}

void Logger::AddLog(const LogEntry& entry)
{
	if (m_size == m_maxSize)
	{
		Resize();
	}
	m_container[m_size++] = entry;
}

void Logger::Resize()
{
	m_maxSize *= 2;
	LogEntry* temp = new LogEntry[m_maxSize];
	for (size_t i = 0; i < m_size; i++)
	{
		temp[i] = m_container[i];
	}
	delete[] m_container;
	m_container = temp;
}
