//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace std;
using namespace drogon;

namespace tech::utils {
    class Http {
    public:
        static void fromJson(
                const HttpStatusCode &code,
                const Json::Value &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void fromPlain(
                const HttpStatusCode &code,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void fromHtml(
                const HttpStatusCode &code,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void from(
                const HttpStatusCode &code,
                const ContentType &type,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static string toJson(
                const HttpRequestPtr &req,
                Json::Value &result
        );
    };
}