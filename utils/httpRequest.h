//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace std;
using namespace drogon;

namespace tech::utils {
    class httpRequest {
    public:
        static string toJson(const HttpRequestPtr &req, Json::Value &result);
    };
}