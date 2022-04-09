#include "globals_siglton.hpp"

GlobalsSinglton* GlobalsSinglton::instance = nullptr;

GlobalsSinglton::~GlobalsSinglton() {
	if (nullptr == this->instance) {
		delete this->instance;
	}
}

GlobalsSinglton* GlobalsSinglton::get_instance() {
	if (nullptr == GlobalsSinglton::instance) {
		GlobalsSinglton::instance = new GlobalsSinglton();
	}
	return GlobalsSinglton::instance;
}

bool GlobalsSinglton::is_test() {
	return is_test_m;
}

std::string GlobalsSinglton::app_root() {
	return app_root_m;
}

void GlobalsSinglton::set_app_root(std::string app_root) {
	app_root_m = app_root;
}

void GlobalsSinglton::set_is_test(bool is_test) {
	is_test_m = is_test;
}

void GlobalsSinglton::set_port(int p) {
	this->port = p;
}

int GlobalsSinglton::get_port() {
	return this->port;
}

