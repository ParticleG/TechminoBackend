//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <models/App.h>
#include <models/Message.h>
#include <services/Player.h>
#include <utils/Authorizer.h>

namespace tech::services {
    class App {
    public:
        class _App {
        public:
            _App(const int &versionCode);

        private:
            int _versionCode;
            int64_t _id{};
            std::string authToken{}, accessToken{};
        };

    public:
        App();

        bool establish(const drogon::WebSocketConnectionPtr &wsConnPtr,
                       const int &versionCode,
                       drogon::CloseCode &code, Json::Value &response);

        void messageHandler(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const Json::Value &response
        );

    private:
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::App>> appMapper;
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::Message>> messageMapper;
    };
}
