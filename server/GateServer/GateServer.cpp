#include <iostream>

#include "./Headers/CServer.h"
#include "./Headers/ConfigManager.h"

int main() {
    auto &gCfgMgr = ConfigManager::Inst();
    gCfgMgr["GateServer"]["Port"];
    std::string gate_port_str = gCfgMgr["GateServer"]["port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());
    try {
        unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop();
        });
        std::make_shared<CServer>(ioc, port)->Start();

        std::cout << "Gate Server listen on port: " << port << std::endl;

        ioc.run();
    }
    catch (std::exception &exp) {
        std::cerr << "Error: " << exp.what() << std::endl;
        return EXIT_FAILURE;
    }
}