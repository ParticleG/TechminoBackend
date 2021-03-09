//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <controllers/Base_WS.h>

namespace tech::socket::v1 {
    class Stream : public drogon::WebSocketController<Stream>, public Base {
    public:
        using Base::handleNewMessage;
        using Base::handleNewConnection;
        using Base::handleConnectionClosed;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/play", "tech::filters::Stream");
        WS_PATH_LIST_END

        Stream();
    };
}