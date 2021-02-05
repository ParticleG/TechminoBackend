//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <plugins/PlayManager.h>

namespace tech::services {
    class Play {
    public:
        Play() : _playManager(app().getPlugin<tech::plugin::PlayManager>()) {};

        void list(
                const std::string &email,
                const std::string &accessToken,
                HttpStatusCode &code,
                Json::Value &responseBody
        );

        void info(
                const std::string &email,
                const std::string &accessToken,
                const std::string &roomType,
                HttpStatusCode &code,
                Json::Value &responseBody
        );

        void create(
                const std::string &email,
                const std::string &accessToken,
                const std::string &roomType,
                const std::string &roomName,
                const std::string &roomPassword,
                HttpStatusCode &code,
                Json::Value &responseBody
        );

    private:
        std::shared_ptr<tech::plugin::PlayManager> _playManager;
    };
}
