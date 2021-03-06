//
// Created by Parti on 2021/2/23.
//

#pragma once

#include <services/Base_WS.h>

namespace tech::socket::v1 {
    class Base {
    public:
        virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &, std::string &&, const drogon::WebSocketMessageType &);

        virtual void handleNewConnection(const drogon::HttpRequestPtr &, const drogon::WebSocketConnectionPtr &);

        virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr &);

    protected:
        std::shared_ptr<tech::services::websocket::Base> _service;
    };
}
