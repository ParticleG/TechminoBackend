//
// Created by Parti on 2021/2/16.
//

#include <services/Manage_WS.h>

using namespace tech::plugins;
using namespace drogon;
using namespace std;

tech::services::websocket::Manage::Manage() : Base(tech::strategies::actions::Prefix::manage) {}

void tech::services::websocket::Manage::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    Json::Value initMessage;
    initMessage["type"] = "Connect";

    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void tech::services::websocket::Manage::close(const WebSocketConnectionPtr &wsConnPtr) {}
