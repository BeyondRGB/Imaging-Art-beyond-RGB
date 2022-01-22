#ifndef REF_DATA_H
#define REF_DATA_H

#include <string>
//#include <iostream>
#include "csv_parser.hpp"

class RefData: public CSVParser {

public:
	RefData(std::string file_path);

private:
	void read_in_data(std::string file_path);
	void pars_line(std::string line, std::string delimiter);
	void pars_header(std::string header);
	
};
#endif //REF_DATA_H