#ifndef REF_DATA_H
#define REF_DATA_H

#include <string>
#include <iostream>

class RefData {

public:
	RefData(std::string file_path);

private:
	void read_in_data(std::string file_path);
	
};
#endif //REF_DATA_H