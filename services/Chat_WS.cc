//
// Created by Parti on 2021/2/5.
//

#include <database/Auth.h>
#include <plugins/ChatManager.h>
#include <plugins/Configurator.h>
#include <services/Chat_WS.h>
#include <utils/crypto.h>
#include <utils/misc.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::database;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace tech::utils;
using namespace std;

Chat::Chat() : Base(tech::utils::websocket::Type::Chat) {}

void Chat::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    _chat = make_shared<structures::Chat>(data["id"].asInt());
    wsConnPtr->setContext(_chat);

    auto type = attributes->get<tech::utils::authorizer::Type>("type");
    Json::Value initMessage;
    initMessage["type"] = "Connect";
    if (type == tech::utils::authorizer::Type::GetAccessToken) {
        auto id = _chat->getInfo()->getValueOfId();
        auto *configurator = app().getPlugin<Configurator>();
        auto auth = Auth().retrieveAuthById(id);
        auth.setAccessToken(crypto::keccak(drogon::utils::getUuid()));
        auth.setAccessTokenExpireTime(misc::fromDate(configurator->getAccessExpire()));
        initMessage["id"] = id;
        initMessage["accessToken"] = auth.getValueOfAccessToken();
    }
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void Chat::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto chatManager = app().getPlugin<ChatManager>();
        auto sidsMap = wsConnPtr->getContext<structures::Chat>()->getSidsMap();
        for (const auto &pair : sidsMap) {
            try {
                chatManager->unsubscribe(pair.first, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << pair.first << ". Reason: " << error.what();
            }
        }
        wsConnPtr->clearContext();
    }
}

