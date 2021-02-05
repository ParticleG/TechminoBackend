//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/WebSocketController.h>

using namespace std;
using namespace drogon;

namespace tech::utils {
    class WebSocket {
    public:
        static void close(
                const WebSocketConnectionPtr &webSocketConnectionPtr,
                CloseCode _code,
                const std::string &_reason
        );
    };
}