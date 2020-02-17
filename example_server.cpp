/*
 * File: example_server.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Sunday, 16th February 2020 2:52:55 pm
 */


#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include <atomic>


#include <functional>
#include <thread>
#include <chrono>
#include <set>
#include <queue>


using server = websocketpp::server<websocketpp::config::asio>;

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
        m_endpoint.set_open_handler(std::bind(
          &utility_server::accept_handler, this,
          std::placeholders::_1
        ));

        m_endpoint.set_close_handler(std::bind(
          &utility_server::close_handler, this,
          std::placeholders::_1
        ));
    }

    void send_message(websocketpp::connection_hdl hdl, std::shared_ptr<asio::steady_timer> timer_ptr) {
      try {
        m_endpoint.send(hdl, "chat", websocketpp::frame::opcode::TEXT);
        timer_ptr->expires_at(timer_ptr->expiry() + std::chrono::seconds(1));
        timer_ptr->async_wait(std::bind(&utility_server::send_message, this, hdl, timer_ptr));
      } catch (const std::exception& e) {
        // timer_ptr->get_io_service()
        timer_ptr->get_io_service().post([=] {timer_ptr->cancel();});
      }
    }

    void close_handler(websocketpp::connection_hdl hdl) {
      // auto socket = m_endpoint.get_con_from_hdl(hdl)->get_socket();
      hdls.erase(hdl);
      timers[hdl]->get_io_service().post([=] {timers[hdl]->cancel();});
      // for (auto&  : hdls) {
      //   // auto s = m_endpoint.get_con_from_hdl(hdl)->get_socket().
      //   if ((websocketpp::connection_hdl)v == hdl) {
      //     std::cout << "found" << std::endl;
      //   }
      // }
    }

    void accept_handler(websocketpp::connection_hdl hdl) {
      m_endpoint.send(hdl, "hello", websocketpp::frame::opcode::value::TEXT);
      hdls.insert(hdl);
      // hdls[cnt] = hdl;
      cnt++;
      auto& io_context = m_endpoint.get_io_service();
      auto timer_ptr = std::make_shared<asio::steady_timer>(io_context, std::chrono::seconds(1));
      timers.insert({hdl, timer_ptr});
      timer_ptr->async_wait(std::bind(&utility_server::send_message, this, hdl, timer_ptr));
    }

    void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        // write a new message
        // m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
        for (const auto& v : hdls) {
          m_endpoint.send(v, msg->get_payload(), msg->get_opcode());
        }
    }

    void run() {
        // Listen on port 9002
        m_endpoint.listen(9002);

        // Queues a connection accept operation
        m_endpoint.start_accept();

        // Start the Asio io_service run loop
        m_endpoint.run();
        // websocketpp::lib::thread asio_thread(&server::run, &m_endpoint);
        // std::this_thread::sleep_for(std::chrono::seconds(100));
    }
private:
    server m_endpoint;
    // std::unordered_map<int, std::weak_ptr<void>> hdls;
    std::set<std::weak_ptr<void>, std::owner_less<std::weak_ptr<void>>> hdls;
    std::map<std::weak_ptr<void>, std::shared_ptr<asio::steady_timer>, std::owner_less<std::weak_ptr<void>>> timers;
    int cnt = 0;
};

int main() {
    utility_server s;
    std::atomic<int> a_s;
    std::thread t(&utility_server::run, &s);
    // std::this_thread::sleep_for(std::chrono::seconds(100));
    t.join();
    // s.run();
    return 0;
}
