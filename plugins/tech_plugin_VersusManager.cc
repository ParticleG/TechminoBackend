/**
 *
 *  tech_RoomManager.cc
 *
 */

#include "tech_plugin_VersusManager.h"

using namespace tech::plugin;
using namespace tech::utils;

void VersusManager::initAndStart(const Json::Value &config) {
    if (config.isMember("port") && config["port"].isUInt()) {
        if (config["port"].asUInt() < 10000 || config["port"].asUInt() > 65535) {
            LOG_ERROR << "Invalid port";
            abort();
        }
        _communicationManager.initAndStart(static_cast<uint16_t>(config["port"].asUInt()), "versus", 4);
    } else {
        LOG_ERROR << R"(Requires "port" in plugin's config')";
        abort();
    }
    if (config.isMember("room_types") && config["room_types"].isArray()) {
        for (auto &roomType : config["room_types"]) {
            _roomTypes.insert({roomType["name"].asString(), roomType["capacity"].asUInt64()});
        }
        if (_roomTypes.empty()) {
            LOG_ERROR << "You must set at least one roomType";
            abort();
        }
    } else {
        LOG_ERROR << R"(Requires "room_types" in plugin's config')";
        abort();
    }
}

void VersusManager::shutdown() {
    /// Shutdown the plugin
}

Json::Value VersusManager::createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                                      const std::string &roomType) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomManager.createRoom(roomID, name, password, roomType, _roomTypes.at(roomType));
    return _roomManager.getRoomJson(roomID);
}

SubscriberID VersusManager::subscribe(const std::string &roomID,
                                      const tech::utils::RoomManager::MessageHandler &handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.subscribe(roomID, handler);
}

void VersusManager::unsubscribe(const std::string &roomID, const SubscriberID &playerID) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomManager.unsubscribe(roomID, playerID);
}

SubscriberID VersusManager::joinChat(const tech::utils::Room::MessageHandler &handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    return _chattingRoom.subscribe(handler);
}

void VersusManager::quitChat(SubscriberID playerID) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _chattingRoom.unsubscribe(playerID);
}

bool VersusManager::checkPassword(const std::string &roomID, const std::string &password) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.checkPassword(roomID, password);
}

void VersusManager::publish(const std::string &roomID, const std::string &message) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    _roomManager.publish(roomID, message);
}

void VersusManager::chat(const std::string &message) {
    _chattingRoom.publish(message);
}

[[maybe_unused]] size_t VersusManager::size() {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.size();
}

uint64_t VersusManager::chatCount() {
    return _chattingRoom.count();
}

Json::Value VersusManager::getRoomList() {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.getRoomList();
}

Json::Value VersusManager::getRoomList(const std::string &roomType) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomTypes.find(roomType);
    if (iter != _roomTypes.end()) {
        return _roomManager.getRoomList(roomType);
    }
    throw std::out_of_range("Unsupported room type");
}