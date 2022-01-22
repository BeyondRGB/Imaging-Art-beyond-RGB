#include "csv_parser.hpp"

int CSVParser::count_line_items(std::string line, std::string delimiter) {
	size_t start = 0;
	size_t pos = line.find(delimiter, start);
	int i = 0;
	while (pos != std::string::npos) {
		start = pos + delimiter.length();
		pos = line.find(delimiter, start);
		i++;
	}
	return ++i;
}

bool CSVParser::has_next(std::string line, std::string delimiter) {
	size_t pos = line.find(delimiter);
	std::cout << pos << std::endl;
	return pos != std::string::npos;
}