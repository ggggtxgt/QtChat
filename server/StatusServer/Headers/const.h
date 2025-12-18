#ifndef GATESERVER_CONST_H
#define GATESERVER_CONST_H

#include <map>
#include <mutex>
#include <queue>
#include <atomic>
#include <memory>
#include <cassert>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <condition_variable>

#include <hiredis.h>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Singleton.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>#include <boost/beast/http.hpp>

enum ErrorCodes {
    Success = 0,                // 成功
    Error_Json = 1001,          // JSON 解析错误
    RPCFailed = 1002,           // RCP 请求错误
    VarifyExpired = 1003,       // 验证码过期
    VarifyCodeErr = 1004,       // 验证码错误
    UserExist = 1005,           // 用户已经存在
    PasswordErr = 1006,         // 密码错误
    EmailNotMatch = 1007,       // 邮箱不匹配
    PasswordUpFailed = 1008,    // 更新密码失败
    PasswordInvalid = 1009,     // 密码错误
    TokenInvalid = 1010,        // Token 错误
    UidInvalid = 1011           // uid 错误
};

class Defer {
public:
    // 接受一个 lambda 表达式或者函数指针
    Defer(std::function<void()> func) : func_(func) {}

    // 析构函数中执行传入的函数
    ~Defer() {
        func_();
    }

private:
    std::function<void()> func_;
};

// 添加前缀
#define CODEPREFIX "code_"

#endif //GATESERVER_CONST_H
