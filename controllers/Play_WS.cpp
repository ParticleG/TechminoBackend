//
// Created by Parti on 2021/2/5.
//

#include <controllers/Play_WS.h>
#include <utils/WebSocket.h>

using namespace tech::socket::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void Play::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, string &&message,
                            const WebSocketMessageType &type) {
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        _service.messageHandler(wsConnPtr, message);
    } else if (type == WebSocketMessageType::Pong) {
        LOG_DEBUG << "Message is Pong";
    } else if (type == WebSocketMessageType::Close) {
        LOG_DEBUG << "Message is Close";
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void Play::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    CloseCode code;
    string reason;

    string email = req->getParameter("email"),
            accessToken = req->getParameter("access_token"),
            roomID = req->getParameter("id"),
            password = req->getParameter("password");

    if ((!_service.validate(wsConnPtr, email, accessToken, roomID, password, code, reason)) ||
        (!_service.join(wsConnPtr, code, reason))) {
        wsConnPtr->send(reason);
        WebSocket::close(wsConnPtr, code, reason);
        return;
    }
}

void Play::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        _service.quit(wsConnPtr);
    }
}