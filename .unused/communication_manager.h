#pragma once

#include <trantor/net/TcpServer.h>

#include <drogon/drogon.h>
#include <drogon/PubSubService.h>
#include <json/json.h>

#include "Crypto.h"

using namespace trantor;

namespace tech::utils {
    struct Player {
        std::string _email{}, _accessToken{}, _name{}, _roomID{}, _roomPassword{};
        drogon::SubscriberID _id{}, _subscriberID = -1;
    };

    class CommunicationManager {
    public:
        void initAndStart(const uint16_t &connectionPtr, const std::string &name, const size_t &ioLoopNum);

    private:
        TcpServer *_tcpServer;

        static void _split(const std::string &source, std::vector<std::string> &outputs, const char &delimiter);

        static bool _authorization(const std::shared_ptr<Player> &player, const std::vector<std::string> &params,
                                   std::string &response);
    };
}


