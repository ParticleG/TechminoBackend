//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Chat_WS.h>

using namespace drogon;
namespace tech::socket::v1 {
    class Chat : public drogon::WebSocketController<Chat> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/online/chat");
        WS_PATH_LIST_END

        virtual void handleNewMessage(const WebSocketConnectionPtr &, std::string &&, const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

    private:
        tech::services::Chat _service;
    };
}