/*
 * File: example_xviz_server.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Monday, 17th February 2020 3:16:09 pm
 */

#include "server/xviz_server.h"
#include "server/xviz_handler.h"
#include "server/xviz_session.h"

using namespace xviz;

class LiveSession : public XVIZBaseSession {
public:
  LiveSession(std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> conn_ptr) : XVIZBaseSession(conn_ptr) {}
  void OnConnect() override {
    conn_ptr_->send("hello");
  }
  void Main() override {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      auto err_code = conn_ptr_->send("chat");
      if (err_code) {
        throw websocketpp::exception("loss connection");
      }
    }
  }
  void OnDisconnect() override {
    conn_ptr_->send("byebye");
  }
};

class LiveHandler : public XVIZBaseHandler {
public:
  LiveHandler() : XVIZBaseHandler() {}
  std::shared_ptr<XVIZBaseSession> GetSession(const std::unordered_map<std::string, std::string>& params,
    std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> conn_ptr) override {
    return std::make_shared<LiveSession>(conn_ptr);
  }
};

int main() {
  std::vector<std::shared_ptr<xviz::XVIZBaseHandler>> handlers;
  handlers.push_back(std::make_shared<LiveHandler>());
  xviz::XVIZServer server(handlers);
  server.Serve();
}