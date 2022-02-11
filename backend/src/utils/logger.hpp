#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"
#include "spdlog/spdlog.h"
#include <string>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define FUNC 
#define LOG_ERROR(msg) Logger::log_error(__FILE__ , __FUNCTION__, __LINE__, msg)
#define LOG_INFO(msg) Logger::log_info(__FILE__ , __FUNCTION__, __LINE__, msg)

class Logger {
public:
	static void init_logging();
	static void log_error(std::string file_path, std::string function, int line, std::string error_msg, bool remove_path = true);
	static void log_info(std::string file_path, std::string function, int line, std::string msg, bool remove_path = true);

};
#endif // !LOGGER_H
