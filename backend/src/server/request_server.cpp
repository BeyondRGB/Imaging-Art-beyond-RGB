#include "request_server.hpp"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void RequestServer::init_server() {
    try {
        // Set logging settings
        server_m.set_access_channels(
            websocketpp::log::alevel::all);
        server_m.clear_access_channels(
            websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        server_m.init_asio();

        // Register our message handler
        server_m.set_message_handler(
            bind(&RequestServer::msg_handler, 
                 this, 
                 &server_m, 
                 ::_1, 
                 ::_2));

        // Listen on port 9002
        server_m.listen(port_m);
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void RequestServer::start_server() {
    try {
        // Start the server accept loop
        server_m.start_accept();

        // Start the ASIO io_service run loop
        server_m.run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void RequestServer::msg_handler(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "MSG: " << msg->get_payload() << std::endl;
    std::shared_ptr<CommunicationObj> coms_obj = std::shared_ptr<CommunicationObj>(new CommunicationObj( s, hdl, msg));
    this->process_manager_m.process_request(msg->get_payload(), coms_obj);

}


void RequestServer::shutdown() {
	//TODO look up to see if anything else needs to be done here
    server_m.stop_listening();
}