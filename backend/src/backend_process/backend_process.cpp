#include <backend_process/backend_process.hpp>

unsigned int BackendProcess::pid = 0;

BackendProcess::BackendProcess(std::string name) {
    this->name = name + " (" + std::to_string(BackendProcess::pid++) + ")";
}
