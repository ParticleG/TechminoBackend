//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <controllers/Base_WS.h>
#include <services/Manage_WS.h>

namespace tech::socket::v1 {
    class Manage : public Base<Manage, tech::services::websocket::Manage> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/monitor", "tech::filters::Manage")
        WS_PATH_LIST_END

        Manage();
    };
}
