//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <controllers/Base_WS.h>

namespace tech::socket::v1 {
    class App : public drogon::WebSocketController<App>, public Base {
    public:
        using Base::handleNewMessage;
        using Base::handleNewConnection;
        using Base::handleConnectionClosed;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/app", "tech::filters::App");
        WS_PATH_LIST_END

        App();
    };
}
