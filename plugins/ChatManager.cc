/**
 *
 *  tech_plugin_ChatManager.cc
 *
 */

#include "ChatManager.h"

#include <memory>

using namespace drogon;
using namespace tech::plugin;

void ChatManager::initAndStart(const Json::Value &config) {
    if (config.isMember("id") && config["id"].isString()) {
        _roomID = config["id"].asString();
        _chattingRoom = std::make_unique<tech::utils::Room>(
                config["id"].asString(),
                config["name"].asString(),
                "",
                "unlimited",
                2147483647
        );
    } else {
        LOG_ERROR << R"(Requires "id" in plugin's config')";
        abort();
    }
}

void ChatManager::shutdown() {
    /// Shutdown the plugin
}

std::string ChatManager::getID() const {
    return _roomID;
}

SubscriberID ChatManager::joinChat(const tech::utils::Room::MessageHandler &handler) {
    std::unique_lock<std::shared_mutex> lock(_sharedMutex);
    return _chattingRoom->subscribe(handler);
}

void ChatManager::quitChat(SubscriberID playerID) {
    std::unique_lock<std::shared_mutex> lock(_sharedMutex);
    _chattingRoom->unsubscribe(playerID);
}

void ChatManager::chat(const std::string &message) {
    _chattingRoom->publish(message);
}

uint64_t ChatManager::chatCount() {
    return _chattingRoom->count();
}
