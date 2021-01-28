//
// Created by Parti on 2020/12/7.
//

#include "room.h"

void tech::utils::Room::publish(const std::string &message) const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    for (auto &pair : _handlersMap) {
        pair.second(message);
    }
}

SubscriberID tech::utils::Room::subscribe(const tech::utils::Room::MessageHandler &handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap[++_count] = handler;
    return _count;
}

SubscriberID tech::utils::Room::subscribe(Room::MessageHandler &&handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    if (_count == _capacity) {
        throw std::range_error("tech::utils::Room is full");
    }
    _handlersMap[++_count] = std::move(handler);
    return _count;
}

void tech::utils::Room::unsubscribe(SubscriberID id) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.erase(id);
    --_count;
}

bool tech::utils::Room::checkPassword(const std::string &password) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return Crypto::blake2b(password, 1) == _password;
}

bool tech::utils::Room::changePassword(const std::string &password, const std::string &newPassword) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    if (Crypto::blake2b(password, 1) == _password) {
        _password = Crypto::blake2b(newPassword, 1);
        return true;
    }
    return false;
}

bool tech::utils::Room::empty() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _handlersMap.empty();
}

bool tech::utils::Room::full() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _count == _capacity;
}

uint64_t tech::utils::Room::count() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _count;
}

void tech::utils::Room::clear() {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.clear();
}

Json::Value tech::utils::Room::toJson() {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    Json::Value tempJson;
    tempJson["room_id"] = _id;
    tempJson["name"] = _name;
    tempJson["type"] = _type;
    tempJson["encrypted"] = checkPassword("");
    tempJson["count"] = static_cast<Json::UInt64>(_count);
    tempJson["capacity"] = static_cast<Json::UInt64>(_capacity);
    return tempJson;
}