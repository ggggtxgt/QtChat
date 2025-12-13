#include "../Headers/ConfigManager.h"
#include "../Headers/VerifyGrpcClient.h"

RPConnectionPool::RPConnectionPool(std::size_t size, std::string host, std::string port) :
        size_(size), host_(host), port_(port) {
    for (std::size_t i = 0; i < size_; i++) {
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
                                                               grpc::InsecureChannelCredentials());

        // 问题：为何能将 unique_ptr 的指针加入？？通过拷贝构造，还是移动构造？？
        // 为何 auto m = VarifyService::NewStub(channel); connection_.push(m); 不行？？
        connections_.push(VarifyService::NewStub(channel));
    }
}

RPConnectionPool::~RPConnectionPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    // 通知挂起的线程
    Close();
    while (!connections_.empty()) {
        connections_.pop();
    }
}

void RPConnectionPool::Close() {
    b_stop_ = true;
    cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() {
        if (b_stop_) {
            return true;
        }
        return !connections_.empty();
    });
    if (b_stop_) {
        return nullptr;
    }
    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void RPConnectionPool::returnConnection(std::unique_ptr<VarifyService::Stub> connection) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_) {
        return;
    }
    connections_.push(std::move(connection));
    cond_.notify_one();
}

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email) {
    ClientContext context;
    GetVarifyRsp reply;
    GetVarifyReq request;
    request.set_email(email);

    auto stub = pool_->getConnection();
    Status status = stub->GetVarifyCode(&context, request, &reply);
    if (status.ok()) {
        pool_->returnConnection(std::move(stub));
        return reply;
    } else {
        pool_->returnConnection(std::move(stub));
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

VerifyGrpcClient::VerifyGrpcClient() {
    auto &gCfgMgr = ConfigManager::Inst();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port = gCfgMgr["VarifyServer"]["Port"];
    pool_.reset(new RPConnectionPool(5, host, port));
}