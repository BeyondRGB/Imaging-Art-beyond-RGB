#include "data_manager.hpp"

DataManager* DataManager::instance = nullptr;

//static DataManager* DataManager::get_instance() {
//	if (nullptr == instance) {
//		instance = new DataManager();
//	}
//	return instance;
//}

RefData* DataManager::get_ref_data(std::string key) {
	if (!this->data_map.contains(key)) {
		//For now key is filename
		RefData* data = new RefData(key);
		this->data_map[key] =  data;
	}
	return this->data_map[key];
}