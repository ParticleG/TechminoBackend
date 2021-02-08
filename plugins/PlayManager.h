//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/plugins/Plugin.h>
#include <services/Room.h>
#include <services/RoomManager.h>
#include <utils/Utils.h>

namespace tech::plugin {
    class PlayManager : public drogon::Plugin<PlayManager> {
    public:
        PlayManager() {}

        virtual void initAndStart(const Json::Value &config) override;

        virtual void shutdown() override;

        Json::Value createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                               const std::string &roomType);

        drogon::SubscriberID subscribe(const std::string &roomID, const tech::services::RoomManager::MessageHandler &handler, const std::shared_ptr<tech::services::Player> &player);

        void unsubscribe(const std::string &roomID, const drogon::SubscriberID &playerID);

        bool checkPassword(const std::string &roomID, const std::string &password);

        void publish(const std::string &roomID, const std::string &message);

        void publish(const std::string &roomID, const std::string &message, const drogon::SubscriberID &excludedID) const;

        void tell(const std::string &roomID, const std::string &message, const drogon::SubscriberID &targetID) const;

        bool checkReadyState(const std::string &roomID);

        std::string getInfos(const std::string &roomID);

        [[maybe_unused]] size_t size();

        Json::Value getRoomList();

        Json::Value getRoomList(const std::string &roomType);

    private:
        std::unordered_map<std::string, uint64_t> _roomTypes;
        std::unique_ptr<tech::services::RoomManager> _roomManager;
        mutable std::shared_mutex _sharedMutex;
    };
}
