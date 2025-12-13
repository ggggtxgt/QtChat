#ifndef GATESERVER_REDISMANAGER_H
#define GATESERVER_REDISMANAGER_H

#include "const.h"

class RedisManager : public Singleton<RedisManager> {
    friend class Singleton<RedisManager>;

public:
    ~RedisManager();

    bool Connect(const std::string &host, int port);

    bool Get(const std::string &key, std::string &value);

    bool Set(const std::string &key, const std::string &value);

    bool Auth(const std::string &password);

    bool LPush(const std::string &key, const std::string &value);

    bool LPop(const std::string &key, std::string &value);

    bool RPush(const std::string &key, const std::string &value);

    bool RPop(const std::string &key, std::string &value);

    bool HSet(const std::string &key, const std::string  &hkey, const std::string &value);

    bool HSet(const char *key, const char *hkey, const char *hvalue, std::size_t hvaluelen);

    std::string HGet(const std::string &key, const std::string &hkey);

    bool Del(const std::string &key);

    bool ExistsKey(const std::string &key);

    void Close();

private:
    RedisManager();

private:
    redisContext *_connect;
    redisReply *_reply;
};

#endif //GATESERVER_REDISMANAGER_H
