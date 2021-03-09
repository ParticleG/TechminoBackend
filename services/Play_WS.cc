//
// Created by Parti on 2021/2/5.
//

#include <plugins/PlayManager.h>
#include <services/Play_WS.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace tech::utils;
using namespace std;

Play::Play() : Base(WebSocket::Type::Play) {}

void Play::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const Attributes &attributes
) {
    auto data = attributes.get<Json::Value>("data");
    _play = make_shared<structures::Play>(data["id"].asInt());
    wsConnPtr->setContext(_play);

    Json::Value initMessage;
    initMessage["message"] = "Connected";
    WebSocket::initPing(wsConnPtr, data, chrono::seconds(10));
}

void Play::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto playManager = app().getPlugin<PlayManager>();
        auto sidsMap = *wsConnPtr->getContext<structures::Play>()->getSidsMap();
        for (const auto &pair : sidsMap) {
            try {
                playManager->unsubscribe(pair.first, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << pair.first << ". Reason: " << error.what();
            }
        }
        wsConnPtr->clearContext();
    }
}
