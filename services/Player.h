//
// Created by Parti on 2021/2/5.
//

#pragma once

#include <drogon/PubSubService.h>
#include <models/User.h>
#include <string>

namespace tech::services {
    class Player {
    public:
        Player(
                const std::string &email,
                std::string roomID,
                std::string roomPassword = ""
        );

        void subscribe(drogon::SubscriberID subscriberID);

        drogon_model::Techmino::User getUser();

        std::string getRoomID() const;

        [[maybe_unused]] std::string getPassword() const;

        drogon::SubscriberID getSubscriberID() const;

    private:
        drogon_model::Techmino::User _user;
        std::string _roomID{}, _password{};
        drogon::SubscriberID _subID{};
    };
}