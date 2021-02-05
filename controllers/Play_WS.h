//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Play_WS.h>

using namespace drogon;
namespace tech::socket::v1 {
    class Play : public drogon::WebSocketController<Play> {
    public:
        virtual void handleNewMessage(const WebSocketConnectionPtr &,
                                      std::string &&,
                                      const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &,
                                         const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/online/play");
        WS_PATH_LIST_END

    private:
        tech::services::Play _service;
    };
}