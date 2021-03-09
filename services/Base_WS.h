//
// Created by Parti on 2021/2/23.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <plugins/HandlerManager.h>
#include <utils/WebSocket.h>

namespace tech::services::websocket {
    class Base {
    public:
        explicit Base(const tech::utils::WebSocket::Type &type);

        virtual void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::Attributes &attributes
        ) = 0;

        virtual void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) = 0;

        drogon::CloseCode requestHandler(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const Json::Value &request,
                Json::Value &response
        );

    private:
        tech::plugins::HandlerManager *_handlerManager;
        tech::utils::WebSocket::Type _type;
    };
}
