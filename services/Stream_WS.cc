//
// Created by Parti on 2021/2/5.
//

#include <plugins/StreamManager.h>
#include <plugins/UserManager.h>
#include <services/Stream_WS.h>

using namespace drogon;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace std;

Stream::Stream() : Base(tech::strategies::actions::Prefix::stream) {}

void Stream::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    tech::utils::misc::logger(typeid(*this).name(), "Try setting context: " + to_string(data["uid"].asInt()));
    wsConnPtr->setContext(make_shared<structures::Stream>(data["uid"].asInt()));

    Json::Value initMessage;
    initMessage["type"] = "Connect";
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
    tech::utils::misc::logger(typeid(*this).name(),
                              "After established: " + tech::utils::websocket::fromJson(initMessage));

    auto srid = data["srid"].asString();
    try {
        tech::utils::misc::logger(typeid(*this).name(), "Try subscribing 'Stream': " + srid);
        app().getPlugin<StreamManager>()->subscribe(srid, wsConnPtr);
        app().getPlugin<UserManager>()->subscribe(
                wsConnPtr->getContext<structures::Stream>()->getUid(),
                wsConnPtr,
                UserManager::MapType::stream
        );
    } catch (const exception &error) {
        Json::Value response;
        response["type"] = "Error";
        response["reason"] = error.what();
        tech::utils::misc::logger(typeid(*this).name(),
                                  "Subscribe failed: " + tech::utils::websocket::fromJson(response));
        tech::utils::websocket::close(wsConnPtr, CloseCode::kViolation, tech::utils::websocket::fromJson(response));
    }
}

void Stream::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto streamManager = app().getPlugin<StreamManager>();
        auto rid = get<string>(wsConnPtr->getContext<structures::Stream>()->getRid());
        tech::utils::misc::logger(typeid(*this).name(), "Before closing: " + rid);
        if (!rid.empty()) {
            try {
                streamManager->unsubscribe(rid, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << rid << ". Reason: " << error.what();
            }
        }
    }
    app().getPlugin<UserManager>()->unsubscribe(
            wsConnPtr->getContext<structures::Stream>()->getUid(),
            UserManager::MapType::stream
    );
}
