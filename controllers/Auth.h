//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <services/Auth.h>

namespace tech::api::v1 {
    class Auth : public drogon::HttpController<Auth> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Auth::updateAuthToken, "/auth", Get);
            METHOD_ADD(Auth::getAuthToken, "/auth", Post);
            METHOD_ADD(Auth::updateAccessToken, "/access", Get);
            METHOD_ADD(Auth::getAccessToken, "/access", Post);
        METHOD_LIST_END

        void updateAuthToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void getAuthToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void updateAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void getAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    private:
        tech::services::Auth _service;
    };
}