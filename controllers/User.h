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
            METHOD_ADD(User::create, "", Post);
            METHOD_ADD(User::info, "", Get);
            METHOD_ADD(User::modify, "", Put);
            METHOD_ADD(User::erase, "", Delete);
        METHOD_LIST_END

        void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void modify(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void erase(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    private:
        tech::services::User _service;
    };
}