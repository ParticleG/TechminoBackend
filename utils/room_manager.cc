#include "room_manager.h"

void tech::utils::RoomManager::publish(const std::string &roomID, const std::string &message) const {
    std::shared_ptr<tech::utils::Room> topicPtr;
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->publish(message);
}

void tech::utils::RoomManager::publish(const std::string &roomID, const std::string &message,
                                       const SubscriberID &excludedID) const {
    std::shared_ptr<tech::utils::Room> topicPtr;
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->publish(message, excludedID);
}

void tech::utils::RoomManager::tell(const std::string &roomID, const std::string &message,
                                    const SubscriberID &targetID) const {
    std::shared_ptr<tech::utils::Room> topicPtr;
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            topicPtr = iter->second;
        } else {
            return;
        }
    }
    topicPtr->tell(message, targetID);
}

SubscriberID tech::utils::RoomManager::subscribe(const std::string &roomID,
                                                 const RoomManager::MessageHandler &handler) {
    auto topicHandler = [roomID, handler](const std::string &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, std::move(topicHandler));
}

SubscriberID
tech::utils::RoomManager::subscribe(const std::string &roomID, RoomManager::MessageHandler &&handler) {
    auto topicHandler = [roomID, handler = std::move(handler)](const std::string &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, std::move(topicHandler));
}

void tech::utils::RoomManager::unsubscribe(const std::string &roomID, SubscriberID id) {
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter == _roomIDMap.end()) {
            return;
        }
        iter->second->unsubscribe(id);
        if (!iter->second->empty())
            return;
    }
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter == _roomIDMap.end()) {
        return;
    }
    if (iter->second->empty())
        _roomIDMap.erase(iter);
}

size_t tech::utils::RoomManager::size() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomIDMap.size();
}

bool tech::utils::RoomManager::full(const std::string &roomID) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->full();
    }
    throw std::out_of_range("tech::utils::Room not found");
}

bool tech::utils::RoomManager::empty(const std::string &roomID) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->empty();
    }
    throw std::out_of_range("tech::utils::Room not found");
}

void tech::utils::RoomManager::clear() {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.clear();
}

void tech::utils::RoomManager::createRoom(const std::string &roomID,
                                          const std::string &name,
                                          const std::string &password,
                                          const std::string &roomType,
                                          const uint64_t &capacity) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto topicPtr = std::make_shared<tech::utils::Room>(roomID, name, password, roomType, capacity);
    _roomIDMap[roomID] = std::move(topicPtr);
}

void tech::utils::RoomManager::removeRoom(const std::string &roomID) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.erase(roomID);
}

bool tech::utils::RoomManager::checkPassword(const std::string &roomID, const std::string &password) {
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            return iter->second->checkPassword(password);
        }
    }
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->checkPassword(password);
    }
    throw std::out_of_range("tech::utils::Room not found");
}

Json::Value tech::utils::RoomManager::getRoomJson(const std::string &roomID) {
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            return iter->second->toJson();
        }
    }
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->toJson();
    }
    throw std::out_of_range("tech::utils::Room not found");
}

Json::Value tech::utils::RoomManager::getRoomList() {
    Json::Value resultJson = Json::arrayValue;
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    for (const auto &room: _roomIDMap) {
        resultJson.append(room.second->toJson());
    }
    return resultJson;
}

Json::Value tech::utils::RoomManager::getRoomList(const std::string &roomType) {
    Json::Value resultJson = Json::arrayValue;
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    for (const auto &room: _roomIDMap) {
        if (room.second->getType() == roomType) {
            resultJson.append(room.second->toJson());
        }
    }
    return resultJson;
}

SubscriberID tech::utils::RoomManager::_subscribeToRoom(const std::string &roomID,
                                                        typename tech::utils::Room::MessageHandler &&handler) {
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        auto iter = _roomIDMap.find(roomID);
        if (iter != _roomIDMap.end()) {
            try {
                return iter->second->subscribe(std::move(handler));
            } catch (std::range_error &error) {
                LOG_WARN << error.what();
                throw error;
            }
        }
    }
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->subscribe(std::move(handler));
    }
    throw std::out_of_range("tech::utils::Room not found");
}
