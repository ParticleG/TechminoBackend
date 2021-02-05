#include <services/RoomManager.h>
#include <drogon/drogon.h>

using namespace tech::services;
using namespace drogon;
using namespace std;

void RoomManager::publish(const string &roomID, const string &message) const {
    shared_ptr<Room> topicPtr;
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->publish(message);
}

void RoomManager::publish(const string &roomID, const string &message,
                          const SubscriberID &excludedID) const {
    shared_ptr<Room> topicPtr;
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->publish(message, excludedID);
}

void RoomManager::tell(const string &roomID, const string &message,
                       const SubscriberID &targetID) const {
    shared_ptr<Room> topicPtr;
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->tell(message, targetID);
}

SubscriberID RoomManager::subscribe(const string &roomID,
                                    const RoomManager::MessageHandler &handler) {
    auto topicHandler = [roomID, handler](const string &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, move(topicHandler));
}

SubscriberID RoomManager::subscribe(const string &roomID, RoomManager::MessageHandler &&handler) {
    auto topicHandler = [roomID, handler = move(handler)](const string &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, move(topicHandler));
}

void RoomManager::unsubscribe(const string &roomID, SubscriberID id) {
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter == _roomIDMap.end()) {
            return;
        }
        iter->second->unsubscribe(id);
        if (!iter->second->empty())
            return;
    }
    unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter == _roomIDMap.end()) {
        return;
    }
    if (iter->second->empty())
        _roomIDMap.erase(iter);
}

size_t RoomManager::size() const {
    shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomIDMap.size();
}

bool RoomManager::full(const string &roomID) {
    shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->full();
    }
    throw out_of_range("Room not found");
}

bool RoomManager::empty(const string &roomID) {
    shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->empty();
    }
    throw out_of_range("Room not found");
}

void RoomManager::clear() {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.clear();
}

void RoomManager::createRoom(const string &roomID,
                             const string &name,
                             const string &password,
                             const string &roomType,
                             const uint64_t &capacity) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    auto topicPtr = make_shared<Room>(roomID, name, password, roomType, capacity);
    _roomIDMap[roomID] = move(topicPtr);
}

void RoomManager::removeRoom(const string &roomID) {
    unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.erase(roomID);
}

bool RoomManager::checkPassword(const string &roomID, const string &password) {
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            return iter->second->checkPassword(password);
        }
    }
    unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->checkPassword(password);
    }
    throw out_of_range("Room not found");
}

Json::Value RoomManager::getRoomJson(const string &roomID) {
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            return iter->second->toJson();
        }
    }
    unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->toJson();
    }
    throw out_of_range("Room not found");
}

Json::Value RoomManager::getRoomList() {
    Json::Value resultJson = Json::arrayValue;
    shared_lock<SharedMutex> lock(_sharedMutex);
    for (const auto &room: _roomIDMap) {
        resultJson.append(room.second->toJson());
    }
    return resultJson;
}

Json::Value RoomManager::getRoomList(const string &roomType) {
    Json::Value resultJson = Json::arrayValue;
    shared_lock<SharedMutex> lock(_sharedMutex);
    for (const auto &room: _roomIDMap) {
        if (room.second->getType() == roomType) {
            resultJson.append(room.second->toJson());
        }
    }
    return resultJson;
}

SubscriberID RoomManager::_subscribeToRoom(const string &roomID, typename Room::MessageHandler &&handler) {
    {
        shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            try {
                return iter->second->subscribe(move(handler));
            } catch (range_error &error) {
                LOG_WARN << error.what();
                throw error;
            }
        }
    }
    unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->subscribe(move(handler));
    }
    throw out_of_range("Room not found");
}
