//
// Created by Parti on 2020/12/7.
//

#include <services/Room.h>
#include <utils/Crypto.h>

#include <utility>

using namespace tech::services;
using namespace tech::utils;
using namespace drogon;
using namespace std;

Room::Room(
        string id,
        string name,
        const string &password,
        string roomType,
        const uint64_t &capacity
) : _id(move(id)), _name(move(name)), _password(Crypto::blake2b(password, 1)), _type(move(roomType)), _capacity(capacity) {
    _count = 0;
    _inner_id = 0;
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

bool Room::checkReadyState() {
    shared_lock<SharedMutex> lock(_sharedMutex);
    bool isAllReady = true;
    for (auto &pair : _playersMap) {
        if (!pair.second->getReadyState()) {
            isAllReady = false;
            break;
        }
    }
    return isAllReady;
}

string Room::getInfos() {
    shared_lock<SharedMutex> lock(_sharedMutex);
    string infos;
    for (auto &pair : _playersMap) {
        infos += pair.second->getInfo();
    }
    return infos;
}

SubscriberID Room::subscribe(const Room::MessageHandler &handler) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    if (_count == _capacity) {
        throw range_error("Room is full");
    }
    _count++;
    auto tempID = _loop_inner_id();
    _handlersMap[tempID] = handler;
    return tempID;
}

SubscriberID Room::subscribe(Room::MessageHandler &&handler) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    if (_count == _capacity) {
        throw range_error("Room is full");
    }
    _count++;
    auto tempID = _loop_inner_id();
    _handlersMap[tempID] = move(handler);
    return tempID;
}

void Room::join(drogon::SubscriberID id, const shared_ptr<Player> &player) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _playersMap[id] = player;
}


void Room::unsubscribe(SubscriberID id) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.erase(id);
    --_count;
}

void Room::quit(SubscriberID id) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _playersMap.erase(id);
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

SubscriberID Room::_loop_inner_id() {
    while (true) {
        if (!_handlersMap.count(_inner_id)) {
            return _inner_id;
        }
        _inner_id = _inner_id + 1 == _capacity ? 0 : _inner_id + 1;
    }
}
