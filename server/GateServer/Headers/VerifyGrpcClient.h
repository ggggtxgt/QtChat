#ifndef QTSERVER_VERIFYGRPCCLIENT_H
#define QTSERVER_VERIFYGRPCCLIENT_H

#include <grpcpp/grpcpp.h>

#include "const.h"
#include "Singleton.h"
#include "../grpc/message.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConnectionPool {
public:
    RPConnectionPool(std::size_t, std::string, std::string);

    ~RPConnectionPool();

    void Close();

    std::unique_ptr<VarifyService::Stub> getConnection();

    void returnConnection(std::unique_ptr<VarifyService::Stub> connection);

private:
    std::atomic<bool> b_stop_;
    std::size_t size_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::condition_variable cond_;

    // @todo 使用锁使得队列线程安全（优化：可用并发编程中两把锁队列）
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVarifyRsp GetVarifyCode(std::string email);

private:
    VerifyGrpcClient();
    std::unique_ptr<RPConnectionPool> pool_;
};

#endif //QTSERVER_VERIFYGRPCCLIENT_H
