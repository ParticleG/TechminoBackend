//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <controllers/Base_WS.h>
#include <services/Stream_WS.h>

namespace tech::socket::v1 {
    class Stream : public Base<Stream, tech::services::websocket::Stream> {
    public:
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/stream", "tech::filters::Stream")
        WS_PATH_LIST_END

        Stream();
    };
}