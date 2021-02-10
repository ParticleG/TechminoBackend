//
// Created by Parti on 2021/2/4.
//

#include <plugins/PlayManager.h>

using namespace tech::plugin;
using namespace tech::services;
using namespace drogon;
using namespace std;

void PlayManager::initAndStart(const Json::Value &config) {
    if (config.isMember("room_types") && config["room_types"].isArray()) {
        for (auto &roomType : config["room_types"]) {
            _roomTypes.insert({roomType["name"].asString(), roomType["capacity"].asUInt64()});
        }
        if (_roomTypes.empty()) {
            LOG_ERROR << "You must set at least one roomType";
            abort();
        }
        _roomManager = make_unique<RoomManager>();
    } else {
        LOG_ERROR << R"(Requires array value "room_types" in plugin PlayManager's config')";
        abort();
    }
}

void PlayManager::shutdown() {}

Json::Value PlayManager::createRoom(const string &roomID, const string &name, const string &password,
                                    const string &roomType) {
    unique_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->createRoom(roomID, name, password, roomType, _roomTypes.at(roomType));
    return _roomManager->getRoomJson(roomID);
}

SubscriberID PlayManager::subscribe(const string &roomID,
                                    const RoomManager::MessageHandler &handler, const shared_ptr<tech::services::Player> &player) {
    unique_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->subscribe(roomID, handler, player);
}

void PlayManager::unsubscribe(const string &roomID, const SubscriberID &playerID) {
    unique_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->unsubscribe(roomID, playerID);
}

bool PlayManager::checkPassword(const string &roomID, const string &password) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->checkPassword(roomID, password);
}

void PlayManager::startGame(const string &roomID) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->startGame(roomID);
}

void PlayManager::changeGroup(const string &roomID, drogon::SubscriberID id, const unsigned int &group) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->changeGroup(roomID, id, group);
}

void PlayManager::setDead(const string &roomID, drogon::SubscriberID id) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->setDead(roomID, id);
}

unsigned int PlayManager::endGame(const string &roomID) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->endGame(roomID);
}

void PlayManager::publish(const string &roomID, const string &message) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->publish(roomID, message);
}

void PlayManager::publish(const string &roomID,
                          const string &message,
                          const SubscriberID &excludedID) const {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->publish(roomID, message, excludedID);
}

void PlayManager::tell(const string &roomID, const string &message, const SubscriberID &targetID) const {
    shared_lock<shared_mutex> lock(_sharedMutex);
    _roomManager->tell(roomID, message, targetID);
}

bool PlayManager::checkReadyState(const string &roomID) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->checkReadyState(roomID);
}

std::string PlayManager::getInfos(const string &roomID) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->getInfos(roomID);
}

[[maybe_unused]] size_t PlayManager::size() {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->size();
}

Json::Value PlayManager::getRoomList() {
    shared_lock<shared_mutex> lock(_sharedMutex);
    return _roomManager->getRoomList();
}

Json::Value PlayManager::getRoomList(const string &roomType) {
    shared_lock<shared_mutex> lock(_sharedMutex);
    auto iter = _roomTypes.find(roomType);
    if (iter != _roomTypes.end()) {
        return _roomManager->getRoomList(roomType);
    }
    throw out_of_range("Unsupported room type");
}
