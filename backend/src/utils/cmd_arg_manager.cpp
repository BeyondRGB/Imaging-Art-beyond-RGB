#include <utils/cmd_arg_manager.hpp>
#include <server/globals_singleton.hpp>

void CMDArgManager::process_args(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (is_optional_arg(arg)) {
            std::string option_items[2];
            bool success = pars_optional_arg(arg, option_items);
            std::string option_key = option_items[0];
            std::string option_value = option_items[1];
            set_option(option_key, option_value);
        }
        else {
            handle_other(arg);
        }

    }
}

bool CMDArgManager::pars_optional_arg(std::string arg, std::string option_values[], std::string delimiter) {
    size_t pos = 0;
    size_t arg_count = 0;
    std::string token;
    while ((pos = arg.find(delimiter)) != std::string::npos) {
        token = arg.substr(0, pos);
        arg.erase(0, pos + delimiter.length());
        if (arg_count < 2) {
            option_values[arg_count++] = token;
        }
    }
    if (arg_count < 2) {
        token = arg.substr(0, pos);
        option_values[arg_count] = token;
        return true;
    }
    return false;
}

bool CMDArgManager::is_optional_arg(std::string arg) {
    return arg[0] == '-' && arg[1] == '-';
}

void CMDArgManager::set_option(std::string key, std::string value) {
    if (key == "--test_run") {
        value = toLowerCase(value);
        if (value == "true") {
            GlobalsSingleton::get_instance()->set_is_test(true);
        }
    }
    if (key == "--app_root") {
        GlobalsSingleton::get_instance()->set_app_root(value);
    }
    if (key == "--port") {
        GlobalsSingleton::get_instance()->set_port(std::stoi(value));
    }
}

void CMDArgManager::handle_other(std::string arg) {
    if (arg == "-usage" || arg == "-u") {
        std::string usage_str =
            "usage: [options]\n"
            "\toptions:\n"
            "\t --test_run=<bool>: set true to bypass server and run testfunc() in main.cpp, this defaults to false\n"
            "\t --port=<int>: The local network port for frontend/backend communication.\n"
            "\t --app_root=<path>: set path for where applications resource folder can be found\n";
        std::cout << usage_str << std::endl;
    }
}

std::string CMDArgManager::toLowerCase(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}