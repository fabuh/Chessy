#pragma once
enum EEntryType
{
	Move,
	Check,
	CheckMate,
	NoType
};

struct LogEntry
{
	EEntryType m_type = EEntryType::NoType;
	const std::string* m_message = nullptr;
public:
	LogEntry(EEntryType type = EEntryType::NoType, const std::string* message = nullptr)
		: m_type(type), m_message(message) { }

	const LogEntry& operator=(const LogEntry& other)
	{
		if (this != &other)
		{
			this->m_message = other.m_message;
			this->m_type = other.m_type;
		}
		return *this;
	}
};

class Logger
{
public:
	Logger();
	~Logger();

	void Read();
	void Write();

	void AddLog(const LogEntry& entry);

private:
	void Resize();

private:
	LogEntry* m_container;
	size_t m_size;
	size_t m_maxSize;
};