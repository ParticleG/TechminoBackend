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

Chat::Chat() : Base(tech::strategies::actions::Prefix::chat) {}

void Chat::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    _chat = make_shared<structures::Chat>(data["uid"].asInt());
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
        initMessage["uid"] = id;
        initMessage["accessToken"] = auth.getValueOfAccessToken();
    }
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void Chat::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto chatManager = app().getPlugin<ChatManager>();
        auto rids = get<vector<string>>(wsConnPtr->getContext<structures::Chat>()->getRid());
        for (const auto &rid : rids) {
            try {
                chatManager->unsubscribe(rid, wsConnPtr);
            } catch (const exception &error) {
                LOG_WARN << "Unsubscribe failed at room: " << rid << ". Reason: " << error.what();
            }
        }
        wsConnPtr->clearContext();
    }
}

