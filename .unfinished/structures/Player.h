//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/PubSubService.h>
#include <models/Info.h>

namespace tech::structures {
    class Player {
    public:
        Player(
                const std::string &email,
                std::string config,
                std::string roomID,
                std::string roomPassword = ""
        );

        void subscribe(drogon::SubscriberID subscriberID);

        drogon_model::Techmino::Info getUser();

        std::string getInfo();

        void setConfig(std::string config);

        std::string getConfig() const;

        std::string getRoomID() const;

        [[maybe_unused]] std::string getPassword() const;

        drogon::SubscriberID getSubscriberID() const;

        void setReadyState(const bool &isReady);

        bool getReadyState() const;

        void setAliveState(const bool &isAlive);

        bool getAliveState() const;

        void setGroup(const unsigned int &group);

        unsigned int getGroup() const;

    private:
        drogon_model::Techmino::Info _user;
        std::string _config, _roomID, _password;
        drogon::SubscriberID _subID{};
        bool _isReady{}, _isAlive{};
        unsigned int _group{};
    };
}