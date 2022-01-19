//#ifndef JSONARRAY_H
//#define JSONARRAY_H
//
//#include <jsoncons/json.hpp>
//#include <utils/json.hpp>
//
//
//template <typename T>
//class JsonArray {
//
//public:
//	JsonArray(jsoncons::json json, int size);
//
//
//	T at(int index);
//	int size();
//
//private:
//	T *array;
//	//jsoncons::json array;
//	int size_m;
//	//bool is_json(T type);
//	//Json get_obj(int index);
//
//};
//
//template <typename T>
//JsonArray<T>::JsonArray(jsoncons::json json, int size) {
//	this->size_m = json.size();
//	//this->array = json;
//	array = new T[size];
//	/*for (int i = 0; i < size; i++) {
//		jsoncons::json item = json.at(i);
//		if (std::is_same<T, class Json>()) {
//			T obj(item);
//			array
//		}
//	}*/
//}
//
//template <typename T>
//T JsonArray<T>::at(int index) {
//	//jsoncons::json item = this->array.at(index);
//	//if (std::is_same<T, class Json>()) {
//	//	std::cout << "Found Json Object In Array" << std::endl;
//	//	//jsoncons::json obj = item.as<jsoncons::json>();
//	//	class Json j(item);
//	//	return j;
//	//}
//	std::cout << "Found other In Array" << std::endl;
//	return this->array.at(index).as<T>();
//
//	/*jsoncons::json item = this->array.at(index);
//	const jsoncons::json_type type = item.type();
//	if (type == jsoncons::json_type::bool_value)
//		return item.as<bool>();
//
//	if (type == jsoncons::json_type::string_value) {
//		std::string str = item.as<std::string>();
//		return str;
//	}
//
//	if (type == jsoncons::json_type::double_value)
//		return item.as<double>();*/
//
//	/*if (type == jsoncons::json_type::array_value || 
//		type == jsoncons::json_type::object_value) {
//		jsoncons::json obj = item.as<jsoncons::json>();
//		Json j(5);
//		return j;
//	}*/
//	/*if (type == jsoncons::json_type::object_value)
//		return Type::OBJECT;
//	else {
//		return this->array.at(index).as<T>();
//	}*/
//}
//
//template <typename T>
//int JsonArray<T>::size() {
//	return this.size_m;
//}
//#endif // !JSONARRAY_H