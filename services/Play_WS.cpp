//
// Created by Parti on 2021/2/5.
//

#include "Play_WS.h"

using namespace tech::services;
using namespace tech::utils;
using namespace std;

bool Play::validate(
        const WebSocketConnectionPtr &wsConnPtr,
        const string &email,
        const string &accessToken,
        const string &roomID,
        const std::string &roomPassword,
        CloseCode &code, string &reason
) {
    if (!Authorizer::accessToken(email, accessToken, code, reason)) {
        return false;
    } else {
        auto playManager = app().getPlugin<tech::plugin::PlayManager>();
        try {
            if (!playManager->checkPassword(roomID, roomPassword)) {
                code = CloseCode::kInvalidMessage;
                reason = "Password is incorrect";
                return false;
            }
            _player = make_shared<Player>(email, roomID, roomPassword);
            wsConnPtr->setContext(_player);
        } catch (out_of_range &e) {
            LOG_ERROR << "error:" << e.what();
            code = CloseCode::kInvalidMessage;
            reason = "Room not found";
            return false;
        }
    }
    return true;
}

bool Play::join(
        const WebSocketConnectionPtr &wsConnPtr,
        CloseCode &code,
        string &reason
) {
    auto playManager = app().getPlugin<tech::plugin::PlayManager>();
    try {
        _player = wsConnPtr->getContext<Player>();
        _player->subscribe(playManager->subscribe(
                _player->getRoomID(),
                [wsConnPtr](const string &topic, const string &message) {
                    wsConnPtr->send(message);
                }
        ));
        playManager->publish(_player->getRoomID(),
                             "J" + _player->getUser().getValueOfUsername() + ":" +
                             to_string(_player->getUser().getValueOfId()));
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

void Play::messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message) {
    _player = wsConnPtr->getContext<Player>();
    auto playManager = app().getPlugin<tech::plugin::PlayManager>();
    char commandType = message[0];
#ifdef DEBUG_MODE
    std::cout << "[" << player.username << "#" << player._roomID << "]$" << commandType << " " << message.substr(1)
              << std::endl;
#endif
    switch (commandType) {
        case 'P':
#ifdef DEBUG_MODE
            std::cout << "[SERVER#0]$P" << std::endl;
#endif
            wsConnPtr->send("", WebSocketMessageType::Pong);
            break;
        case 'Q':
#ifdef DEBUG_MODE
            std::cout << "[SERVER#0]$Q " << player.username + ":" + std::to_string(player._roomID) << std::endl;
#endif
            wsConnPtr->forceClose();
            break;
        case 'B':
            playManager->publish(_player->getRoomID(),
                                 "B" + to_string(utils::Utils::uniform_random()));
            break;
        case 'T':
            playManager->publish(_player->getRoomID(),
                                 "T" + _player->getUser().getValueOfUsername() + ":" +
                                 to_string(_player->getUser().getValueOfId()) + ":" +
                                 message.substr(1));
            break;
        case 'C':
            playManager->publish(_player->getRoomID(),
                                 "C" + _player->getUser().getValueOfUsername() + ":" +
                                 to_string(_player->getUser().getValueOfId()) + ":" +
                                 message.substr(1),
                                 _player->getSubscriberID());
            break;
        case 'S':
            playManager->publish(_player->getRoomID(),
                                 "S" + to_string(_player->getUser().getValueOfId()) + ":" +
                                 message.substr(1),
                                 _player->getSubscriberID());
            break;
        default:
            playManager->publish(_player->getRoomID(), "EInvalid command.");
            break;
    }
}

void Play::quit(const WebSocketConnectionPtr &wsConnPtr) {
    auto playManager = app().getPlugin<tech::plugin::PlayManager>();
    _player = wsConnPtr->getContext<Player>();
    playManager->unsubscribe(_player->getRoomID(), _player->getSubscriberID());
    playManager->publish(_player->getRoomID(),
                         "L" + _player->getUser().getValueOfUsername() +
                         ":" + to_string(_player->getUser().getValueOfId()));
}