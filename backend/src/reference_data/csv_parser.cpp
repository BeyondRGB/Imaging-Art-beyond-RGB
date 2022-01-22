#include "csv_parser.hpp"

bool CSVParser::open_file(std::string file){
	if (this->file_m.is_open())
		this->close_file();
	this->file_m.open(file);
	if (this->file_m.fail())
		return true;
	return false;
}

void CSVParser::close_file() {
	this->file_m.close();
}



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
	//std::cout << pos << std::endl;
	return line.length() > 0;
}

bool CSVParser::has_one_item(std::string line, std::string delimiter) {
	size_t pos = line.find(delimiter);
	return pos == std::string::npos;
}

std::string CSVParser::get_next_line() {
	std::string line;
	std::getline(this->file_m, line);
	return line;
}

bool CSVParser::has_next_line() {
	return this->file_m.peek() != EOF;
}