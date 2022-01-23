#include "data_manager.hpp"

DataManager* DataManager::instance = nullptr;

DataManager::~DataManager() {
	if (nullptr != this->so1931) {
		delete this->so1931;
	}
	if (nullptr != this->so1964) {
		delete this->so1964;
	}
	// Clean up RefData in data map
	// Although they will get deleted when the unordered_map destructor is called
	// It never is so do it here
	for (const auto& [key, data] : data_map) {
		delete data;
	}

}

void DataManager::shut_down() {
	std::cout << "DataManger shutdown" << std::endl;
	if (nullptr != this->instance)
		delete this->instance;
}

RefData* DataManager::get_ref_data(std::string key) {
	if (!this->data_map.contains(key)) {
		//For now key is filename
		RefData* data = new RefData(key);
		this->data_map[key] =  data;
	}
	return this->data_map[key];
}

StandardObserver* DataManager::get_observer_1931() {
	if (nullptr == this->so1931) {
		this->so1931 = new StandardObserver(StandardObserver::ObserverType::SO_1931);
	}
	return this->so1931;
}

StandardObserver* DataManager::get_observer_1964() {
	if (nullptr == this->so1964) {
		this->so1964 = new StandardObserver(StandardObserver::ObserverType::SO_1964);
	}
	return this->so1964;
}