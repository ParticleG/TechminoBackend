//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <models/App.h>
#include <models/Message.h>
#include <utils/http.h>

namespace tech::services::http {
    class App {
    public:
        App();

        void info(drogon::HttpStatusCode &code, Json::Value &body);

    private:
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::App>> appMapper;
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::Message>> messageMapper;
    };
}
