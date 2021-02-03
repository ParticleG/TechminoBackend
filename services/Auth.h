//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <utils/httpRequest.h>
#include <utils/httpResponse.h>
#include <models/Auth.h>

using namespace drogon_model;

namespace tech::services {
    class Auth {
    public:
        Auth();

        void updateAuthToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void getAuthToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void updateAccessToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

        void getAccessToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody);

    private:
        shared_ptr<orm::Mapper<Techmino::Auth>> authMapper;
    };
}