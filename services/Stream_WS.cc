//
// Created by Parti on 2021/2/5.
//

#include <plugins/StreamManager.h>
#include <services/Stream_WS.h>

using namespace drogon;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace tech::utils;
using namespace std;

Stream::Stream() : Base(WebSocket::Type::Stream) {}

void Stream::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const Attributes &attributes
) {
    auto data = attributes.get<Json::Value>("data");
    _stream = make_shared<structures::Stream>(data["id"].asInt());
    wsConnPtr->setContext(_stream);

    Json::Value initMessage;
    initMessage["message"] = "Connected";
    initMessage["data"] = data["connected"];
    WebSocket::initPing(wsConnPtr, data, chrono::seconds(10));

    auto rid = data["rid"].asString();
    try {
        app().getPlugin<StreamManager>()->subscribe(rid, wsConnPtr);
    } catch (const exception &error) {
        Json::Value response;
        response["message"] = error.what();
        WebSocket::close(wsConnPtr, CloseCode::kViolation, WebSocket::fromJson(response));
    }
}

void Stream::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto streamManager = app().getPlugin<StreamManager>();
        auto sidsMap = *wsConnPtr->getContext<structures::Stream>()->getSidsMap();
        for (const auto &pair : sidsMap) {
            try {
                streamManager->unsubscribe(pair.first, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << pair.first << ". Reason: " << error.what();
            }
        }
        wsConnPtr->clearContext();
    }
}
