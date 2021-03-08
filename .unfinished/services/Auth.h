//
// Created by Parti on 2021/2/4.
//

#pragma once

#include "models/Auth.h"
#include "utils/Http.h"

namespace tech::services::http {
    class Auth {
    public:
        Auth();

        static void updateAuthToken(
                const int64_t &id,
                const std::string &authToken,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        static void getAuthToken(
                const std::string &email,
                const std::string &password,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        static void updateAccessToken(
                const int64_t &id,
                const std::string &accessToken,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

        void getAccessToken(
                const int64_t &id,
                const std::string &authToken,
                drogon::HttpStatusCode &code,
                Json::Value &responseBody
        );

    private:
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::Auth>> authMapper;
    };
}