#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

class CSVParser {

public:
	CSVParser() {}
	int count_line_items(std::string line, std::string delimiter = ",");
	bool has_next(std::string line, std::string delimiter = ",");

	template <typename T>
	T get_next(std::string &line, std::string delimiter = ",") {
		std::string token;
		size_t pos = line->find(delimiter);
		// Remove current token from line
		line->erase(0, pos + delimiter.length());
		token = line->substr(0);
		return boost::lexical_cast<T>(token);
	}



};

#endif // !CSV_PARSER_H
