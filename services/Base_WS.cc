//
// Created by Parti on 2021/2/23.
//

#include <services/Base_WS.h>

using namespace tech::services::websocket;
using namespace tech::plugins;
using namespace tech::utils;
using namespace drogon;
using namespace std;

CloseCode Base::requestHandler(
        const WebSocketConnectionPtr &wsConnPtr,
        const Json::Value &request,
        Json::Value &response
) {
    if (!(
            request.isMember("action") && request["action"].isInt()
    )) {
        response["message"] = "Wrong format";
        response["reason"] = "Requires int type 'action' in 'data'";
        return CloseCode::kNone;
    }
    auto action = request["action"].asInt();
    return _handlerManager->process(wsConnPtr, _type, action, request, response);
}

Base::Base(const WebSocket::Type &type) : _handlerManager(app().getPlugin<HandlerManager>()), _type(type) {}
