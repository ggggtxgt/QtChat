#include "../Headers/RedisConnectionPool.h"

RedisConnectionPool::RedisConnectionPool(std::size_t size, const char *host, int port, const char *pwd) :
        size_(size), host_(host), port_(port), b_stop_(false) {

    // 创建连接
    for (std::size_t i = 0; i < size_; i++) {
        auto *context = redisConnect(host, port);
        if (context == nullptr || context->err != 0) {
            // 若连接出错，则释放该连接
            if (context != nullptr) {
                redisFree(context);
            }
            continue;
        }

        // 每一个连接成功之后，都需要验证密码
        auto reply = (redisReply *) redisCommand(context, "AUTH %s", pwd);
        if (reply->type == REDIS_REPLY_ERROR) {
            std::cout << "认证失败" << std::endl;
            // 执行成功，释放 redisCommand 执行之后返回的 redisReply 所占用的内存
            freeReplyObject(reply);
            redisFree(context);
            continue;
        }

        // 执行成功，释放 redisCommand 执行之后返回的 redisReply 所占用的内存
        freeReplyObject(reply);
        std::cout << "认证成功" << std::endl;
        connections_.push(context);
    }
}

RedisConnectionPool::~RedisConnectionPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (!connections_.empty()) {
        connections_.pop();
    }
}

redisContext *RedisConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] {
        if (b_stop_) {
            return true;
        }
        return !connections_.empty();
    });

    // 如果停止则直接返回空指针
    if (b_stop_) {
        return nullptr;
    }
    auto *context = connections_.front();
    connections_.pop();
    return context;
}

void RedisConnectionPool::returnConnection(redisContext *context) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_) return;
    connections_.push(context);
    cond_.notify_one();
}

void RedisConnectionPool::Close() {
    b_stop_ = true;
    cond_.notify_all();
}