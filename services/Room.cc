//
// Created by Parti on 2020/12/7.
//

#include <services/Room.h>
#include <utils/Crypto.h>

using namespace tech::services;
using namespace tech::utils;
using namespace drogon;
using namespace std;

Room::Room(
        const string &id,
        const string &name,
        const string &password,
        const string &roomType,
        const uint64_t &capacity
) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _id = id;
    _name = name;
    _password = Crypto::blake2b(password, 1);
    _count = 0;
    _type = roomType;
    _capacity = capacity;
}

void Room::publish(const string &message) const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    for (auto &pair : _handlersMap) {
        pair.second(message);
    }
}

void Room::publish(const string &message, const SubscriberID &excludedID) const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    for (auto &pair : _handlersMap) {
        if (pair.first != excludedID) {
            pair.second(message);
        }
    }
}

void Room::tell(const string &message, const SubscriberID &targetID) const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    for (auto &pair : _handlersMap) {
        if (pair.first == targetID) {
            pair.second(message);
        }
    }
}

SubscriberID Room::subscribe(const Room::MessageHandler &handler) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap[++_count] = handler;
    return _count;
}

SubscriberID Room::subscribe(Room::MessageHandler &&handler) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    if (_count == _capacity) {
        throw range_error("Room is full");
    }
    _handlersMap[++_count] = move(handler);
    return _count;
}

void Room::unsubscribe(SubscriberID id) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.erase(id);
    --_count;
}

bool Room::checkPassword(const string &password) {
    shared_lock<SharedMutex> lock(_sharedMutex);
    return Crypto::blake2b(password, 1) == _password;
}

bool Room::changePassword(const string &oldPassword, const string &newPassword) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    if (Crypto::blake2b(oldPassword, 1) == _password) {
        _password = Crypto::blake2b(newPassword, 1);
        return true;
    }
    return false;
}

bool Room::empty() const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    return _handlersMap.empty();
}

bool Room::full() const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    return _count == _capacity;
}

uint64_t Room::count() const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    return _count;
}

string Room::getType() const {
    return _type;
}

void Room::clear() {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.clear();
}

Json::Value Room::toJson() {
    shared_lock<SharedMutex> lock(_sharedMutex);
    Json::Value tempJson;
    tempJson["id"] = _id;
    tempJson["name"] = _name;
    tempJson["type"] = _type;
    tempJson["private"] = !checkPassword("");
    tempJson["count"] = static_cast<Json::UInt64>(_count);
    tempJson["capacity"] = static_cast<Json::UInt64>(_capacity);
    return tempJson;
}