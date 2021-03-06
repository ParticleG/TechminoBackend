//
// Created by Parti on 2021/2/5.
//

#include <models/Auth.h>
#include <plugins/Configurator.h>
#include <services/Chat_WS.h>
#include <utils/Crypto.h>
#include <utils/Utils.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::plugins;
using namespace tech::services::websocket;
using namespace tech::utils;
using namespace std;

Chat::Chat() : Base(WebSocket::Type::Chat) {}

void Chat::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const Attributes &attributes
) {
    auto data = attributes.get<Json::Value>("data");
    _chat = make_shared<structures::Chat>(data["id"].asInt());
    wsConnPtr->setContext(_chat);

    auto type = attributes.get<Authorizer::Type>("type");
    Json::Value initMessage;
    initMessage["message"] = "Connected";
    if (type == Authorizer::Type::GetAccessToken) {
        Mapper<Techmino::Auth> authMapper(app().getDbClient());
        auto id = _chat->getInfo()->getValueOfId();
        auto *configurator = app().getPlugin<Configurator>();
        auto auth = authMapper.findOne(Criteria(
                Techmino::Auth::Cols::__id, CompareOperator::EQ, id));
        auth.setAccessToken(Crypto::keccak(drogon::utils::getUuid()));
        auth.setAccessTokenExpireTime(Utils::fromDate(configurator->getAccessExpire()));
        initMessage["id"] = id;
        initMessage["accessToken"] = auth.getValueOfAccessToken();
    }
    WebSocket::initPing(wsConnPtr, initMessage, chrono::seconds(10));
}

void Chat::close(const WebSocketConnectionPtr &wsConnPtr) {

}

