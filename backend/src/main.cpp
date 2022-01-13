
#include <iostream>
#include "server/request_server.hpp"

int main() {
    std::cout << "BeyondRBG Backend Started: listening on port 9002\n";
    RequestServer server;
    server.init_server();
    server.start_server();

    //Block till server shuts down
    std::cout << "Server Shutting Down.\n";
    
}
