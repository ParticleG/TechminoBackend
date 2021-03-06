//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/plugins/Plugin.h>
#include <structures/BaseRoom.h>
#include <structures/BaseManager.h>
#include <utils/Utils.h>

namespace tech::plugins {
    class PlayManager : public drogon::Plugin<PlayManager> {
    public:
        PlayManager() {}

        virtual void initAndStart(const Json::Value &config) override;

        virtual void shutdown() override;

        Json::Value createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                               const std::string &roomType);

        drogon::SubscriberID subscribe(const std::string &roomID, const tech::structures::BaseManager::MessageHandler &handler, const std::shared_ptr<tech::structures::Player> &player);

        void unsubscribe(const std::string &roomID, const drogon::SubscriberID &playerID);

        bool checkPassword(const std::string &roomID, const std::string &password);

        void startGame(const std::string &roomID);

        void changeGroup(const std::string &roomID, drogon::SubscriberID id, const unsigned int &group);

        void setDead(const std::string &roomID, drogon::SubscriberID id);

        uint64_t endGame(const std::string &roomID);

        void publish(const std::string &roomID, const std::string &message);

        void publish(const std::string &roomID, const std::string &message, const drogon::SubscriberID &excludedID) const;

        [[maybe_unused]] void tell(const std::string &roomID, const std::string &message, const drogon::SubscriberID &targetID) const;

        bool checkReadyState(const std::string &roomID);

        bool setReadyState(const std::string &roomID, const bool &isReady);

        std::string getInfos(const std::string &roomID);

        [[maybe_unused]] size_t size();

        Json::Value getRoomList();

        Json::Value getRoomList(const std::string &roomType);

    private:
        std::unordered_map<std::string, uint64_t> _roomTypes;
        std::unique_ptr<tech::structures::BaseManager> _roomManager;
        mutable std::shared_mutex _sharedMutex;
    };
}
