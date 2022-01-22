#include "csv_parser.hpp"

int CSVParser::count_line_items(std::string line, std::string delimiter) {
	size_t pos = line.find(delimiter);
	int i = 0;
	while (pos != std::string::npos) {
		// Remove current token from line
		line.erase(0, pos + delimiter.length());
		pos = line.find(delimiter);
		i++;
	}
	return ++i;
}

bool CSVParser::has_next(std::string line, std::string delimiter) {
	size_t pos = line.find(delimiter);
	return true;
}