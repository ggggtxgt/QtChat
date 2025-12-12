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

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVarifyRsp GetVarifyCode(std::string email) {
        ClientContext context;
        GetVarifyRsp reply;
        GetVarifyReq request;
        request.set_email(email);

        Status status = stub_->GetVarifyCode(&context, request, &reply);
        if (status.ok()) {
            return reply;
        } else {
            reply.set_error(ErrorCodes::RPCFailed);
            return reply;
        }
    }

private:
    VerifyGrpcClient() {
        std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
        stub_ = VarifyService::NewStub(channel);
    }

    std::unique_ptr<VarifyService::Stub> stub_;
};

#endif //QTSERVER_VERIFYGRPCCLIENT_H
