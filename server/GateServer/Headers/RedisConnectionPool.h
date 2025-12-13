#ifndef GATESERVER_REDISCONNECTIONPOOL_H
#define GATESERVER_REDISCONNECTIONPOOL_H

#include "const.h"

class RedisConnectionPool {
public:
    RedisConnectionPool(std::size_t size, const char *host, int port, const char *pwd);

    ~RedisConnectionPool();

    redisContext *getConnection();

    void returnConnection(redisContext *context);

    void Close();

private:
    std::atomic<bool> b_stop_;
    std::size_t size_;
    const char *host_;
    int port_;
    std::queue<redisContext *> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif //GATESERVER_REDISCONNECTIONPOOL_H
