//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <utils/Http.h>
#include <models/User.h>

using namespace drogon_model;

namespace tech::services {
    class User {
    public:
        User();

        void create(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void info(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void modify(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void erase(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

    private:
        shared_ptr<Mapper<Techmino::User>> userMapper;
    };
}