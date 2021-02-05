//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace tech::utils {
    class Http {
    public:
        static void fromJson(
                const drogon::HttpStatusCode &code,
                const Json::Value &body,
                const std::function<void(const drogon::HttpResponsePtr &)> &callback
        );

        static void fromPlain(
                const drogon::HttpStatusCode &code,
                const std::string &body,
                const std::function<void(const drogon::HttpResponsePtr &)> &callback
        );

        static void fromHtml(
                const drogon::HttpStatusCode &code,
                const std::string &body,
                const std::function<void(const drogon::HttpResponsePtr &)> &callback
        );

        static void from(
                const drogon::HttpStatusCode &code,
                const drogon::ContentType &type,
                const std::string &body,
                const std::function<void(const drogon::HttpResponsePtr &)> &callback
        );

        static std::string toJson(
                const drogon::HttpRequestPtr &req,
                Json::Value &result
        );
    };
}