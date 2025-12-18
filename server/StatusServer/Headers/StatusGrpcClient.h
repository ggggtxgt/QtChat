#ifndef GATESERVER_STATUSGRPCCLIENT_H
#define GATESERVER_STATUSGRPCCLIENT_H

#include <grpcpp/grpcpp.h>

#include "const.h"
#include "Singleton.h"
#include "ConfigManager.h"
#include "../grpc/message.pb.h"
#include "../grpc/message.grpc.pb.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;

class StatusConPool {
public:
    StatusConPool(std::size_t size, std::string host, std::string port);

    ~StatusConPool();

    std::unique_ptr<StatusService::Stub> getConnection();

    void returnConnection(std::unique_ptr<StatusService::Stub> context);

    void Close();

private:
    std::size_t size_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<StatusService::Stub>> connections_;
};

class StatusGrpcClient : public Singleton<StatusGrpcClient> {
    friend class Singleton<StatusGrpcClient>;

public:
    ~StatusGrpcClient() = default;

    GetChatServerRsp GetChatServer(int uid);

    message::LoginRsp Login(int uid, std::string token);

private:
    StatusGrpcClient();

    std::unique_ptr<StatusConPool> pool_;
};

#endif //GATESERVER_STATUSGRPCCLIENT_H
