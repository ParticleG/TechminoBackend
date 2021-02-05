//
// Created by Parti on 2021/2/5.
//

#include <utils/WebSocket.h>

using namespace tech::utils;
using namespace drogon;
using namespace std;

void WebSocket::close(
        const WebSocketConnectionPtr &webSocketConnectionPtr,
        CloseCode _code,
        const string &_reason
) {
    webSocketConnectionPtr->shutdown(_code, _reason);
}
