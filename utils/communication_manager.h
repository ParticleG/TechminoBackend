#pragma once

#include <trantor/net/TcpServer.h>

#include <drogon/drogon.h>
#include <drogon/PubSubService.h>
#include <json/json.h>

#include "crypto.h"

using namespace trantor;

namespace tech::utils {
    class CommunicationManager {
    public:
        void initAndStart(const uint16_t &port, const std::string &name, const size_t &ioLoopNum);

    private:
        TcpServer *_tcpServer;
    };
}


