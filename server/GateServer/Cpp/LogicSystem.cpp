#include "../Headers/LogicSystem.h"
#include "../Headers/HttpConnection.h"

LogicSystem::LogicSystem() {
    RegisterGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test request\n";
        int i = 0;
        for (auto &elem: connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body())
                    << "param " << i << "  key is " << elem.first << "  value is "
                    << elem.second << std::endl;
        }
    });

    RegisterPost("/get_varify_code", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        connection->_response.set(http::field::content_type, "text/json");

        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        // key 值不存在
        if (!src_root.isMember("email")) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        auto email = src_root["email"].asString();
        std::cout << "email is " << email << std::endl;
        root["error"] = 0;
        root["email"] = src_root["email"];
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    });
}

LogicSystem::~LogicSystem() {

}

void LogicSystem::RegisterGet(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegisterPost(std::string url, HttpHandler handler) {
    _post_handlers.insert(make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
    if (_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }
    _get_handlers[path](connection);
    return true;
}


bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> connection) {
    if (_post_handlers.find(path) == _post_handlers.end()) {
        return false;
    }
    _post_handlers[path](connection);
    return true;
}