//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <services/Base_WS.h>
#include <structures/Stream.h>
#include <utils/authorizer.h>

namespace tech::services::websocket {
    class Stream : public Base {
    public:
        Stream();

        void establish(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                const drogon::AttributesPtr &attributes
        ) override;

        void close(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) override;

    private:
        std::shared_ptr<tech::structures::Stream> _stream;
    };
}