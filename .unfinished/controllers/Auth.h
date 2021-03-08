//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include ".unfinished/services/Auth.h"

namespace tech::api::v1 {
    class Auth : public drogon::HttpController<Auth> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Auth::updateAuthToken, "/auth", drogon::Get);
            METHOD_ADD(Auth::getAuthToken, "/auth", drogon::Post);
            METHOD_ADD(Auth::updateAccessToken, "/access", drogon::Get);
            METHOD_ADD(Auth::getAccessToken, "/access", drogon::Post);
        METHOD_LIST_END

        void updateAuthToken(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void getAuthToken(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void updateAccessToken(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void getAccessToken(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    private:
        tech::services::http::Auth _service;
    };
}