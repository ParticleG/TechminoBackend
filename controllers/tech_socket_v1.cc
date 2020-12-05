#include <plugins/tech_plugin_VersusManager.h>
#include "tech_socket_v1.h"

using namespace tech::socket::v1;

void Chat::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                            const WebSocketMessageType &type) {
    WSCloser wsCloser;
    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        if (message == "/quit"){
            wsConnPtr->forceClose();
//            wsCloser._code = CloseCode::kNormalClosure;
//            wsCloser._reason = "Player request closed";
//            wsCloser.close(wsConnPtr);
            return;
        }
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->chat(player._name + "#" + std::to_string(player._id) + ":" + message);
    } else if (type == WebSocketMessageType::Pong) {
        LOG_DEBUG << "Message is Pong";
    } else if (type == WebSocketMessageType::Close) {
        LOG_DEBUG << "Message is Close";
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void Chat::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    LOG_DEBUG << "New connection";
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
    roomManager->chat("#J:" + std::to_string(roomManager->chatCount()));
    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Chat::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    LOG_DEBUG << "Connection closed";
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->quitChat(player._subscriberID);
        roomManager->chat("#L:" + std::to_string(roomManager->chatCount()));
    }
}

void Solo::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                            const WebSocketMessageType &type) {
    auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else if (type == WebSocketMessageType::Text || type == WebSocketMessageType::Binary) {
        auto &player = wsConnPtr->getContextRef<Player>();
        LOG_DEBUG << "Room ID: " << player._roomID << ", Player ID: " << player._subscriberID;
        roomManager->publish(player._roomID, message);
    } else if (type == WebSocketMessageType::Pong) {
        LOG_DEBUG << "Message is Pong";
    } else if (type == WebSocketMessageType::Close) {
        LOG_DEBUG << "Message is Close";
    } else if (type == WebSocketMessageType::Unknown) {
        LOG_DEBUG << "Message is Unknown";
    }
}

void Solo::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    wsConnPtr->send("Welcome to solo room.");
    WSCloser wsCloser;
    Player player;
    player._email = req->getParameter("email");
    player._accessToken = req->getParameter("access_token");
    player._roomID = req->getParameter("room_id");
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
        player._subscriberID = roomManager->subscribe(player._roomID,
                                                      [wsConnPtr](const std::string &topic,
                                                                  const std::string &message) {
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

    LOG_DEBUG << "Room ID: " << player._roomID << ", Player ID: " << player._subscriberID;
    wsConnPtr->setContext(std::make_shared<Player>(std::move(player)));
}

void Solo::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    LOG_DEBUG << "websocket closed!";
    if (wsConnPtr->hasContext()) {
        auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
        auto &player = wsConnPtr->getContextRef<Player>();
        roomManager->unsubscribe(player._roomID, player._subscriberID);
    }
}

void SmallRoom::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                                 const WebSocketMessageType &type) {
    //write your application logic here
    wsConnPtr->send(message);
}

void SmallRoom::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    //write your application logic here
}

void SmallRoom::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    //write your application logic here
}

void LargeRoom::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                                 const WebSocketMessageType &type) {
    std::cout << message << std::endl;
    if (type == WebSocketMessageType::Ping) {
        LOG_DEBUG << "Received a PING";
    } else {
        wsConnPtr->send(message);
    }
}

void LargeRoom::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    std::cout << "new websocket connection!" << std::endl;
}

void LargeRoom::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    std::cout << "websocket closed!" << std::endl;
}
