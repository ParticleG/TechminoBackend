//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <drogon/WebSocketController.h>

namespace tech::utils {
    class Authorizer {
    public:
        static bool accessToken(
                const std::string &email,
                const std::string &accessToken,
                drogon::CloseCode &code,
                std::string &reason
        );

        static bool accessToken(
                const std::string &email,
                const std::string &accessToken,
                drogon::HttpStatusCode &code,
                Json::Value &body
        );

        static bool authToken(
                const std::string &email,
                const std::string &authToken,
                drogon::HttpStatusCode &code,
                Json::Value &body
        );

        static bool password(
                const std::string &email,
                const std::string &password,
                drogon::HttpStatusCode &code,
                Json::Value &body
        );
    };
}