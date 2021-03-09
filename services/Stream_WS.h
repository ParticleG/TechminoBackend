//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Base_WS.h>
#include <structures/Stream.h>
#include <utils/Authorizer.h>

namespace tech::services::websocket {
    class Stream : public Base {
    public:
        Stream();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::Attributes &attributes
        );

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        );

    private:
        std::shared_ptr<tech::structures::Stream> _stream;
    };
}