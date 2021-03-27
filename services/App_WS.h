//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <drogon/WebSocketController.h>
#include <plugins/HandlerManager.h>
#include <services/Base_WS.h>
#include <structures/App.h>

namespace tech::services::websocket {
    class App : public Base {
    public:
        App();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::AttributesPtr &attributes
        ) override;

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) override;

    private:
        std::shared_ptr<tech::structures::App> _app;
    };
}
