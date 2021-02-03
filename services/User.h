//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <utils/httpResponse.h>
#include <models/Auth.h>
#include <models/User.h>

using namespace drogon_model;

namespace tech::services {
    class User {
    public:
        void create(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void info(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void modify(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &body);

        void erase(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &body);

    private:
        bool _validate(HttpStatusCode &code,
                       Json::Value &body,
                       orm::Mapper<Techmino::Auth> &authMapper,
                       const std::string &email,
                       const std::string &authToken);
    };
}