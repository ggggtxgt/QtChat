#include "../Headers/LogicSystem.h"
#include "../Headers/HttpConnection.h"

LogicSystem::LogicSystem() {
    Register("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test request";
    });
}

LogicSystem::~LogicSystem() {

}

void LogicSystem::Register(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
    if (_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }
    _get_handlers[path](connection);
    return true;
}