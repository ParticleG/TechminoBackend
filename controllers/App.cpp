//
// Created by Parti on 2021/2/16.
//

#include <controllers/App.h>
#include <utils/Http.h>
#include <utils/WebSocket.h>

using namespace tech::socket::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void App::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, string &&message,
                           const WebSocketMessageType &type) {
    if (type == WebSocketMessageType::Ping) {
        wsConnPtr->send(message, WebSocketMessageType::Pong);
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        Json::Value request, response;
        string parseError = WebSocket::toJson(message, request);
        if (!parseError.empty()) {
            response["message"] = "Wrong format";
            response["reason"] = parseError;
        } else {
            CloseCode code = _service.requestHandler(wsConnPtr, request, response);
            if (static_cast<int>(code)) {
                WebSocket::close(wsConnPtr, code, WebSocket::fromJson(response));
            } else {
                wsConnPtr->send(WebSocket::fromJson(response));
            }
        }
    } else if (type == WebSocketMessageType::Close) {
        wsConnPtr->forceClose();
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void App::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    CloseCode code;
    Json::Value requestBody, response;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        code = CloseCode::kWrongMessageContent;
        response["message"] = "Wrong format";
        response["reason"] = parseError;
        WebSocket::close(wsConnPtr, code, WebSocket::fromJson(response));
    } else {
        int versionCode = requestBody["version"].asInt();
        if (!_service.establish(wsConnPtr, versionCode, code, response)) {
            WebSocket::close(wsConnPtr, code, WebSocket::fromJson(response));
            return;
        }
        WebSocket::initPing(wsConnPtr, chrono::seconds(26));
    }
}

void App::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        wsConnPtr->clearContext();
    }
}

