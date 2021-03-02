//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Base_WS.h>
#include <structures/Chat.h>
#include <utils/Authorizer.h>

namespace tech::services::websocket {
    class Chat : public Base {
    public:
        Chat();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::Attributes &attributes
        );

    private:
        std::shared_ptr<tech::structures::Chat> _chat;
    };
}