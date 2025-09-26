#include <server/globals_singleton.hpp>

GlobalsSingleton* GlobalsSingleton::instance = nullptr;

GlobalsSingleton::~GlobalsSingleton() {
	if (nullptr == this->instance) {
		delete this->instance;
	}
}

GlobalsSingleton* GlobalsSingleton::get_instance() {
	if (nullptr == GlobalsSingleton::instance) {
		GlobalsSingleton::instance = new GlobalsSingleton();
	}
	return GlobalsSingleton::instance;
}

bool GlobalsSingleton::is_test() {
	return is_test_m;
}

std::string GlobalsSingleton::app_root() {
	return app_root_m;
}

void GlobalsSingleton::set_app_root(std::string app_root) {
	app_root_m = app_root;
}

void GlobalsSingleton::set_is_test(bool is_test) {
	is_test_m = is_test;
}

void GlobalsSingleton::set_port(int p) {
	this->port = p;
}

int GlobalsSingleton::get_port() {
	return this->port;
}

