#ifndef BACKEND_PROCESS_H
#define BACKEND_PROCESS_H


#include "server/comunication_obj.hpp"
#include "server/communicator.hpp"

class BackendProcess: public Communicator{

public:
	virtual void run() = 0;
	std::string get_process_name() { return m_name; }
	void set_process_name(std::string name) { m_name = name; }
private:
	std::string m_name = "Undefined Process";
};

#endif // !BACKEND_PROCESS_H
