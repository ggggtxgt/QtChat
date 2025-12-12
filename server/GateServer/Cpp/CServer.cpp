#include "../Headers/CServer.h"
#include "../Headers/HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port)
        : _io_context(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {
}

void CServer::Start() {
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](beast::error_code code) {
        try {
            // 若出错，则放弃该连接，继续监听其他连接
            if (code) {
                self->Start();
                return;
            }

            // 创建新连接，并且创建 HttpConnection 类管理的连接
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
            // 继续监听
            self->Start();
        }
        catch (std::exception &exp) {
        }
    });
}