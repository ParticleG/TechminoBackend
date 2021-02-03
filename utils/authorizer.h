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
        static bool authToken(
                const std::string &email,
                const std::string &authToken,
                HttpStatusCode &code,
                Json::Value &body
        );
    };
}