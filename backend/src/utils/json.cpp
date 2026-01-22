#include <utils/json.hpp>

Json::Json(std::string json_str) {
    std::cout << "Parsing Now" << std::endl;
    try {
        this->json_obj = jsoncons::json::parse(json_str);
    } catch (...) {
        std::cerr << "Unable to parse json string" << std::endl;
    }
}

Json::Json(jsoncons::json json_obj) { this->json_obj = json_obj; }

Json::Json(const Json &other) { this->json_obj = other.json_obj; }

bool Json::has(std::string key, Type type) {
    bool contains = this->json_obj.contains(key);
    bool is_type = false;
    if (contains) {
        jsoncons::json item = this->json_obj[key];
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
    return this->json_obj[key].as<bool>();
}

std::string Json::get_string(std::string key) {
    validate_is_obj();
    if (!this->has(key, Type::STRING)) {
        throw ParsingError("String('" + key + "') not found");
    }
    return this->json_obj[key].as<std::string>();
}

double Json::get_number(std::string key) {
    validate_is_obj();
    if (!this->has(key, Type::NUMBER)) {
        throw ParsingError("Number('" + key + "') not found");
    }
    return this->json_obj[key].as<double>();
}

Json Json::get_obj(std::string key) {
    validate_is_obj();
    if (!this->has(key, Type::OBJECT)) {
        throw ParsingError("Object('" + key + "') not found");
    }
    jsoncons::json obj = this->json_obj[key].as<jsoncons::json>();
    Json json(obj);
    return json;
}

Json Json::get_array(std::string key) {
    validate_is_obj();
    if (!this->has(key, Type::ARRAY)) {
        throw ParsingError("Array('" + key + "') not found");
    }
    jsoncons::json array = this->json_obj[key].as<jsoncons::json>();
    ;
    Json j(array);
    return j;
}

bool Json::bool_at(int index) {
    validate_is_array();
    validate_bounds(index);
    jsoncons::json item = this->json_obj[index];
    bool is_bool = this->get_type(item) == Type::BOOL;
    if (!is_bool) {
        throw ParsingError("Item at index(" + std::to_string(index) +
                           ") is not a bool");
    }
    return item.as<bool>();
}

std::string Json::string_at(int index) {
    validate_is_array();
    validate_bounds(index);
    jsoncons::json item = this->json_obj[index];
    bool is_string = this->get_type(item) == Type::STRING;
    if (!is_string) {
        throw ParsingError("Item at index(" + std::to_string(index) +
                           ") is not a string");
    }
    return item.as<std::string>();
}

double Json::number_at(int index) {
    validate_is_array();
    validate_bounds(index);
    jsoncons::json item = this->json_obj[index];
    bool is_number = this->get_type(item) == Type::NUMBER;
    if (!is_number) {
        throw ParsingError("Item at index(" + std::to_string(index) +
                           ") is not a number");
    }
    return item.as<double>();
}

Json Json::obj_at(int index) {
    validate_is_array();
    validate_bounds(index);
    jsoncons::json item = this->json_obj[index];
    bool is_obj = this->get_type(item) == Type::OBJECT;
    if (!is_obj) {
        throw ParsingError("Item at index(" + std::to_string(index) +
                           ") is not an obj");
    }
    Json j(item);
    return j;
}

Json Json::array_at(int index) {
    validate_is_array();
    validate_bounds(index);
    jsoncons::json item = this->json_obj[index];
    bool is_array = this->get_type(item) == Type::ARRAY;
    if (!is_array) {
        throw ParsingError("Item at index(" + std::to_string(index) +
                           ") is not an array");
    }
    Json j(item);
    return j;
}

int Json::get_size() { return this->json_obj.size(); }

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

Json::Type Json::get_type() { return get_type(this->json_obj); }

Json::Type Json::get_type(jsoncons::json item) {
    const jsoncons::json_type type = item.type();
    if (type == jsoncons::json_type::bool_value)
        return Type::BOOL;
    if (type == jsoncons::json_type::string_value)
        return Type::STRING;
    if (type == jsoncons::json_type::double_value ||
        type == jsoncons::json_type::uint64_value ||
        type == jsoncons::json_type::int64_value ||
        type == jsoncons::json_type::half_value)
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

jsoncons::json Json::get_jsoncons() { return this->json_obj; }
