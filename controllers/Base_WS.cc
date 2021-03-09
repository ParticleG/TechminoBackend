//
// Created by Parti on 2021/2/23.
//

#include <controllers/Base_WS.h>

using namespace tech::socket::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void Base::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, string &&message,
                            const WebSocketMessageType &type) {
    if (type == WebSocketMessageType::Ping) {
        wsConnPtr->send(message, WebSocketMessageType::Pong);
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        Json::Value request, response;
        string parseError = websocket::toJson(message, request);
        if (!parseError.empty()) {
            response["message"] = "Wrong format";
            response["reason"] = parseError;
        } else {
            CloseCode code = _service->requestHandler(wsConnPtr, request, response);
            if (code == CloseCode::kNone) {
                wsConnPtr->send(websocket::fromJson(response));
            } else {
                websocket::close(wsConnPtr, code, websocket::fromJson(response));
            }
        }
    } else if (type == WebSocketMessageType::Close) {
        wsConnPtr->forceClose();
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    } else {
        LOG_DEBUG << "Message is: " << message;
    }
}

void Base::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    _service->establish(wsConnPtr, *req->getAttributes());
}

void Base::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    _service->close(wsConnPtr);
}