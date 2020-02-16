/*
 * File: example_server.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Sunday, 16th February 2020 2:52:55 pm
 */


#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

class utility_server {
public:
    utility_server() {
         // Set logging settings
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        m_endpoint.init_asio();

        // Set the default message handler to the echo handler
        m_endpoint.set_message_handler(std::bind(
            &utility_server::echo_handler, this,
            std::placeholders::_1, std::placeholders::_2
        ));
    }

    void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        // write a new message
        m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
    }

    void run() {
        // Listen on port 9002
        m_endpoint.listen(9002);

        // Queues a connection accept operation
        m_endpoint.start_accept();

        // Start the Asio io_service run loop
        m_endpoint.run();
    }
private:
    server m_endpoint;
};

int main() {
    utility_server s;
    s.run();
    return 0;
}
