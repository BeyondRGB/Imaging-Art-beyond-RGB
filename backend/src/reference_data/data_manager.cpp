#include "data_manager.hpp"

DataManager* DataManager::instance = nullptr;

DataManager::~DataManager() {
	if (nullptr != this->so1931) {
		delete this->so1931;
	}
	if (nullptr != this->so1964) {
		delete this->so1964;
	}
	if (nullptr != this->illuminants) {
		delete this->illuminants;
	}
	// Clean up RefData in data map
	// Although they will get deleted when the unordered_map destructor is called
	// It never is so do it here
	for (const auto& [key, data] : data_map) {
		delete data;
	}

}

void DataManager::set_observer(StandardObserver::ObserverType observer) {
	this->observer_type = observer;
}

void DataManager::set_illuminants(Illuminants::IlluminantType illuminants) {
	this->illum_type = illuminants;
}

void DataManager::shut_down() {
	if (nullptr != this->instance)
		delete this->instance;
}

RefData* DataManager::get_ref_data(std::string key) {
	if (!this->data_map.contains(key)) {
		//For now key is filename
		RefData* data = new RefData(key, this->illum_type, this->observer_type);
		this->data_map[key] =  data;
	}
	return this->data_map[key];
}

double DataManager::x_observer_value(int index) {
	StandardObserver::ValueType value_type = StandardObserver::ValueType::X;
	if (this->observer_type == StandardObserver::ObserverType::SO_1931) {
		return this->so1931->value_by_index(value_type, index);
	}
	else {
		return this->so1964->value_by_index(value_type, index);
	}
}

double DataManager::y_observer_value(int index) {
	StandardObserver::ValueType value_type = StandardObserver::ValueType::Y;
	if (this->observer_type == StandardObserver::ObserverType::SO_1931) {
		return this->get_observer_1931()->value_by_index(value_type, index);
	}
	else {
		return this->get_observer_1964()->value_by_index(value_type, index);
	}
}

double DataManager::z_observer_value(int index) {
	StandardObserver::ValueType value_type = StandardObserver::ValueType::Z;
	if (this->observer_type == StandardObserver::ObserverType::SO_1931) {
		return this->get_observer_1931()->value_by_index(value_type, index);
	}
	else {
		return this->get_observer_1964()->value_by_index(value_type, index);
	}
}

double DataManager::illuminant_value(int index) {
	return this->get_illuminants()->value_by_index(index);
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

Illuminants* DataManager::get_illuminants() {
	if (nullptr == this->illuminants) {
		this->illuminants = new Illuminants(this->illum_type);
	}
	return this->illuminants;
}