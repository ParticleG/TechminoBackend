//
// Created by Parti on 2021/2/4.
//

#pragma once

#include "models/Info.h"
#include "utils/Http.h"

namespace tech::services::http {
    class User {
    public:
        User();

        void create(
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        void info(
                const std::string &email,
                const std::string &authToken,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        void modify(
                const std::string &email,
                const std::string &authToken,
                const std::string &username,
                const std::string &motto,
                const std::string &avatar,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        void erase(
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

    private:
        std::shared_ptr<Mapper<drogon_model::Techmino::Info>> userMapper;
    };
}