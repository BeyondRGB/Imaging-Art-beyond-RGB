#ifndef GLOBALS_SINGLTON_H
#define GLOBALS_SINGLTON_H

#include <string>

class GlobalsSinglton {
public:
	~GlobalsSinglton();
	static GlobalsSinglton* get_instance();

	bool is_test();
	std::string app_root();

	void set_is_test(bool is_test);
	void set_app_root(std::string app_root);
protected:

private:
	GlobalsSinglton() {}
	static GlobalsSinglton* instance;
	bool is_test_m = false;
	std::string app_root_m = "./";

};

#endif // !GLOBALS_SINGLTON_H
