//
// Created by Parti on 2021/2/5.
//

#include <database/Auth.h>
#include <plugins/ChatManager.h>
#include <plugins/Configurator.h>
#include <plugins/UserManager.h>
#include <services/Chat_WS.h>
#include <utils/crypto.h>

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
    wsConnPtr->setContext(make_shared<structures::Chat>(data["uid"].asInt()));

    auto type = attributes->get<tech::utils::authorizer::Type>("type");
    Json::Value initMessage;
    initMessage["type"] = "Connect";
    if (type == tech::utils::authorizer::Type::GetAccessToken) {
        auto id = wsConnPtr->getContext<structures::Chat>()->getInfo().getValueOfId();
        auto *configurator = app().getPlugin<Configurator>();
        auto auth = Auth().retrieveAuthById(id);
        auth.setAccessToken(crypto::keccak(drogon::utils::getUuid()));
        auth.setAccessTokenExpireTime(misc::fromDate(configurator->getAccessExpire()));
        initMessage["uid"] = id;
        initMessage["accessToken"] = auth.getValueOfAccessToken();
    }
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
    app().getPlugin<UserManager>()->subscribe(
            wsConnPtr->getContext<structures::Chat>()->getInfo().getValueOfId(),
            wsConnPtr,
            UserManager::MapType::chat
    );
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
    }
    app().getPlugin<UserManager>()->unsubscribe(
            wsConnPtr->getContext<structures::Chat>()->getInfo().getValueOfId(),
            UserManager::MapType::chat
    );
}

