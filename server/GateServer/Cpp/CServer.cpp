#include "../Headers/CServer.h"
#include "../Headers/HttpConnection.h"
#include "../Headers/AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port)
        : _io_context(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {
}

void CServer::Start() {
    auto self = shared_from_this();
    auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
    _acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code code) {
        try {
            // 若出错，则放弃该连接，继续监听其他连接
            if (code) {
                self->Start();
                return;
            }

            // 创建新连接，并且创建 HttpConnection 类管理的连接
            new_con->Start();
            // 继续监听
            self->Start();
        }
        catch (std::exception &exp) {
        }
    });
}