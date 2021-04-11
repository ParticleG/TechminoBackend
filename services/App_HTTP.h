//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <models/Auth.h>
#include <utils/http.h>

namespace tech::services::http {
    class App {
    public:
        App();

        Json::Value validateAccount(drogon::HttpStatusCode &code, const Json::Value &request);

    private:
        drogon::orm::Mapper<drogon_model::Techmino::Auth> _authMapper;
    };
}
