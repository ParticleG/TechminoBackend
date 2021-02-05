//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <utils/Authorizer.h>
#include <services/Player.h>
#include <plugins/PlayManager.h>

namespace tech::services {
    class Play {
    public:
        bool validate(
                const WebSocketConnectionPtr &wsConnPtr,
                const std::string &email,
                const std::string &accessToken,
                const std::string &roomID,
                const std::string &roomPassword,
                CloseCode &code,
                std::string &reason
        );

        bool join(
                const WebSocketConnectionPtr &wsConnPtr,
                CloseCode &code,
                std::string &reason
        );

        void messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message);

        void quit(const WebSocketConnectionPtr &wsConnPtr);

    private:
        std::shared_ptr<Player> _player;
    };
}