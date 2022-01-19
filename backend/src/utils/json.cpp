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

//T Json::get_value(std::string key, Type type) {
//
//	switch (type) {
//	case Json::ARRAY:
//		return this->json_obj.get(key).as<jsoncons::json::array>();
//		break;
//	case Json::BOOL:
//		return this->json_obj.get(key).as<bool>();
//		break;
//	case Json::NUMBER:
//		return this->json_obj.get(key).as<double>();
//		break;
//	case Json::STRING:
//		return this->json_obj.get(key).as<std::string>();
//		break;
//	case Json::OBJECT:
//		jsoncons::json obj = this->json_obj.get(key).as<jsoncons::json>();
//		Json json(obj);
//		return json;
//		break;
//	}
//}

bool Json::has(std::string key, Type type) {
	bool contains = this->json_obj.contains(key);
	bool is_type = false;
	if (this->json_obj.contains(key)) {
		jsoncons::json item = this->json_obj.get(key);
		Type item_type = this->get_type(item);
		std::cout << "Type: " << item_type << std::endl;
		if (item_type == type || type == Type::ANNY) {
			is_type = true;
		}
		//switch (type)
		//{
		//case Json::ANNY:
		//	is_type = true;
		//	break;
		//case Json::ARRAY:
		//	is_type = item.is_object();
		//	break;
		//case Json::BOOL:
		//	is_type = item.is_bool();
		//	break;
		////case Json::INT:
		////	//is_type = item.is_i();
		////	break;
		//case Json::NUMBER:
		//	is_type = item.is_number();
		//	break;
		//case Json::STRING:
		//	is_type = item.is_string();
		//	break;
		//case Json::OBJECT:
		//	is_type = item.is_object();
		//	break;
		//	return is_type;
		//}
	}
	return is_type;
}

bool Json::get_bool(std::string key) {
	if (!this->has(key, Type::BOOL))
		throw ParsingError("Bool('" + key + "') not found");
	return this->json_obj.get(key).as<bool>();
}

bool Json::bool_at(int index) {
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::BOOL) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	return item.as<bool>();
}

std::string Json::get_string(std::string key) {
	if (!this->has(key, Type::STRING))
		throw ParsingError("String('" + key + "') not found");
	return this->json_obj.get(key).as<std::string>();
}

std::string Json::string_at(int index) {
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::STRING) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a string");
	}
	return item.as<std::string>();
}

double Json::get_number(std::string key) {
	if (!this->has(key, Type::NUMBER))
		throw ParsingError("Number('" + key + "') not found");
	return this->json_obj.get(key).as<double>();
}

double Json::number_at(int index) {
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::NUMBER) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	return item.as<double>();
}

Json Json::get_obj(std::string key) {
	if (!this->has(key, Type::OBJECT))
		throw ParsingError("Object('" + key + "') not found");
	jsoncons::json obj = this->json_obj.get(key).as<jsoncons::json>();
	Json json(obj);
	return json;
}

Json Json::obj_at(int index) {
	validate_bounds(index);
	jsoncons::json item = this->json_obj[index];
	if (!this->get_type(item) == Type::OBJECT) {
		throw ParsingError("Item at index(" + std::to_string(index) + ") is not a bool");
	}
	Json j(item);
	return j;
}

Json Json::get_array(std::string key) {
	if (!this->has(key, Type::ARRAY)) 
		throw ParsingError("Array('" + key + "') not found");
	jsoncons::json array = this->json_obj.get(key).as<jsoncons::json>();;
	Json j(array);
	return j;
}
//template <typename T>
//JsonArray<T> Json::get_array(std::string key) {
//	if (!this->has(key, Type::ARRAY))
//		throw ParsingError("Array('" + key + "') not found");
//	jsoncons::json array = this->json_obj.get(key);
//	const auto item = array[0];
//	std::cout << "Item: " << item << " Type: " << item.type() << std::endl;
//	
//	JsonArray<int> json_array(array);
//	int itemat = json_array.at(1);
//	std::cout << "ItemAt: " << itemat << std::endl;
//	return json_array;
//}

int Json::get_size() {
	return this->json_obj.size();
}

void Json::validate_bounds(int index) {
	if (index >= this->get_size())
		throw ParsingError("Index out of bounds");
}

Json::Type Json::get_type(std::string key) {
	if (!this->has(key, Type::ANNY))
		throw ParsingError(key + " not found");
	jsoncons::json item = this->json_obj[key];
	return this->get_type(item);
	//const jsoncons::json_type type = this->json_obj[key].type();
	//std::cout << "Json type: " << type << std::endl;
	//if (type == jsoncons::json_type::bool_value)
	//	return Type::BOOL;
	//if (type == jsoncons::json_type::string_value)
	//	return Type::STRING;
	//if (type == jsoncons::json_type::double_value)
	//	return Type::NUMBER;
	//if (type == jsoncons::json_type::array_value)
	//	return Type::ARRAY;
	//if (type == jsoncons::json_type::object_value)
	//	return Type::OBJECT;
	//return Type::NONE;
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
