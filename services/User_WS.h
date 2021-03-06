//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <services/Base_WS.h>
#include <structures/User.h>
#include <utils/Authorizer.h>

namespace tech::services::websocket {
    class User : public Base {
    public:
        User();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::Attributes &attributes
        );

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        );

    private:
        std::shared_ptr<tech::structures::User> _user;
    };
}
