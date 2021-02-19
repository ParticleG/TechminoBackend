//
// Created by Parti on 2021/2/5.
//

#include <services/Play_WS.h>

using namespace tech::services::websocket;
using namespace tech::utils;
using namespace drogon;
using namespace std;

bool Play::validate(
        const WebSocketConnectionPtr &wsConnPtr,
        const string &email,
        const string &accessToken,
        const string &config,
        const string &roomID,
        const string &roomPassword,
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
            _player = make_shared<Player>(email, config, roomID, roomPassword);
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
        playManager->subscribe(
                _player->getRoomID(),
                [wsConnPtr](const string &topic, const string &message) {
                    wsConnPtr->send(message);
                }, _player
        );
        playManager->publish(_player->getRoomID(),
                             "J" + _player->getInfo(),
                             _player->getSubscriberID());
        wsConnPtr->send("J" + playManager->getInfos(_player->getRoomID()));
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
            wsConnPtr->send("", WebSocketMessageType::Pong);
            break;
        case 'Q':
            wsConnPtr->forceClose();
            break;
        case 'R':
            _player->setReadyState(true);
            playManager->publish(_player->getRoomID(),
                                 "R" + to_string(_player->getSubscriberID()));
            if (playManager->checkReadyState(_player->getRoomID())) {
                if (playManager->setReadyState(_player->getRoomID(), true)) {
                    auto tempPlayer = *_player;
                    thread([tempPlayer]() {
                        this_thread::sleep_for(chrono::seconds(3));
                        auto playManager = app().getPlugin<tech::plugin::PlayManager>();
                        if (playManager->checkReadyState(tempPlayer.getRoomID())) {
                            playManager->startGame(tempPlayer.getRoomID());
                            playManager->publish(tempPlayer.getRoomID(),
                                                 "B" + to_string(utils::Utils::uniform_random()));
                        }
                        this_thread::sleep_for(chrono::seconds(3));
                        playManager->setReadyState(tempPlayer.getRoomID(), false);
                    }).detach();
                }
            }
            break;
        case 'D':
            try {
                playManager->setDead(_player->getRoomID(), _player->getSubscriberID());
                auto winnerGroup = playManager->endGame(_player->getRoomID());
                auto tempPlayer = *_player;
                thread([tempPlayer, winnerGroup]() {
                    this_thread::sleep_for(chrono::seconds(3));
                    auto playManager = app().getPlugin<tech::plugin::PlayManager>();
                    playManager->publish(tempPlayer.getRoomID(),
                                         "F" + to_string(winnerGroup));
                }).detach();
            } catch (...) {}
            break;
        case 'T':
            playManager->publish(_player->getRoomID(),
                                 "T" + _player->getUser().getValueOfUsername() + ";" +
                                 to_string(_player->getUser().getValueOfId()) + ";" +
                                 message.substr(1));
            break;
        case 'C':
            _player->setConfig(message.substr(1));
            playManager->publish(_player->getRoomID(),
                                 "C" + _player->getUser().getValueOfUsername() + ";" +
                                 to_string(_player->getUser().getValueOfId()) + ";" +
                                 to_string(_player->getSubscriberID()) + ";" +
                                 message.substr(1),
                                 _player->getSubscriberID());
            break;
        case 'S':
            playManager->publish(_player->getRoomID(),
                                 "S" + to_string(_player->getSubscriberID()) + ";" +
                                 message.substr(1),
                                 _player->getSubscriberID());
            break;
        default:
            LOG_WARN << "Invalid WebSocket message in Play: " << message;
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
                         ";" + to_string(_player->getUser().getValueOfId()));
}