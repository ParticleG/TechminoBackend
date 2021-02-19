//
// Created by Parti on 2021/2/5.
//

#include <services/Chat_WS.h>

using namespace tech::services::websocket;
using namespace tech::utils;
using namespace drogon;
using namespace std;

bool Chat::join(
        const WebSocketConnectionPtr &wsConnPtr,
        CloseCode &code,
        string &reason
) {
    auto chatManager = app().getPlugin<tech::plugin::ChatManager>();
    try {
        _player = wsConnPtr->getContext<Player>();
        chatManager->joinChat(
                [wsConnPtr](const std::string &message) {
                    wsConnPtr->send(message);
                }, _player
        );
        chatManager->chat("J" + _player->getUser().getValueOfUsername() +
                          ";" + to_string(_player->getUser().getValueOfId()) +
                          ";" + to_string(chatManager->chatCount()));
        wsConnPtr->setContext(_player);
    } catch (range_error &e) {
        LOG_ERROR << "error:" << e.what();
        code = CloseCode::kInvalidMessage;
        reason = "Room is full";
        return false;
    } catch (out_of_range &e) {
        LOG_ERROR << "error:" << e.what();
        code = CloseCode::kInvalidMessage;
        reason = "Room is not available for now";
        return false;
    }
    return true;
}

void Chat::messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message) {
    char commandType = message[0];
    if (commandType == 'P') {
        wsConnPtr->send("", WebSocketMessageType::Pong);
    } else if (commandType == 'Q') {
        wsConnPtr->forceClose();
    } else if (commandType == 'T') {
        _player = wsConnPtr->getContext<Player>();
        auto chatManager = app().getPlugin<tech::plugin::ChatManager>();
        chatManager->chat("T" + _player->getUser().getValueOfUsername() + ";" +
                          to_string(_player->getUser().getValueOfId()) + ";" +
                          message.substr(1));
    } else {
        LOG_WARN << "Invalid WebSocket message in Chat: " << message;
        auto chatManager = app().getPlugin<tech::plugin::ChatManager>();
        chatManager->chat("EInvalid command.");
    }
}

bool Chat::validate(
        const WebSocketConnectionPtr &wsConnPtr,
        const string &email,
        const string &accessToken,
        const string &config,
        const string &roomID,
        CloseCode &code, string &reason
) {
    if (!Authorizer::accessToken(email, accessToken, code, reason)) {
        return false;
    } else {
        _player = make_shared<Player>(email, config, roomID);
        wsConnPtr->setContext(_player);
    }
    return true;
}

void Chat::quit(const WebSocketConnectionPtr &wsConnPtr) {
    auto chatManager = app().getPlugin<tech::plugin::ChatManager>();
    _player = wsConnPtr->getContext<Player>();
    chatManager->quitChat(_player->getSubscriberID());
    chatManager->chat("L" + _player->getUser().getValueOfUsername() +
                      ";" + to_string(_player->getUser().getValueOfId()) +
                      ";" + to_string(chatManager->chatCount()));
}
