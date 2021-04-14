//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Base_WS.h>
#include <structures/Play.h>
#include <utils/authorizer.h>

namespace tech::services::websocket {
    class Play : public Base {
    public:
        Play();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::AttributesPtr &attributes
        ) override;

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) override;
    };
}