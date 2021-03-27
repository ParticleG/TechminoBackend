//
// Created by Parti on 2021/2/16.
//

#include <services/App_WS.h>

using namespace tech::plugins;
using namespace drogon;
using namespace std;

tech::services::websocket::App::App() : Base(tech::utils::websocket::Type::App) {}

void tech::services::websocket::App::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    _app = make_shared<structures::App>(data["versionCode"].asInt());
    wsConnPtr->setContext(_app);

    auto initMessage = attributes->get<Json::Value>("content");
    initMessage["message"] = "Connected";

    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(26));
}

void tech::services::websocket::App::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        wsConnPtr->clearContext();
    }
}
