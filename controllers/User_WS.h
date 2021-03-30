//
// Created by Parti on 2021/2/16.
//

#pragma once

#include <controllers/Base_WS.h>
#include <services/User_WS.h>

namespace tech::socket::v1 {
    class User : public Base<User, tech::services::websocket::User> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/user", "tech::filters::User")
        WS_PATH_LIST_END
        User();
    };
}
