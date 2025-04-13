#include <Include/Engine/Core/Diagnostics/Logger.h>

Logger::Logger() : m_shouldStop(false), m_buffer{ 0 }, m_currentSize(0), m_lastSize(0) {
	startWorker();
}

void Logger::addLog(const std::string message) {
	if (m_currentSize + message.length() + 1 >= sizeof(m_buffer))
	{
		clearBuffer();
	}
	memcpy(m_buffer + m_currentSize, message.c_str(), message.length());
	m_currentSize += message.length();
	m_buffer[m_currentSize] = '\n';
	m_currentSize++;
	m_buffer[m_currentSize] = '\0';
}
void Logger::logInfo(const std::string message) {
	push(std::make_pair(Info, message));
}

void Logger::logWarning(const std::string message) {
	push(std::make_pair(Warning, message));
}

void Logger::logError(const std::string message) {
	push(std::make_pair(Error, message));
}

void Logger::logCriticalError(const std::string message) {
	push(std::make_pair(Critical, message));
}

void Logger::push(std::pair<LogLevel, std::string> pair) {
	LockGuard<CriticalSection> lock(m_cs);
	m_queue.push_back(pair);
	m_condition.notifyOne();
}

Logger::~Logger() {
	m_shouldStop = true;
	m_condition.notifyAll();
	if (m_workerThread.joinable()) {
		m_workerThread.join();
	}
}

void Logger::startWorker() {
	m_workerThread = thread(&Logger::threadCycle, this);
}

void Logger::threadCycle() {
	UniqueLockCS<CriticalSection> lock(m_cs);
	lock.unlock();
	while (!m_shouldStop) {
		m_condition.wait(lock, [this] { return !m_queue.empty() || m_shouldStop; });
		m_file.open("out/logs/log.txt", std::ios::app);
		if (!m_file.is_open()) {
			OutputDebugStringA("Debug message: Can not create log file!\n");
			return;
		}
		while (!m_queue.empty()) {
			auto instance = m_queue.front();
			m_queue.pop_front();
			lock.lock();
			writeInLogs(instance.second, instance.first);
			lock.unlock();
		}
		m_file.close();
	}
}

void Logger::writeInLogs(const std::string message, LogLevel level) {
	if (m_currentSize + message.length() + 1 >= sizeof(m_buffer))
	{
		clearBuffer();
	}
	setLogLevel(level);
	addLog(message);
	flush();
}

void Logger::flush() {
	if (m_file.is_open()) {
		m_file << std::string(m_buffer + m_lastSize, m_currentSize - m_lastSize);
	}
}

void Logger::clearLogs() {
	if (m_file.is_open()) {
		m_file.close();
	}

	std::ofstream file("out/logs/log.txt", std::ios::trunc);
	file << "";
	file.close();

	m_file.open("out/logs/log.txt", std::ios::app);

	m_currentSize = 0;
	m_buffer[m_currentSize] = '\0';
}

void Logger::clearBuffer() {
	m_currentSize = 0;
	m_buffer[m_currentSize] = '\0';
}

void Logger::setLogLevel(LogLevel level) {
	std::string logPrefix;
	m_lastSize = m_currentSize;
	switch (level) {
	case LogLevel::Info:
		logPrefix = "[INFO]: ";
		break;
	case LogLevel::Warning:
		logPrefix = "[WARNING]: ";
		break;
	case LogLevel::Error:
		logPrefix = "[ERROR]: ";
		break;
	case LogLevel::Critical:
		logPrefix = "[CRITICAL]: ";
		break;
	default:
		logPrefix = "[UNKNOWN]: ";
		break;
	}

	size_t logPrefixLen = logPrefix.size();
	if (m_currentSize + logPrefixLen + 1 >= sizeof(m_buffer))
	{
		clearBuffer();
	}
	memcpy(m_buffer + m_currentSize, logPrefix.c_str(), logPrefixLen);
	m_currentSize += logPrefixLen;
}