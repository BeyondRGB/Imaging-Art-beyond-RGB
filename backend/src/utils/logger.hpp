#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"
#include "spdlog/spdlog.h"
#include <string>
#include <iostream>


/**
* Macro for logging errors.
* If this macro is used it will automaticly include filename, line number, and function name 
* from where the macro was called.
* It will log the error to the log file and to stderr.
* @param msg: the message to log
*/
#define LOG_ERROR(msg) Logger::log_error(__FILE__ , __FUNCTION__, __LINE__, msg)

/**
* Macro for logging errors.
* If this macro is used it will automaticly include filename, line number, and function name
* from where the macro was called.
* It will log the error to the log file and to stdout.
* @param msg: the message to log
*/
#define LOG_INFO(msg) Logger::log_info(__FILE__ , __FUNCTION__, __LINE__, msg)

class Logger {
public:
	/**
	* Initializes logging.
	* This function must be called before attempting to use any other
	* logging funcitons.
	* It is currently called in main
	*/
	static void init_logging();

	/**
	* Log the given error msg.
	* This will append an error to the log.txt as well as ouput an error to stderr
	* This function is not inteded for general use but it needs to be public for the macros to work,
	* instead it is recomend to use the LOG_ERR macro
	* @param file_path: the file name and path for where this function is called
	* @param function: the function name where this was called from
	* @param line: the line number where this was called from
	* @param error_msg: the message to log
	* @param remove_path: bool to identify if the path should be removed from the filename
	*		if true only the file name will appear in the logged msg
	*		otherwise the path is included
	*		this defaults to true
	*/
	static void log_error(std::string file_path, std::string function, int line, std::string error_msg, bool remove_path = true);
	
	/**
	* Log the given info msg.
	* This will append an info msg to the log.txt as well as ouput an info msg to stdout
	* This function is not inteded for general use but it needs to be public for the macros to work,
	* instead it is recomend to use the LOG_INFO macro
	* @param file_path: the file name and path for where this function is called
	* @param function: the function name where this was called from
	* @param line: the line number where this was called from
	* @param msg: the message to log
	* @param remove_path: bool to identify if the path should be removed from the filename
	*		if true only the file name will appear in the logged msg
	*		otherwise the path is included
	*		this defaults to true
	*/
	static void log_info(std::string file_path, std::string function, int line, std::string msg, bool remove_path = true);

};
#endif // !LOGGER_H
