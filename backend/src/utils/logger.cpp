#include "logger.hpp"

#define LOG "Log"
#define STD_OUT "stdout"
#define STD_ERR "stderr"

#define MAX_LOG_SIZE 1048576 * 5
#define MAX_LOG_FILES 2
#define BACKTRACE_SIZE 32

void Logger::init_logging() {
	auto log = spdlog::rotating_logger_mt(LOG, "logs/log.txt", MAX_LOG_SIZE, MAX_LOG_FILES);
	auto console = spdlog::stdout_color_mt(STD_OUT);
	auto error = spdlog::stderr_color_mt(STD_ERR);
	spdlog::enable_backtrace(BACKTRACE_SIZE);
	spdlog::get(LOG)->info("\n\n*********************    Begin Application Execution    *********************\n");
}

void Logger::log_error(std::string file, std::string function, int line, std::string error_msg) {
	std::cout << "Logging Error: " << file << std::endl;
	spdlog::get(STD_ERR)->error("{0}->{1}({2}): {3}", file, function, line, error_msg);
	spdlog::get(LOG)->error("{0}->{1}({2}): {3}", file, function, line, error_msg);

}

void Logger::log_info(std::string file, std::string function, int line, std::string msg) {
	spdlog::get(STD_OUT)->info("{0}->{1}({2}): {3}", file, function, line, msg);
	spdlog::get(LOG)->info("{0}->{1}({2}): {3}", file, function, line, msg);
}


