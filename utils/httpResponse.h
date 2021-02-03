//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace std;
using namespace drogon;

namespace tech::utils {
    class httpResponse {
    public:
        static void json(
                const HttpStatusCode &code,
                const Json::Value &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void plain(
                const HttpStatusCode &code,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void html(
                const HttpStatusCode &code,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );

        static void normal(
                const HttpStatusCode &code,
                const ContentType &type,
                const string &body,
                const function<void(const HttpResponsePtr &)> &callback
        );
    };
}