#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H

#include "const.h"

class HttpConnection;

typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem> {

    friend class Singleton<LogicSystem>;

public:
    ~LogicSystem();

    void RegisterGet(std::string, HttpHandler);

    // 为防止互相引用，并且LogicSystem能够访问到HttpConnection，应在LogicSystem.cpp中包含HttpConnection.h
    // 并且在HttpConnection之中添加友元类LogicSystem，且在HttpConnection.cpp之中包含LogicSystem.h
    bool HandleGet(std::string, std::shared_ptr<HttpConnection>);

    void RegisterPost(std::string, HttpHandler);

    bool HandlePost(std::string, std::shared_ptr<HttpConnection>);

private:
    LogicSystem();

    // post请求和get请求的回调函数map，key为路由，value为回调函数
    std::map<std::string, HttpHandler> _post_handlers;
    std::map<std::string, HttpHandler> _get_handlers;
};


#endif //GATESERVER_LOGICSYSTEM_H
