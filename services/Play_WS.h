//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <plugins/PlayManager.h>
#include <services/Player.h>
#include <utils/Authorizer.h>

namespace tech::services {
    class Play {
    public:
        bool validate(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const std::string &email,
                const std::string &accessToken,
                const std::string &config,
                const std::string &roomID,
                const std::string &roomPassword,
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