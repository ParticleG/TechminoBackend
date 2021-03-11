//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <controllers/Base_WS.h>
#include <services/Chat_WS.h>

namespace tech::socket::v1 {
    class Chat : public Base<Chat, tech::services::websocket::Chat> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/chat", "tech::filters::Chat");
        WS_PATH_LIST_END
    };
}