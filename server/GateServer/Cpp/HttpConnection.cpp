#include "../Headers/LogicSystem.h"
#include "../Headers/HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket socket) : _socket(std::move(socket)) {
}

void HttpConnection::Start() {
    auto self = shared_from_this();
    http::async_read(_socket, _buffer, _request, [self](beast::error_code error, std::size_t bytes_transferred) {
        try {
            if (error) {
                std::cout << "http read error is" << error.what() << std::endl;
                return;
            }

            boost::ignore_unused(bytes_transferred);
            self->HandleRequest();
            self->CheckDeadline();
        }

        catch (std::exception &exp) {
            std::cout << "exception is " << exp.what() << std::endl;
        }
    });
}

void HttpConnection::HandleRequest() {
    // 设置版本
    _response.version(_request.version());
    _response.keep_alive(false);

    // 处理 http get 请求
    if (http::verb::get == _request.method()) {
        bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
        // 处理错误情况
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        // 处理正确情况
        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        WriteResponse();
        return;
    }
}

void HttpConnection::WriteResponse() {
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    http::async_write(_socket, _response, [self](beast::error_code error, std::size_t bytes_transferred) {
        self->_socket.shutdown(tcp::socket::shutdown_send, error);
        self->deadline_.cancel();
    });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();
    deadline_.async_wait([self](beast::error_code error) {
        // 客户端已超时，则关闭
        if (!error) {
            self->_socket.close();
        }
    });
}