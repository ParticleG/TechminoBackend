//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <services/User.h>

namespace tech::api::v1 {
    class User : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::create, "", drogon::Post);
            METHOD_ADD(User::info, "", drogon::Get);
            METHOD_ADD(User::modify, "", drogon::Put);
            METHOD_ADD(User::erase, "", drogon::Delete);
        METHOD_LIST_END

        void create(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void info(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void modify(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void erase(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    private:
        tech::services::User _service;
    };
}