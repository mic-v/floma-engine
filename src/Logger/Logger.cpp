#include "Logger.h"
#include <iostream>

#include <chrono>
#include <ctime>
#include <string>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::strftime(&output[0], output.size(), " %d-%b-%Y- %H:%M:%S", std::localtime(&time));
	return output;
}

void Logger::Log(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "[LogINFO - " + CurrentDateTimeToString() + "]: " + message;
	std::cout <<"\x1B[32m" << logEntry.message << "\033[0m\n";
	messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.message = "[LogERR - " + CurrentDateTimeToString() + "]: " + message;
	std::cerr << "\x1B[91m" << logEntry.message << "\033[0m\n";
	messages.push_back(logEntry);

}
