#ifndef CMD_ARG_Manager_H
#define CMD_ARG_Manager_H

#include <string>
#include <iostream>

class CMDArgManager {
public:
	CMDArgManager() {}
	static void process_args(int argc, char** argv);

private:
	static bool pars_optional_arg(std::string arg, std::string option_values[], std::string delimiter = "=");
	static bool is_optional_arg(std::string arg);
	static void set_option(std::string kye, std::string value);
	static void handle_other(std::string arg);
	static std::string toLowerCase(std::string str);


};

#endif // !CMD_ARG_Manager_H
