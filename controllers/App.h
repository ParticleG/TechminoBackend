//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <services/App.h>

namespace tech::socket::v1 {
    class App : public drogon::WebSocketController<App> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/app");
        WS_PATH_LIST_END

        virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &, std::string &&, const drogon::WebSocketMessageType &) override;

        virtual void handleNewConnection(const drogon::HttpRequestPtr &, const drogon::WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr &) override;

    private:
        tech::services::App _service;
    };
}
