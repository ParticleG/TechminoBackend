//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/plugins/Plugin.h>
#include <services/Player.h>
#include <services/Room.h>

namespace tech ::plugin {
    class ChatManager : public drogon::Plugin<ChatManager> {
    public:
        ChatManager() {}

        virtual void initAndStart(const Json::Value &config) override;

        virtual void shutdown() override;

        std::string getID() const;

        drogon::SubscriberID joinChat(const tech::services::Room::MessageHandler &handler, const std::shared_ptr<tech::services::Player> &player);

        void quitChat(drogon::SubscriberID playerID);

        void chat(const std::string &message);

        uint64_t chatCount();

    private:
        std::string _roomID;
        std::unique_ptr<tech::services::Room> _chattingRoom;
        mutable std::shared_mutex _sharedMutex;
    };
}

