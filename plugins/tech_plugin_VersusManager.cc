/**
 *
 *  tech_RoomManager.cc
 *
 */

#include "tech_plugin_VersusManager.h"

using namespace tech::plugin;

std::string tech::plugin::md5(const std::string &srcStr) {
    // 调用md5哈希
    unsigned char mdStr[33] = {0};
    MD5((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);

    char buf[65] = {0};
    char tmp[3] = {0};
    for (int i = 0; i < 32; i++) {
        sprintf_s(tmp, "%02x", mdStr[i]);
        strcat_s(buf, tmp);
    }
    buf[32] = '\0';
    return std::string(buf);
}

std::string tech::plugin::sha256(const std::string &srcStr) {
    unsigned char mdStr[33] = {0};
    SHA256((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);
    char buf[65] = {0};
    char tmp[3] = {0};
    for (int i = 0; i < 32; i++) {
        sprintf_s(tmp, "%02x", mdStr[i]);
        strcat_s(buf, tmp);
    }
    buf[32] = '\0';
    return std::string(buf);
}


template<typename MessageType>
void Room<MessageType>::publish(const MessageType &message) const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    for (auto &pair : _handlersMap) {
        pair.second(message);
    }
}

template<typename MessageType>
SubscriberID Room<MessageType>::subscribe(const Room<MessageType>::MessageHandler &handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap[++_count] = handler;
    return _count;
}

template<typename MessageType>
SubscriberID Room<MessageType>::subscribe(Room::MessageHandler &&handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    if (_count == _capacity) {
        throw std::range_error("Room is full");
    }
    _handlersMap[++_count] = std::move(handler);
    return _count;
}

template<typename MessageType>
void Room<MessageType>::unsubscribe(SubscriberID id) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.erase(id);
}

template<typename MessageType>
bool Room<MessageType>::checkPassword(const std::string &password) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return sha256(password) == _password;
}

template<typename MessageType>
bool Room<MessageType>::changePassword(std::string password, std::string newPassword) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    if (sha256(password) == _password) {
        _password = sha256(newPassword);
        return true;
    }
    return false;
}

template<typename MessageType>
bool Room<MessageType>::empty() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _handlersMap.empty();
}

template<typename MessageType>
bool Room<MessageType>::full() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _count == _capacity;
}

template<typename MessageType>
void Room<MessageType>::clear() {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _handlersMap.clear();
}

template<typename MessageType>
Json::Value Room<MessageType>::toJson() {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    Json::Value tempJson;
    tempJson["room_id"] = _id;
    tempJson["name"] = _name;
    tempJson["type"] = _type;
    tempJson["count"] = _count;
    tempJson["capacity"] = _capacity;
    return tempJson;
}

template<typename MessageType>
void RoomManager<MessageType>::publish(const std::string &roomID,
                                       const MessageType &message) const {
    std::shared_ptr<Room<MessageType>> topicPtr;
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

template<typename MessageType>
SubscriberID RoomManager<MessageType>::subscribe(const std::string &roomID,
                                                 const RoomManager::MessageHandler &handler) {
    auto topicHandler = [roomID, handler](const MessageType &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, std::move(topicHandler));
}

template<typename MessageType>
SubscriberID RoomManager<MessageType>::subscribe(const std::string &roomID,
                                                 RoomManager::MessageHandler &&handler) {
    auto topicHandler = [roomID, handler = std::move(handler)](const MessageType &message) {
        handler(roomID, message);
    };
    return _subscribeToRoom(roomID, std::move(topicHandler));
}

template<typename MessageType>
void RoomManager<MessageType>::unsubscribe(const std::string &roomID, SubscriberID id) {
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

template<typename MessageType>
size_t RoomManager<MessageType>::size() const {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomIDMap.size();
}

template<typename MessageType>
bool RoomManager<MessageType>::full(const std::string &roomID) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->full();
    }
    throw std::out_of_range("Room not found");
}

template<typename MessageType>
bool RoomManager<MessageType>::empty(const std::string &roomID) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomIDMap.find(roomID);
    if (iter != _roomIDMap.end()) {
        return iter->second->empty();
    }
    throw std::out_of_range("Room not found");
}

template<typename MessageType>
void RoomManager<MessageType>::clear() {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.clear();
}

template<typename MessageType>
void RoomManager<MessageType>::createRoom(const std::string &roomID,
                                          const std::string &name,
                                          const std::string &password,
                                          const std::string &roomType,
                                          const uint64_t &capacity) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    auto topicPtr = std::make_shared<Room<MessageType>>(roomID, name, password, roomType, capacity);
    _roomIDMap[roomID] = std::move(topicPtr);
}

template<typename MessageType>
void RoomManager<MessageType>::removeRoom(const std::string &roomID) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomIDMap.erase(roomID);
}

template<typename MessageType>
bool RoomManager<MessageType>::checkPassword(const std::string &roomID, const std::string &password) {
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
    throw std::out_of_range("Room not found");
}

template<typename MessageType>
Json::Value RoomManager<MessageType>::getRoomJson(const std::string &roomID) {
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
    throw std::out_of_range("Room not found");
}

template<typename MessageType>
Json::Value RoomManager<MessageType>::getRoomList(const std::string &roomType) {
    Json::Value resultJson = Json::arrayValue;
    {
        std::shared_lock<SharedMutex> lock(_sharedMutex);
        for (const auto &room: _roomIDMap) {
            if (room.second->getType() == roomType) {
                resultJson.append(room.second->toJson());
            }
        }
        return resultJson;
    }
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    for (const auto &room: _roomIDMap) {
        if (room.second->getType() == roomType) {
            resultJson.append(room.second->toJson());
        }
    }
    return resultJson;
}

template<typename MessageType>
SubscriberID tech::plugin::RoomManager<MessageType>::_subscribeToRoom(const std::string &roomID,
                                                                      typename Room<MessageType>::MessageHandler &&handler) {
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
    throw std::out_of_range("Room not found");
}

void VersusManager::initAndStart(const Json::Value &config) {
    if (config.isMember("room_types") && config["room_types"].isArray()) {
        for (auto &roomType : config["room_types"]) {
            _roomTypes.insert({roomType["name"].asString(), roomType["capacity"].asUInt64()});
        }
        if (_roomTypes.empty()) {
            LOG_ERROR << "You must set at least one roomType";
            abort();
        }
    } else {
        LOG_ERROR << "Error in configuration";
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
                                      const RoomManager<std::string>::MessageHandler &handler) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.subscribe(roomID, handler);
}

void VersusManager::unsubscribe(const std::string &roomID, const SubscriberID &playerID) {
    std::unique_lock<SharedMutex> lock(_sharedMutex);
    _roomManager.unsubscribe(roomID, playerID);
}

bool VersusManager::checkPassword(const std::string &roomID, const std::string &password) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.checkPassword(roomID, password);
}

void VersusManager::publish(const std::string &roomID, const std::string &message) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    _roomManager.publish(roomID, message);
}

size_t VersusManager::size() {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    return _roomManager.size();
}

Json::Value VersusManager::getRoomList(const std::string &roomType) {
    std::shared_lock<SharedMutex> lock(_sharedMutex);
    auto iter = _roomTypes.find(roomType);
    if (iter != _roomTypes.end()) {
        return _roomManager.getRoomList(roomType);
    }
    throw std::out_of_range("Unsupported room type");
}