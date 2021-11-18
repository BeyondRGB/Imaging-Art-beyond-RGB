#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_ 

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <chrono>
#include <thread>

#include <iostream>

#include "pipeline.h"
#include "server/comunication_obj.h"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

static int pipeline_count = 0;

void startPipeline(CommunicationObj coms_obj) {
    Pipeline pipeline(coms_obj, ++pipeline_count);
    pipeline.executePipeline();
}

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    CommunicationObj coms_obj(s, hdl, msg);
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    if (msg->get_payload() == "processImg") {
        //Pipeline pipeline(s, hdl, msg);
        std::thread pipeline_thread(startPipeline, coms_obj);
        pipeline_thread.detach();
    }
    else {
        try {
            std::cout << "OpCode: " << msg->get_opcode();
            s->send(hdl, ("I got: " + msg->get_payload()), msg->get_opcode());
        }
        catch (websocketpp::exception const& e) {
            std::cout << "Echo failed because: "
                << "(" << e.what() << ")" << std::endl;
        }
    }
}


int main() {
    std::cout << "BeyondRBG Backend Started: listening on port 9002\n";
    // Create a server endpoint
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
