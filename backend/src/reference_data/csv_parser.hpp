#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

class CSVParser {

public:
	CSVParser() {}
	bool open_file(std::string file);
	void close_file();
	int count_line_items(std::string line, std::string delimiter = ",");
	bool has_next(std::string line, std::string delimiter = ",");
	std::string get_next_line();
	bool has_next_line();
	template <typename T>
	T get_next(std::string &line, std::string delimiter = ",") {
		std::string token;
		size_t pos = line.find(delimiter);
		token = line.substr(0, pos);
		// Remove current token from line
		size_t erase_stop = pos + delimiter.length();
		if (this->has_one_item(line))
			// Erase entire line
			erase_stop = line.length();
		line.erase(0, erase_stop);
		return boost::lexical_cast<T>(token);
	}

private:
	bool has_one_item(std::string line, std::string delimiter = ",");
	std::ifstream file_m;


};

#endif // !CSV_PARSER_H
