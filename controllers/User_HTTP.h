//
// Created by Parti on 2021/2/3.
//

#pragma once

#include <drogon/HttpController.h>
#include <services/User_HTTP.h>

namespace tech::api::v1 {
    class User : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::validateAccount, "/register", drogon::Post);
            METHOD_ADD(User::loginAccount, "/login", drogon::Post);
            METHOD_ADD(User::profileInfo, "/profile/info", drogon::Post);
            METHOD_ADD(User::updateInfo, "/profile/info", drogon::Put);
            METHOD_ADD(User::profileAvatar, "/profile/avatar", drogon::Post);
            METHOD_ADD(User::updateAvatar, "/profile/avatar", drogon::Put);
        METHOD_LIST_END

        void validateAccount(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void loginAccount(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void profileInfo(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void updateInfo(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void profileAvatar(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void updateAvatar(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    private:
        services::http::User _service;
    };
}
