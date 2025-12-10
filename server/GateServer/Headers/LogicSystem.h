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

    bool HandleGet(std::string, std::shared_ptr<HttpConnection>);

    void RegisterPost(std::string, HttpHandler);

    bool HandlePost(std::string, std::shared_ptr<HttpConnection>);

private:
    LogicSystem();

    std::map<std::string, HttpHandler> _post_handlers;
    std::map<std::string, HttpHandler> _get_handlers;
};


#endif //GATESERVER_LOGICSYSTEM_H
