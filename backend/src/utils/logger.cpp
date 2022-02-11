#include "logger.hpp"

#define LOG "Logger"
#define STD_OUT "stdout"
#define STD_ERR "stderr"

#define MAX_LOG_SIZE 1000000 * 5 // 5 MB
#define MAX_LOG_FILES 2
#define BACKTRACE_SIZE 32

std::string remove_path_from_filename(std::string file_path);

void Logger::init_logging() {
	// This should only ever allow two log files of max size 5 MB
	// When they both fill up it should overwrite the oldest
	//TODO once we have figured out our pathing this should be updated to use that
	auto log = spdlog::rotating_logger_mt(LOG, "logs/log.txt", MAX_LOG_SIZE, MAX_LOG_FILES);
	auto console = spdlog::stdout_color_mt(STD_OUT);
	auto error = spdlog::stderr_color_mt(STD_ERR);
	spdlog::enable_backtrace(BACKTRACE_SIZE);
	spdlog::get(LOG)->info("\n*********************    Begin Application Execution    *********************\n");
}

void Logger::log_error(std::string file_path, std::string function, int line, std::string error_msg, bool remove_path) {
	std::string file = file_path;
	if (remove_path)
		file = remove_path_from_filename(file_path);
	spdlog::get(STD_ERR)->error("({0}:{2}->{1}()): {3}", file, function, line, error_msg);
	spdlog::get(LOG)->error("({0}:{2}->{1}()): {3}", file, function, line, error_msg);

}

void Logger::log_info(std::string file_path, std::string function, int line, std::string msg, bool remove_path) {
	std::string file = file_path;
	if(remove_path)
		file = remove_path_from_filename(file_path);
	spdlog::get(STD_OUT)->info("({0}:{2}->{1}()): {3}", file, function, line, msg);
	spdlog::get(LOG)->info("({0}:{2}->{1}()): {3}", file, function, line, msg);
}

std::string remove_path_from_filename(std::string file_path) {
	const char* fp = file_path.c_str();
	// Remove window path
	fp = (strrchr(fp, '\\') ? strrchr(fp, '\\') + 1 : fp);
	// Remove linux/osx path
	std::string file = (strrchr(fp, '/') ? strrchr(fp, '/') + 1 : fp);
	return file;
}


