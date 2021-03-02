//
// Created by Parti on 2021/2/16.
//

#include <services/App_WS.h>

using namespace tech::services::websocket;
using namespace tech::plugins;
using namespace tech::utils;
using namespace drogon;
using namespace std;

App::App() : Base(WebSocket::Type::App) {}

void App::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const Attributes &attributes
) {
    auto data = attributes.get<Json::Value>("data");
    _app = make_shared<structures::App>(data["versionCode"].asInt());
    wsConnPtr->setContext(_app);

    auto content = attributes.get<Json::Value>("content");
    content["message"] = "Connected";

    WebSocket::initPing(wsConnPtr, content, chrono::seconds(26));
}
