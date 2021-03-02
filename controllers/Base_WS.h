//
// Created by Parti on 2021/2/23.
//

#pragma once

#include <services/Base_WS.h>

namespace tech::socket::v1 {
    class Base : public drogon::WebSocketController<Base> {
    public:
        virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &, std::string &&, const drogon::WebSocketMessageType &) override;

        virtual void handleNewConnection(const drogon::HttpRequestPtr &, const drogon::WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr &) override;

    protected:
        std::shared_ptr<tech::services::websocket::Base> _service;
    };
}
