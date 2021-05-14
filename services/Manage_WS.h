//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <plugins/HandlerManager.h>
#include <services/Base_WS.h>

namespace tech::services::websocket {
    class Manage : public Base {
    public:
        Manage();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::AttributesPtr &attributes
        ) override;

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) override;
    };
}
