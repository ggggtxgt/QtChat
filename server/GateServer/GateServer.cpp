#include <iostream>

#include "./Headers/const.h"
#include "./Headers/CServer.h"
#include "./Headers/RedisManager.h"
#include "./Headers/ConfigManager.h"

void TestRedisManager() {
    assert(RedisManager::GetInstance()->Connect("81.68.86.146", 6380));
    assert(RedisManager::GetInstance()->Auth("123456"));
    assert(RedisManager::GetInstance()->Set("blogwebsite", "llfc.club"));
    std::string value = "";
    assert(RedisManager::GetInstance()->Get("blogwebsite", value));
    assert(RedisManager::GetInstance()->Get("nonekey", value) == false);
    assert(RedisManager::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
    assert(RedisManager::GetInstance()->HGet("bloginfo", "blogwebsite") != "");
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey2", value) == false);
    RedisManager::GetInstance()->Close();
}

int main() {
    TestRedisManager();
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