
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"

static bool is_test = false;

//bool pars_optional_arg(std::string arg, std::string option_values[], std::string delimiter = "=");
//bool is_optional_arg(std::string arg);
//void set_option(std::string kye, std::string value);
//void handle_other(std::string arg);

//std::string toLowerCase(std::string str) {
//    for (int i = 0; i < str.length(); i++) {
//        str[i] = tolower(str[i]);
//    }
//    std::cout << "to lower:" << str << std::endl;
//    return str;
//}

//void handle_args(int argc, char** argv) {
//    std::cout << "ArgC: " << argc << std::endl;
//    for (int i = 1; i < argc; i++) {
//        std::string arg = argv[i];
//        if (is_optional_arg(arg)) {
//            std::string option_items[2];
//            bool success = pars_optional_arg(arg, option_items);
//            std::string option_key = option_items[0];
//            std::string option_value = option_items[1];
//            std::cout << option_key << " -> " << option_value << std::endl;
//            set_option(option_key, option_value);
//        }
//        else {
//            handle_other(arg);
//        }
//        
//    }
//}

//void set_option(std::string key, std::string value) {
//    std::cout << "Setting Options: " << key << " -> " << value << std::endl;
//    if (key == "--test_run") {
//        value = toLowerCase(value);
//        std::cout << value << std::endl;
//         if (value == "true") {
//             is_test = true;
//        }
//    }
//    if (key == "--app_root") {
//        //TODO set pathe here
//        std::cout << "Got Path" << std::endl;
//    }
//}

//void handle_other(std::string arg) {
//    if (arg == "-usage" || arg == "-u") {
//        std::string usage_str = 
//            "usage: [options]\n"
//                "\toptions:\n"
//                "\t --test_run=<bool>: set true to bypass server and run testfunc() in main.cpp, this defaults to false\n"
//                "\t --app_root=<path>: set path for where applications resource folder can be found\n";
//        std::cout << usage_str << std::endl;
//    }
//}

//bool pars_optional_arg(std::string arg, std::string option_values[], std::string delimiter) {
//    size_t pos = 0;
//    size_t arg_count = 0;
//    std::string token;
//    while ((pos = arg.find(delimiter)) != std::string::npos) {
//        token = arg.substr(0, pos);
//        arg.erase(0, pos + delimiter.length());
//        if (arg_count < 2) {
//            option_values[arg_count++] = token;
//        }
//    }
//    if (arg_count < 2) {
//        token = arg.substr(0, pos);
//        option_values[arg_count] = token; 
//        return true;
//    }
//    return false;
//}

//bool is_optional_arg(std::string arg) {
//    return arg[0] == '-' && arg[1] == '-';
//}

//Testing Includes: Remove before submiting PR
void testFunc() {
    std::cout << "TestFunc" << std::endl;
    std::cout << "App_Root: " << GlobalsSinglton::get_instance()->app_root() << std::endl;;
}


int main(int argc, char** argv) {
    CMDArgManager::process_args(argc, argv);
	bool test = true; // Set to true if you want to test something and bypass the server
	if (GlobalsSinglton::get_instance()->is_test()) {
		testFunc();
		std::cout << "Exit" << std::endl;
		exit(1);
	}

    std::cout << "BeyondRBG Backend Started: listening on port 9002\n";
    RequestServer server;
    server.init_server();
    server.start_server();

    //Block till server shuts down
    std::cout << "Exiting.\n";
    delete GlobalsSinglton::get_instance();
    
}
