//#include "json_array.hpp"

//bool JsonArray::is_json(T type) {
//	return std::is_same<type, Json>::value;
//}
//
//Json JsonArray::get_obj(int index) {
//	jsoncons::json item = this->array.at(index);
//	Json j(item);
//	return j;
//}
////template <typename T>
////JsonArray<T>::JsonArray(jsoncons::json json) {
////	this.size_m = json.size();
////	this->array = json;
////	/*this.array = new T[this.size_m];
////	for (int i = 0; i < this.size_m; i++) {
////		T item = json[i];
////		std::cout << "Item: " << item << " Type: " << item.type() << std::endl;
////		this.array[i] = item;
////	}*/
////}
//
//template <typename T>
//T JsonArray<T>::at(int index) {
//	return this.array[index];
//}
//template <typename T>
//int JsonArray<T>::size() {
//	return this.size_m;
//}