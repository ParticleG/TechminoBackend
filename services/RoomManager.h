//
// Created by Parti on 2020/12/7.
//

#pragma once

#include <services/Room.h>

namespace tech::services {
    class RoomManager : public trantor::NonCopyable {
    public:
        using MessageHandler = std::function<void(const std::string &, const std::string &)>;
#if __cplusplus >= 201703L | defined _WIN32
        using SharedMutex = std::shared_mutex;
#else
        using SharedMutex = std::shared_timed_mutex;
#endif

        void publish(const std::string &roomID, const std::string &message) const;

        void publish(const std::string &roomID, const std::string &message, const drogon::SubscriberID &excludedID) const;

        void tell(const std::string &roomID, const std::string &message, const drogon::SubscriberID &targetID) const;

        bool checkReadyState(const std::string &roomID);

        std::string getInfos(const std::string &roomID);

        drogon::SubscriberID subscribe(const std::string &roomID, const MessageHandler &handler, const std::shared_ptr<tech::services::Player> &player);

        void unsubscribe(const std::string &roomID, drogon::SubscriberID id);

        size_t size() const;

        bool full(const std::string &roomID);

        bool empty(const std::string &roomID);

        void clear();

        void createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                        const std::string &roomType, const uint64_t &capacity);

        void removeRoom(const std::string &roomID);

        bool checkPassword(const std::string &roomID, const std::string &password);

        void startGame(const std::string &roomID);

        void changeGroup(const std::string &roomID, drogon::SubscriberID id, const unsigned int &group);

        void setDead(const std::string &roomID, drogon::SubscriberID id);

        unsigned int endGame(const std::string &roomID);

        Json::Value getRoomJson(const std::string &roomID);

        Json::Value getRoomList();

        Json::Value getRoomList(const std::string &roomType);

    private:
        std::unordered_map<std::string, std::shared_ptr<Room>> _roomIDMap;
        mutable SharedMutex _sharedMutex;

        drogon::SubscriberID _subscribeToRoom(const std::string &roomID, typename Room::MessageHandler &&handler, const std::shared_ptr<tech::services::Player> &player);
    };
}

