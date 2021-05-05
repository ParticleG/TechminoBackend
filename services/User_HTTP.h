//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <models/Auth.h>
#include <models/Info.h>
#include <utils/http.h>

namespace tech::services::http {
    class User {
    public:
        User();

        Json::Value validateAccount(drogon::HttpStatusCode &code, const Json::Value &request);

        Json::Value loginAccount(
                drogon::HttpStatusCode &code,
                const Json::Value &request,
                std::vector<drogon::Cookie> &cookies
        );

        Json::Value profileInfo(drogon::HttpStatusCode &code, const Json::Value &request);

        Json::Value profileAvatar(drogon::HttpStatusCode &code, const Json::Value &request);

        Json::Value UpdateAvatar(drogon::HttpStatusCode &code, const Json::Value &request);

    private:
        drogon::orm::Mapper<drogon_model::Techmino::Auth> _authMapper;
        drogon::orm::Mapper<drogon_model::Techmino::Info> _infoMapper;
    };
}
