//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <models/Auth.h>

using namespace drogon;

namespace tech::utils {
    class authorizer {
    public:
        static bool accessToken(
                const std::string &email,
                const std::string &accessToken,
                HttpStatusCode &code,
                Json::Value &body
        );
        static bool authToken(
                const std::string &email,
                const std::string &authToken,
                HttpStatusCode &code,
                Json::Value &body
        );
        static bool password(
                const std::string &email,
                const std::string &password,
                HttpStatusCode &code,
                Json::Value &body
        );
    };
}