#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "ref_data.hpp"
#include "standard_observer.hpp"
#include "illuminants.hpp"
#include <unordered_map>

class DataManager {

public:
	static DataManager* get_instance() {
		if (nullptr == instance) {
			instance = new DataManager();
		}
		return instance;
	}

	void shut_down();

	RefData* get_ref_data(std::string key);
	StandardObserver* get_observer_1931();
	StandardObserver* get_observer_1964();
	Illuminants* get_illuminants();

private:
	DataManager() {};
	~DataManager();

	static DataManager* instance;
	StandardObserver* so1931 = nullptr;
	StandardObserver* so1964 = nullptr;
	Illuminants* illuminants = nullptr;
	std::unordered_map<std::string, RefData*> data_map;


};

#endif // !DATA_MANAGER_H