/*
 * File: xviz_server.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Monday, 17th February 2020 12:40:44 pm
 */


#include "server/xviz_server.h"

using namespace xviz;

XVIZServer::XVIZServer(const std::vector<std::shared_ptr<XVIZBaseHandler>>& handlers, uint16_t port, 
    int max_payload, bool per_message_defalte) :
      handlers_(handlers), port_(port), max_payload_{max_payload}, per_message_deflate_(per_message_defalte) {
  InitInternalServer();
}
XVIZServer::XVIZServer(std::vector<std::shared_ptr<XVIZBaseHandler>>&& handlers, uint16_t port, 
    int max_payload, bool per_message_defalte) :
      handlers_(std::move(handlers)), port_(port), max_payload_{max_payload}, per_message_deflate_(per_message_defalte) {
  InitInternalServer();
}


void XVIZServer::InitInternalServer() {
  internal_server_ptr_ = std::make_shared<websocketpp::server<websocketpp::config::asio>>();

  internal_server_ptr_->set_error_channels(websocketpp::log::elevel::none);
  internal_server_ptr_->set_access_channels(websocketpp::log::alevel::none);
  internal_server_ptr_->set_reuse_addr(true);
  internal_server_ptr_->init_asio();

  internal_server_ptr_->set_open_handler(std::bind(
    &XVIZServer::HandleSession, this, std::placeholders::_1
  ));

  internal_server_ptr_->listen(port_);
  internal_server_ptr_->start_accept();
}


void XVIZServer::HandleSession(websocketpp::connection_hdl hdl) {
  auto conn = internal_server_ptr_->get_con_from_hdl(hdl);
  auto query = conn->get_uri()->get_query();
  auto params = ParseParameters(query);

  for (const auto& handler_ptr : handlers_) {
    auto session_ptr = handler_ptr->GetSession(params, conn);
    if (session_ptr) {
      std::thread t(std::bind(
        &XVIZServer::SessionThread, this, session_ptr
      ));
      t.detach();
      break;
    }
  }
}

void XVIZServer::SessionThread(std::shared_ptr<XVIZBaseSession> session_ptr) {
  LOG_DEBUG("start a thread to handle session");
  try {
    session_ptr->OnConnect();
    session_ptr->Main();
    session_ptr->OnDisconnect();
  } catch (const websocketpp::exception& e) {
    LOG_WARNING("%s", e.what());
  }
  LOG_DEBUG("end a thread to handle session");
}

void XVIZServer::Serve() {
  internal_server_ptr_->run();
}

std::unordered_map<std::string, std::string> XVIZServer::ParseParameters(const std::string& uri) {
  // TODO implement this
  return std::unordered_map<std::string, std::string>();
}