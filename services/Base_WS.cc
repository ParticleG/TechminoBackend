//
// Created by Parti on 2021/2/23.
//

#include <services/Base_WS.h>
#include <utils/websocket.h>
#include <utils/misc.h>

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
    tech::utils::misc::logger(typeid(*this).name(), "New request: " + tech::utils::websocket::fromJson(request));
    return _handlerManager->process(wsConnPtr, _prefix, request["action"].asInt(), request, response);
}

Base::Base(const tech::strategies::actions::Prefix &prefix) :
        _handlerManager(app().getPlugin<HandlerManager>()), _prefix(prefix) {}
