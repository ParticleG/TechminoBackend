//
// Created by Parti on 2021/2/5.
//

#include "WebSocket.h"

using namespace tech::utils;

void WebSocket::close(
        const WebSocketConnectionPtr &webSocketConnectionPtr,
        CloseCode _code,
        const std::string &_reason
) {
    webSocketConnectionPtr->shutdown(_code, _reason);
}
