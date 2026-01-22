#include <utils/csv_parser.hpp>

bool CSVParser::open_file(std::string file) {
    if (this->file_m.is_open())
        this->close_file();
    this->file_m.open(file);
    if (this->file_m.good())
        return true;
    return false;
}

void CSVParser::close_file() { this->file_m.close(); }

int CSVParser::get_line_count() {
    this->reset_file();
    int count = 0;
    while (this->has_next_line()) {
        this->get_next_line();
        count++;
    }
    this->reset_file();
    return count;
}

void CSVParser::reset_file() {
    this->file_m.clear();
    this->file_m.seekg(0, this->file_m.beg);
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
    return line.length() > 0;
}

bool CSVParser::has_one_item(std::string line, std::string delimiter) {
    size_t pos = line.find(delimiter);
    return pos == std::string::npos;
}

std::string CSVParser::get_next_line() {
    std::string line;
    std::getline(this->file_m, line);
    if (line.back() == '\r')
        line.pop_back();
    return line;
}

bool CSVParser::has_next_line() { return this->file_m.peek() != EOF; }

std::string CSVParser::peek() {
    // Store current position
    int current_pos = this->file_m.tellg();
    std::string line;
    line = this->get_next_line();
    // Return to sored postion
    this->file_m.seekg(current_pos, std::ios_base::beg);
    return line;
}
