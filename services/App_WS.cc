//
// Created by Parti on 2021/2/16.
//

#include <services/App_WS.h>

using namespace tech::plugins;
using namespace drogon;
using namespace std;

tech::services::websocket::App::App() : Base(tech::strategies::actions::Prefix::app) {}

void tech::services::websocket::App::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    wsConnPtr->setContext(make_shared<structures::App>());

    auto initMessage = data["content"];
    initMessage["type"] = "Connect";

    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(26));
}

void tech::services::websocket::App::close(const WebSocketConnectionPtr &wsConnPtr) {}
