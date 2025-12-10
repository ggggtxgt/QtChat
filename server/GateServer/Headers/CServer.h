#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include "const.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
    CServer(boost::asio::io_context &ioc, unsigned short &port);

    void Start();

private:
    tcp::acceptor _acceptor;
    net::io_context &_io_context;
    tcp::socket _socket;
};

#endif //GATESERVER_CSERVER_H
