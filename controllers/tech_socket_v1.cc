#include <plugins/tech_plugin_VersusManager.h>
#include "tech_socket_v1.h"

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
    player._email = req->getParameter("email");
    player._accessToken = req->getParameter("access_token");
    player._roomID = "room_chatting";
    if (!authorization(wsCloser, player)) {
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }

    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();

    try {
        player._subscriberID = roomManager->joinChat([wsConnPtr](const std::string &message) {
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
    roomManager->chat("J" + player._name +
                      ":" + std::to_string(player._id) +
                      ":" + std::to_string(roomManager->chatCount()));
    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Chat::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->quitChat(player._subscriberID);
        roomManager->chat("L" + player._name +
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
        roomManager->chat("T" + player._name + ":" + std::to_string(player._id) + ":" + message.substr(1));
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
    player._email = req->getParameter("email");
    player._accessToken = req->getParameter("access_token");
    player._roomID = req->getParameter("id");
    player._roomPassword = req->getParameter("password");
    if (!authorization(wsCloser, player)) {
        wsConnPtr->send(wsCloser._reason);
        wsCloser.close(wsConnPtr);
        return;
    }

    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();

    try {
        if (!roomManager->checkPassword(player._roomID, player._roomPassword)) {
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
        player._subscriberID = roomManager->subscribe(
                player._roomID,
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
    roomManager->publish(player._roomID, "J" + player._name +
                                         ":" + std::to_string(player._id));

    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Play::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->unsubscribe(player._roomID, player._subscriberID);
        roomManager->publish(player._roomID, "L" + player._name +
                                             ":" + std::to_string(player._id));
    }
}

void Play::_messageHandler(const WebSocketConnectionPtr &wsConnPtr, const std::string &message) {
    char commandType = message[0];
    std::cout << message << std::endl;
    if (commandType == 'P') {
        wsConnPtr->send("", WebSocketMessageType::Pong);
    } else if (commandType == 'Q') {
        wsConnPtr->forceClose();
    } else if (commandType == 'T') {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("T" + player._name + ":" + std::to_string(player._id) + ":" + message.substr(1));
    } else if (commandType == 'C') {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("C" + std::to_string(player._id) + ":" + message.substr(1));
    } else if (commandType == 'S') {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("S" + std::to_string(player._id) + ":" + message.substr(1));
    } else {
        auto &player = wsConnPtr->getContextRef<Player>();
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        roomManager->chat("EInvalid command.");
    }
}

//std::string Play::_jsonToPlainText(const Json::Value &value) {
//    Json::StreamWriterBuilder writerBuilder;
//    writerBuilder.settings_["indentation"] = "";
//    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
//    std::ostringstream oss;
//    jsonWriter->write(value, &oss);
//    return oss.str();
//}
