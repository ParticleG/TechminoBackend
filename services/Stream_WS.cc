//
// Created by Parti on 2021/2/5.
//

#include <plugins/StreamManager.h>
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
    wsConnPtr->setContext(make_shared<structures::Stream>(data["uid"].asInt()));

    Json::Value initMessage;
    initMessage["type"] = "Connect";
    initMessage["data"]["connected"] = data["connected"];
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
    LOG_DEBUG << "(" << GetCurrentThreadId() << ")[" << typeid(*this).name() <<"] After established: " << tech::utils::websocket::fromJson(initMessage);

    auto rid = data["rid"].asString();
    try {
        app().getPlugin<StreamManager>()->subscribe(rid, wsConnPtr);
    } catch (const exception &error) {
        Json::Value response;
        response["type"] = "Error";
        response["reason"] = error.what();
        tech::utils::websocket::close(wsConnPtr, CloseCode::kViolation, tech::utils::websocket::fromJson(response));
    }
}

void Stream::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto streamManager = app().getPlugin<StreamManager>();
        auto rid = get<string>(wsConnPtr->getContext<structures::Stream>()->getRid());
        LOG_DEBUG << "(" << GetCurrentThreadId() << ")[" << typeid(*this).name() <<"] Before closing: " << rid;
        if(!rid.empty()) {
            try {
                streamManager->unsubscribe(rid, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << rid << ". Reason: " << error.what();
            }
        }
        wsConnPtr->clearContext();
    }
}
