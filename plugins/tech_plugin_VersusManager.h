/**
 *
 *  tech_RoomManager.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
#include <utils/communication_manager.h>
#include <utils/room_manager.h>
#include <utils/room.h>
#include <utils/utils.h>

using namespace drogon;
using namespace trantor;

namespace tech {
    namespace plugin {

        class VersusManager : public drogon::Plugin<VersusManager> {
        public:
#if __cplusplus >= 201703L | defined _WIN32
            using SharedMutex = std::shared_mutex;
#else
            using SharedMutex = std::shared_timed_mutex;
#endif

            VersusManager() {}

            /// This method must be called by drogon to initialize and start the plugin.
            /// It must be implemented by the user.
            virtual void initAndStart(const Json::Value &config) override;

            /// This method must be called by drogon to shutdown the plugin.
            /// It must be implemented by the user.
            virtual void shutdown() override;

            Json::Value createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                                   const std::string &roomType);

            SubscriberID
            subscribe(const std::string &roomID, const tech::utils::RoomManager::MessageHandler &handler);

            void unsubscribe(const std::string &roomID, const SubscriberID &playerID);

            SubscriberID joinChat(const tech::utils::Room::MessageHandler &handler);

            void quitChat(SubscriberID playerID);

            bool checkPassword(const std::string &roomID, const std::string &password);

            void publish(const std::string &roomID, const std::string &message);

            void publish(const std::string &roomID, const std::string &message, const SubscriberID &excludedID) const;

            void tell(const std::string &roomID, const std::string &message, const SubscriberID &targetID) const;

            void chat(const std::string &message);

            [[maybe_unused]] size_t size();

            uint64_t chatCount();

            Json::Value getRoomList();

            Json::Value getRoomList(const std::string &roomType);

        private:
            std::unordered_map<std::string, uint64_t> _roomTypes;
            tech::utils::CommunicationManager _communicationManager;
            tech::utils::RoomManager _roomManager;
            tech::utils::Room _chattingRoom{"room_chatting", "Chat room", "", "unlimited", 2147483647};
            mutable SharedMutex _sharedMutex;
        };

    }
}
