//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <models/App.h>
#include <models/Message.h>
#include <structures/Player.h>
#include <utils/Authorizer.h>

namespace tech::services::websocket {
    class App {
    public:
        App();

        bool establish(const drogon::WebSocketConnectionPtr &wsConnPtr,
                       const int &versionCode,
                       drogon::CloseCode &code, Json::Value &response);

        drogon::CloseCode requestHandler(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const Json::Value &request,
                Json::Value &response
        );

    private:
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::App>> appMapper;
        std::shared_ptr<drogon::orm::Mapper<drogon_model::Techmino::Message>> messageMapper;
    };
}
