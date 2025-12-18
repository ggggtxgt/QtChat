#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H

#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    friend class LogicSystem;

    HttpConnection(boost::asio::io_context &ioc);

    void Start();

    tcp::socket &GetSocket();

private:
    void CheckDeadline();

    void WriteResponse();

    void HandleRequest();

    void PreParseGetParam();

private:
    tcp::socket _socket;
    beast::flat_buffer _buffer{8192};               // 接收数据
    http::request<http::dynamic_body> _request;     // 解析请求
    http::response<http::dynamic_body> _response;   // 回应客户端
    // 定时器（判断请求是否超时）
    net::steady_timer deadline_{_socket.get_executor(), std::chrono::seconds(60)};

    std::string _get_url;
    std::unordered_map<std::string, std::string> _get_params;
};


#endif //GATESERVER_HTTPCONNECTION_H
