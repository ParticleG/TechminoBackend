//
// Created by Parti on 2021/2/16.
//

#include <plugins/UserManager.h>
#include <services/User_WS.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace std;

User::User() : Base(tech::strategies::actions::Prefix::user) {}

void User::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const AttributesPtr &attributes
) {
    auto data = attributes->get<Json::Value>("data");
    wsConnPtr->setContext(make_shared<structures::User>(data["uid"].asInt()));

    auto type = attributes->get<tech::utils::authorizer::Type>("type");
    Json::Value initMessage;
    initMessage["type"] = "Connect";
    if (type == tech::utils::authorizer::Type::GetAuthToken) {
        auto auth = wsConnPtr->getContext<structures::User>()->getAuth();
        initMessage["uid"] = auth.getValueOfId();
        initMessage["authToken"] = auth.getValueOfAuthToken();
    }
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
    app().getPlugin<UserManager>()->subscribe(
            wsConnPtr->getContext<structures::User>()->getAuth().getValueOfId(),
            wsConnPtr,
            UserManager::MapType::user
    );
}

void User::close(const WebSocketConnectionPtr &wsConnPtr) {
    app().getPlugin<UserManager>()->unsubscribe(
            wsConnPtr->getContext<structures::User>()->getAuth().getValueOfId(),
            UserManager::MapType::user
    );
}
