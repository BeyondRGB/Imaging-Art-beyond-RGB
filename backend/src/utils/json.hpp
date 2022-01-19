#ifndef JSON_H
#define JSON_H
#include <jsoncons/json.hpp>
#include "json_array.hpp"



class Json {


public:
	enum Type {
		ANNY,
		ARRAY,
		BOOL,
		NUMBER,
		NONE,
		OBJECT,
		STRING
	};

	Json(std::string json_str);


	bool has(std::string key, Type type = Type::ANNY);
	
	bool get_bool(std::string key);
	std::string get_string(std::string key);
	Json get_obj(std::string key);
	double get_number(std::string key);
	//template <typename T>
	//JsonArray<T> get_array(std::string key);
	Json get_array(std::string key);

	double number_at(int index);
	std::string string_at(int index);
	Json obj_at(int index);
	bool bool_at(int index);

	std::string to_string(bool should_beutify = false);

	int get_size();
	Json::Type get_type(std::string key);


private:
	Json(jsoncons::json json_obj);
	jsoncons::json json_obj;

	Json::Type get_type(jsoncons::json item);
	void validate_bounds(int index);

};

class JsonError : public std::exception {};

class ParsingError : public JsonError {
public:
	ParsingError();
	ParsingError(std::string error_msg) { error = "Error: " + error_msg; }
	virtual char const* what() const noexcept { return error.c_str(); }
private:
	std::string error = "Undefined Json Parsing Error";
};

//template <typename T>
//JsonArray<T> Json::get_array(std::string key) {
//	if (!this->has(key, Type::ARRAY))
//		throw ParsingError("Array('" + key + "') not found");
//	jsoncons::json array = this->json_obj.get(key);
//	const auto item = array[0];
//	std::cout << "Item: " << item << " Type: " << item.type() << std::endl;
//
//	JsonArray<T> json_array(array);
//	return json_array;
//}
#endif // !JSON_H