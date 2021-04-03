//
// Created by Parti on 2021/2/23.
//

#include <services/Base_WS.h>

using namespace tech::services::websocket;
using namespace tech::plugins;
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
        response["type"] = "Warn";
        response["reason"] = "Wrong format: Requires int type 'action' in 'data'";
        return CloseCode::kNormalClosure;
    }
    auto action = request["action"].asInt();
    return _handlerManager->process(wsConnPtr, _type, action, request, response);
}

Base::Base(const tech::utils::websocket::Type &type) : _handlerManager(app().getPlugin<HandlerManager>()), _type(type) {}
