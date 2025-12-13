#include "../Headers/LogicSystem.h"
#include "../Headers/HttpConnection.h"

HttpConnection::HttpConnection(boost::asio::io_context &ioc) : _socket(ioc) {
}

tcp::socket &HttpConnection::GetSocket() {
    return _socket;
}

void HttpConnection::Start() {
    auto self = shared_from_this();
    // 调用http::async_read函数
    // 参数1：异步可读的数据流，可以理解为 socket
    // 参数2：用于存储接受的数据（由于可能存储文本、图像、音频，因此使用Dynamic动态类型的buffer）
    // 参数3：请求参数（一般需要传递能够接受多种资源类型的请求参数）
    // 参数4：回调函数（接受成功或失败，都将触发回调函数，使用Lambda表达式即可）
    http::async_read(_socket, _buffer, _request, [self](beast::error_code error, std::size_t bytes_transferred) {
        try {
            if (error) {
                std::cout << "http read error is" << error.what() << std::endl;
                return;
            }

            boost::ignore_unused(bytes_transferred);
            self->HandleRequest();
            self->CheckDeadline();
        }

        catch (std::exception &exp) {
            std::cout << "exception is " << exp.what() << std::endl;
        }
    });
}

// 十进制转换为十六进制
unsigned char ToHex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

// 十六进制转换为十进制
unsigned char FromHex(unsigned char x) {
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else
        assert(0);
    return y;
}

// url 编码
std::string UrlEncode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        //判断是否仅有数字和字母构成
        if (isalnum((unsigned char) str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ') //为空字符
            strTemp += "+";
        else {
            //其他字符需要提前加%并且高四位和低四位分别转为16进制
            strTemp += '%';
            strTemp += ToHex((unsigned char) str[i] >> 4);
            strTemp += ToHex((unsigned char) str[i] & 0x0F);
        }
    }
    return strTemp;
}

// url 解码
std::string UrlDecode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        //还原+为空
        if (str[i] == '+') strTemp += ' ';
            //遇到%将后面的两个字符从16进制转为char再拼接
        else if (str[i] == '%') {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char) str[++i]);
            unsigned char low = FromHex((unsigned char) str[++i]);
            strTemp += high * 16 + low;
        } else strTemp += str[i];
    }
    return strTemp;
}

void HttpConnection::PreParseGetParam() {
    // 提取 URI get_test?key1=value1&key2=value2
    auto uri = _request.target();
    // 查找查询字符串的开始位置（即 '?' 的位置）
    auto query_pos = uri.find('?');
    if (query_pos == std::string::npos) {
        _get_url = uri;
        return;
    }
    _get_url = uri.substr(0, query_pos);
    std::string query_string = uri.substr(query_pos + 1);

    // 提取key value
    std::string key;
    std::string value;
    size_t pos = 0;
    // 找到 & 标记
    while ((pos = query_string.find('&')) != std::string::npos) {
        auto pair = query_string.substr(0, pos);
        // 找到 =
        size_t eq_pos = pair.find('=');
        if (eq_pos != std::string::npos) {
            key = UrlDecode(pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码
            value = UrlDecode(pair.substr(eq_pos + 1));
            _get_params[key] = value;
        }
        query_string.erase(0, pos + 1);
    }

    // 处理最后一个参数对（如果没有 & 分隔符）
    if (!query_string.empty()) {
        size_t eq_pos = query_string.find('=');
        if (eq_pos != std::string::npos) {
            key = UrlDecode(query_string.substr(0, eq_pos));
            value = UrlDecode(query_string.substr(eq_pos + 1));
            _get_params[key] = value;
        }
    }
}

void HttpConnection::HandleRequest() {
    // 设置版本
    _response.version(_request.version());

    // 设置为短连接
    _response.keep_alive(false);

    // 处理 http get 请求
    if (http::verb::get == _request.method()) {

        PreParseGetParam();
        bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
        // 处理错误情况
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        // 处理正确情况
        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        WriteResponse();
        return;
    }

    // 处理 http post 请求
    if (http::verb::post == _request.method()) {

        bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
        // 处理错误情况
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        // 处理正确情况
        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        WriteResponse();
        return;
    }
}

void HttpConnection::WriteResponse() {
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    http::async_write(_socket, _response, [self](beast::error_code error, std::size_t bytes_transferred) {
        // 由于http为短连接。因此发送数据之后不需要监听对方连接，直接断开连接即可
        self->_socket.shutdown(tcp::socket::shutdown_send, error);
        // http请求具有时间约束，发送的数据包不能超时，因此发送时启动定时器，收到发送的回调之后取消定时器
        self->deadline_.cancel();
    });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();
    deadline_.async_wait([self](beast::error_code error) {
        // 客户端已超时，则关闭
        if (!error) {
            self->_socket.close(error);
        }
    });
}