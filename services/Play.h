//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <plugins/Configurator.h>

namespace tech::services {
    class Play {
    public:
        Play() : _configurator(app().getPlugin<tech::plugin::Configurator>()) {};

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
        shared_ptr<tech::plugin::Configurator> _configurator;
    };
}
