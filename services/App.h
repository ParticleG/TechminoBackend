//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <utils/Http.h>
#include <models/App.h>
#include <models/Message.h>

using namespace drogon_model;

namespace tech::services {
    class App {
    public:
        App();

        void info(HttpStatusCode &code, Json::Value &body);
    private:
        shared_ptr<orm::Mapper<Techmino::App>> appMapper;
        shared_ptr<orm::Mapper<Techmino::Message>> messageMapper;
    };
}
