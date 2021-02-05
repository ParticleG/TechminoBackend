//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Play_WS.h>

namespace tech::socket::v1 {
    class Play : public drogon::WebSocketController<Play> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/online/play");
        WS_PATH_LIST_END

        virtual void handleNewMessage(const drogon::WebSocketConnectionPtr &,
                                      std::string &&,
                                      const drogon::WebSocketMessageType &) override;

        virtual void handleNewConnection(const drogon::HttpRequestPtr &,
                                         const drogon::WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr &) override;

    private:
        tech::services::Play _service;
    };
}