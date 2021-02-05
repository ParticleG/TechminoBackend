//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <plugins/ChatManager.h>
#include <services/Player.h>
#include <utils/Authorizer.h>

namespace tech::services {
    class Chat {
    public:
        bool validate(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const std::string &email,
                const std::string &accessToken,
                const std::string &roomID,
                drogon::CloseCode &code,
                std::string &reason
        );

        bool join(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                drogon::CloseCode &code,
                std::string &reason
        );

        void messageHandler(const drogon::WebSocketConnectionPtr &wsConnPtr, const std::string &message);

        void quit(const drogon::WebSocketConnectionPtr &wsConnPtr);

    private:
        std::shared_ptr<Player> _player;
    };
}