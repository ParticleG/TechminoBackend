//
// Created by Parti on 2021/2/5.
//

#pragma once

#include "controllers/Base_WS.h"

namespace tech::socket::v1 {
    class Chat : public drogon::WebSocketController<Chat>, public Base {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/chat", "tech::filters::Chat");
        WS_PATH_LIST_END

        Chat();
    };
}