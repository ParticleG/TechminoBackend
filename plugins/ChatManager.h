//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/plugins/Plugin.h>
#include <.unused/communication_manager.h>
#include <services/RoomManager.h>
#include <services/Room.h>
#include <utils/Utils.h>

namespace tech ::plugin {
    class ChatManager : public drogon::Plugin<ChatManager> {
    public:
        ChatManager() {}

        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        virtual void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        virtual void shutdown() override;

        std::string getID() const;

        SubscriberID joinChat(const tech::utils::Room::MessageHandler &handler);

        void quitChat(SubscriberID playerID);

        void chat(const std::string &message);

        uint64_t chatCount();

    private:
        std::string _roomID;
        std::unique_ptr<tech::utils::Room> _chattingRoom;
        mutable std::shared_mutex _sharedMutex;
    };
}

