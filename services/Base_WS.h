//
// Created by Parti on 2021/2/23.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <plugins/HandlerManager.h>
#include <strategies/actions.h>
#include <utils/websocket.h>

namespace tech::services::websocket {
    class Base {
    public:
        explicit Base(const tech::strategies::actions::Prefix &prefix);

        virtual void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::AttributesPtr &attributes
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
        tech::strategies::actions::Prefix _prefix;
    };
}
