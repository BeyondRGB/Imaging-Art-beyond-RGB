#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ref_data.hpp"
#include <unordered_map>
class DataManager {

public:
	static DataManager* get_instance() {
		if (nullptr == instance) {
			instance = new DataManager();
		}
		return instance;
	}
	RefData* get_ref_data(std::string key);

private:
	DataManager() {};
	~DataManager() { delete instance; };

	static DataManager* instance;
	std::unordered_map<std::string, RefData*> data_map;

};

#endif // !DATA_MANAGER_H