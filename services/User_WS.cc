//
// Created by Parti on 2021/2/16.
//

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
    _user = make_shared<structures::User>(data["uid"].asInt());
    wsConnPtr->setContext(_user);

    auto type = attributes->get<tech::utils::authorizer::Type>("type");
    Json::Value initMessage;
    initMessage["type"] = "Connect";
    if (type == tech::utils::authorizer::Type::GetAuthToken) {
        const auto auth = *_user->getAuth();
        initMessage["uid"] = auth.getValueOfId();
        initMessage["authToken"] = auth.getValueOfAuthToken();
    }
    tech::utils::websocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void User::close(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        wsConnPtr->clearContext();
    }
}
