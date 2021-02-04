#include <plugins/tech_plugin_VersusManager.h>
#include "tech_socket_v1.h"

#define DEBUG_MODE

using namespace tech::socket::v1;

void Chat::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                            const WebSocketMessageType &type) {
    WSCloser wsCloser;
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        _messageHandler(wsConnPtr, message);
    } else if (type == WebSocketMessageType::Pong) {
        LOG_DEBUG << "Message is Pong";
    } else if (type == WebSocketMessageType::Close) {
        LOG_DEBUG << "Message is Close";
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void Chat::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    WSCloser wsCloser;
    Player player;
    player.email = req->getParameter("email");
    player.accessToken = req->getParameter("access_token");
    player.roomID = "room_chatting";
    if (!authorization(wsCloser, player)) {
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }

    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();

    try {
        player.subscriberID = roomManager->joinChat([wsConnPtr](const std::string &message) {
            wsConnPtr->send(message);
        });
    } catch (std::range_error &e) {
        LOG_ERROR << "error:" << e.what();
        wsCloser._code = CloseCode::kInvalidMessage;
        wsCloser._reason = "Room is full";
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    } catch (std::out_of_range &e) {
        LOG_ERROR << "error:" << e.what();
        wsCloser._code = CloseCode::kInvalidMessage;
        wsCloser._reason = "Room is not available for now";
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }
    std::cout << "[" + player.username + "#" + std::to_string(player._id) + "]" + std::to_string(player.subscriberID) << std::endl;
    roomManager->chat("J" + player.username +
                      ":" + std::to_string(player._id) +
                      ":" + std::to_string(roomManager->chatCount()));
    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Chat::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->quitChat(player.subscriberID);
        roomManager->chat("L" + player.username +
                          ":" + std::to_string(player._id) +
                          ":" + std::to_string(roomManager->chatCount()));
    }
}

void Chat::_messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message) {
    char commandType = message[0];
    if (commandType == 'P') {
        wsConnPtr->send("", WebSocketMessageType::Pong);
    } else if (commandType == 'Q') {
        wsConnPtr->forceClose();
    } else if (commandType == 'T') {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("T" + player.username + ":" + std::to_string(player._id) + ":" + message.substr(1));
    } else {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("EInvalid command.");
    }
}

void Play::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                            const WebSocketMessageType &type) {
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        _messageHandler(wsConnPtr, message);
    } else if (type == WebSocketMessageType::Pong) {
        LOG_DEBUG << "Message is Pong";
    } else if (type == WebSocketMessageType::Close) {
        LOG_DEBUG << "Message is Close";
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void Play::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    WSCloser wsCloser;
    Player player;
    player.email = req->getParameter("email");
    player.accessToken = req->getParameter("access_token");
    player.roomID = req->getParameter("id");
    player.roomPassword = req->getParameter("password");
    if (!authorization(wsCloser, player)) {
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }

    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();

    try {
        if (!roomManager->checkPassword(player.roomID, player.roomPassword)) {
            wsCloser._code = CloseCode::kInvalidMessage;
            wsCloser._reason = "Password is incorrect";
            wsConnPtr->send(wsCloser._reason);
            wsCloser.close(wsConnPtr);
            return;
        }
    } catch (std::out_of_range &e) {
        LOG_ERROR << "error:" << e.what();
        wsCloser._code = CloseCode::kInvalidMessage;
        wsCloser._reason = "Room not found";
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }

    try {
        player.subscriberID = roomManager->subscribe(
                player.roomID,
                [wsConnPtr](const std::string &topic, const std::string &message) {
                    wsConnPtr->send(message);
                }
        );
    } catch (std::range_error &e) {
        LOG_ERROR << "error:" << e.what();
        wsCloser._code = CloseCode::kInvalidMessage;
        wsCloser._reason = "Room is full";
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    } catch (std::out_of_range &e) {
        LOG_ERROR << "error:" << e.what();
        wsCloser._code = CloseCode::kInvalidMessage;
        wsCloser._reason = "Room is not available for now";
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }
    roomManager->publish(player.roomID, "J" + player.username +
                                        ":" + std::to_string(player._id));
#ifdef DEBUG_MODE
    std::cout << "[SERVER#0]$J " << player.username + ":" + std::to_string(player._id) << std::endl;
    std::cout << "[" + player.username + "#" + std::to_string(player._id) + "]SubID: " + std::to_string(player.subscriberID) << std::endl;
#endif

    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Play::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->unsubscribe(player.roomID, player.subscriberID);
        roomManager->publish(player.roomID, "L" + player.username +
                                            ":" + std::to_string(player._id));
#ifdef DEBUG_MODE
        std::cout << "[SERVER#0]$L " << player.username + ":" + std::to_string(player._id) << std::endl;
#endif
    }
}

void Play::_messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message) {
    auto &player = wsConnPtr->getContextRef<Player>();
    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
    char commandType = message[0];
    auto seed = utils::utils::uniform_random();
#ifdef DEBUG_MODE
    std::cout << "[" << player.username << "#" << player._id << "]$" << commandType << " " << message.substr(1)
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
            std::cout << "[SERVER#0]$Q " << player.username + ":" + std::to_string(player._id) << std::endl;
#endif
            wsConnPtr->forceClose();
            break;
        case 'B':
#ifdef DEBUG_MODE
            std::cout << "[SERVER#0]$B " << seed << std::endl;
#endif
            roomManager->publish(player.roomID, "B" + std::to_string(seed));
            break;
        case 'T':
            roomManager->publish(player.roomID,
                                 "T" + player.username + ":" + std::to_string(player._id) + ":" + message.substr(1));
            break;
        case 'C':
            roomManager->publish(player.roomID,
                                 "C" + player.username + ":" + std::to_string(player._id) + ":" + message.substr(1),
                                 player.subscriberID);
            break;
        case 'S':
            roomManager->publish(player.roomID,
                                 "S" + std::to_string(player._id) + ":" + message.substr(1),
                                 player.subscriberID);
            break;
        default:
            roomManager->publish(player.roomID, "EInvalid command.");
            break;
    }
}
