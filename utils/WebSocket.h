//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/WebSocketController.h>

namespace tech::utils {
    class WebSocket {
    public:
        static void close(
                const drogon::WebSocketConnectionPtr &webSocketConnectionPtr,
                drogon::CloseCode _code,
                const std::string &_reason
        );
    };
}