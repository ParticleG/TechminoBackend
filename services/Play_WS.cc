//
// Created by Parti on 2021/2/5.
//

#include <plugins/PlayManager.h>
#include <services/Play_WS.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace std;

Play::Play() : Base(tech::strategies::actions::Prefix::play) {}

void Play::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    _play = make_shared<structures::Play>(data["uid"].asInt());
    wsConnPtr->setContext(_play);

    Json::Value initMessage;
    initMessage["type"] = "Connect";
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void Play::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto playManager = app().getPlugin<PlayManager>();
        auto rid = get<string>(wsConnPtr->getContext<structures::Play>()->getRid());
        try {
            playManager->unsubscribe(rid, wsConnPtr);
        } catch (const exception &error) {
            LOG_WARN << "Unsubscribe failed at room: " << rid << ". Reason: " << error.what();
        }
        wsConnPtr->clearContext();
    }
}
