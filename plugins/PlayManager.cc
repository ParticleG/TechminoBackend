/**
 *
 *  tech_plugin_PlayManager.cc
 *
 */

#include "PlayManager.h"

using namespace drogon;
using namespace tech::plugin;

void PlayManager::initAndStart(const Json::Value &config) {
    if (config.isMember("room_types") && config["room_types"].isArray()) {
        for (auto &roomType : config["room_types"]) {
            _roomTypes.insert({roomType["name"].asString(), roomType["capacity"].asUInt64()});
        }
        if (_roomTypes.empty()) {
            LOG_ERROR << "You must set at least one roomType";
            abort();
        }
    } else {
        LOG_ERROR << R"(Requires array value "room_types" in plugin PlayManager's config')";
        abort();
    }
}

void PlayManager::shutdown() {}

Json::Value PlayManager::createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                                    const std::string &roomType) {
    std::unique_lock<std::shared_mutex> lock(_sharedMutex);
    _roomManager.createRoom(roomID, name, password, roomType, _roomTypes.at(roomType));
    return _roomManager.getRoomJson(roomID);
}

SubscriberID PlayManager::subscribe(const std::string &roomID,
                                    const tech::utils::RoomManager::MessageHandler &handler) {
    std::unique_lock<std::shared_mutex> lock(_sharedMutex);
    return _roomManager.subscribe(roomID, handler);
}

void PlayManager::unsubscribe(const std::string &roomID, const SubscriberID &playerID) {
    std::unique_lock<std::shared_mutex> lock(_sharedMutex);
    _roomManager.unsubscribe(roomID, playerID);
}

bool PlayManager::checkPassword(const std::string &roomID, const std::string &password) {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    return _roomManager.checkPassword(roomID, password);
}

void PlayManager::publish(const std::string &roomID, const std::string &message) {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    _roomManager.publish(roomID, message);
}

void PlayManager::publish(const std::string &roomID,
                          const std::string &message,
                          const SubscriberID &excludedID) const {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    _roomManager.publish(roomID, message, excludedID);
}

void PlayManager::tell(const std::string &roomID, const std::string &message, const SubscriberID &targetID) const {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    _roomManager.tell(roomID, message, targetID);
}


[[maybe_unused]] size_t PlayManager::size() {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    return _roomManager.size();
}

Json::Value PlayManager::getRoomList() {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    return _roomManager.getRoomList();
}

Json::Value PlayManager::getRoomList(const std::string &roomType) {
    std::shared_lock<std::shared_mutex> lock(_sharedMutex);
    auto iter = _roomTypes.find(roomType);
    if (iter != _roomTypes.end()) {
        return _roomManager.getRoomList(roomType);
    }
    throw std::out_of_range("Unsupported room type");
}