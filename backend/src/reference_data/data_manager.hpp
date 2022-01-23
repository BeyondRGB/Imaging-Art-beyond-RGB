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
	double x_observer_value(int index);
	double y_observer_value(int index);
	double z_observer_value(int index);
	double illuminant_value(int index);
	

private:
	DataManager() {};
	~DataManager();
	// Default to D50
	Illuminants::IlluminantType illum_type = Illuminants::IlluminantType::D50;
	// Default to 1931
	StandardObserver::ObserverType observer_type = StandardObserver::ObserverType::SO_1931;

	static DataManager* instance;
	StandardObserver* so1931 = nullptr;
	StandardObserver* so1964 = nullptr;
	Illuminants* illuminants = nullptr;
	std::unordered_map<std::string, RefData*> data_map;

	StandardObserver* get_observer_1931();
	StandardObserver* get_observer_1964();
	Illuminants* get_illuminants();


};

#endif // !DATA_MANAGER_H