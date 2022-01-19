#include "json.hpp"

Json::Json(std::string json_str) {
	std::cout << "Parsing Now" << std::endl;
	try {
		this->json_obj = jsoncons::json::parse(json_str);
	}
	catch (...) {
		std::cerr << "Unable to parse json string" << std::endl;
	}
}

Json::Json(jsoncons::json json_obj) {
	this->json_obj = json_obj;
}

bool Json::has(std::string key, Type type) {
	bool contains = this->json_obj.contains(key);
	bool is_type = false;
	if (contains) {
		jsoncons::json item = this->json_obj.get(key);
		Type item_type = this->get_type(item);
		if (item_type == type || type == Type::ANNY) {
			is_type = true;
		}
	}
	return is_type;
}

bool Json::get_bool(std::string key) {
	validate_is_obj();
	if (!this->has(key, Type::BOOL)) {
		throw ParsingError("Bool('" + key + "') not found");
	}
	return this->json_obj.get(key).as<bool>();
}

bool Json::bool_at(int index) {
	validate_is_array();
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::BOOL) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	return item.as<bool>();
}

std::string Json::get_string(std::string key) {
	validate_is_obj();
	if (!this->has(key, Type::STRING)) {
		throw ParsingError("String('" + key + "') not found");
	}
	return this->json_obj.get(key).as<std::string>();
}

std::string Json::string_at(int index) {
	validate_is_array();
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::STRING) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a string");
	}
	return item.as<std::string>();
}

double Json::get_number(std::string key) {
	validate_is_obj();
	if (!this->has(key, Type::NUMBER)) {
		throw ParsingError("Number('" + key + "') not found");
	}
	return this->json_obj.get(key).as<double>();
}

double Json::number_at(int index) {
	validate_is_array();
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::NUMBER) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	return item.as<double>();
}

Json Json::get_obj(std::string key) {
	validate_is_obj();
	if (!this->has(key, Type::OBJECT)) {
		throw ParsingError("Object('" + key + "') not found");
	}
	jsoncons::json obj = this->json_obj.get(key).as<jsoncons::json>();
	Json json(obj);
	return json;
}

Json Json::obj_at(int index) {
	validate_is_array();
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::OBJECT) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	Json j(item);
	return j;
}

Json Json::get_array(std::string key) {
	validate_is_obj();
	if (!this->has(key, Type::ARRAY)) {
		throw ParsingError("Array('" + key + "') not found");
	}
	jsoncons::json array = this->json_obj.get(key).as<jsoncons::json>();;
	Json j(array);
	return j;
}

int Json::get_size() {
	return this->json_obj.size();
}

void Json::validate_bounds(int index) {
	if (index >= this->get_size()) {
		throw ParsingError("Index out of bounds");
	}
}

void Json::validate_is_array() {
	if (this->get_type() != Json::Type::ARRAY) {
		throw ParsingError("Current Item is not a Json Array");
	}
}

void Json::validate_is_obj() {
	if (this->get_type() != Json::Type::OBJECT) {
		throw ParsingError("Current Item is not a Json Object");
	}
}

Json::Type Json::get_type(std::string key) {
	if (!this->has(key, Type::ANNY)) {
		throw ParsingError(key + " not found");
	}
	jsoncons::json item = this->json_obj[key];
	return this->get_type(item);
}

Json::Type Json::get_type() {
	return get_type(this->json_obj);
}

Json::Type Json::get_type(jsoncons::json item) {
	const jsoncons::json_type type = item.type();
	if (type == jsoncons::json_type::bool_value)
		return Type::BOOL;
	if (type == jsoncons::json_type::string_value)
		return Type::STRING;
	if (type == jsoncons::json_type::double_value)
		return Type::NUMBER;
	if (type == jsoncons::json_type::array_value)
		return Type::ARRAY;
	if (type == jsoncons::json_type::object_value)
		return Type::OBJECT;
	return Type::NONE;
}

std::string Json::to_string(bool should_beutify) {
	std::string str;
	if (should_beutify)
		this->json_obj.dump_pretty(str);
	else
		this->json_obj.dump(str);
	return str;
}
