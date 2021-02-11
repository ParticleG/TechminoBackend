//
// Created by Parti on 2020/12/7.
//

#pragma once

#include <json/json.h>
#include <services/Player.h>

namespace tech::services {
    class Room : public trantor::NonCopyable {
    public:
        using MessageHandler = std::function<void(const std::string &)>;
#if __cplusplus >= 201703L | defined _WIN32
        using SharedMutex = std::shared_mutex;
#else
        using SharedMutex = std::shared_timed_mutex;
#endif

        Room(
                std::string id,
                std::string name,
                const std::string &password,
                std::string roomType,
                const uint64_t &capacity
        );

        void publish(const std::string &message) const;

        void publish(const std::string &message, const drogon::SubscriberID &excludedID) const;

        void tell(const std::string &message, const drogon::SubscriberID &targetID) const;

        bool checkReadyState();

        bool setReadyState(const bool &isReady);

        std::string getInfos();

        drogon::SubscriberID subscribe(const MessageHandler &handler);

        drogon::SubscriberID subscribe(MessageHandler &&handler);

        void join(drogon::SubscriberID id, const std::shared_ptr<tech::services::Player> &player);

        void changeGroup(drogon::SubscriberID id, const unsigned int &group);

        void startGame();

        uint64_t getWiningGroup();

        void endGame();

        void setDead(drogon::SubscriberID id);

        void unsubscribe(drogon::SubscriberID id);

        void quit(drogon::SubscriberID id);

        bool checkPassword(const std::string &password);

        bool changePassword(const std::string &oldPassword, const std::string &newPassword);

        bool empty() const;

        bool full() const;

        uint64_t count() const;

        std::string getType() const;

        void clear();

        Json::Value toJson();

    private:
        std::string _id, _name, _password, _type;
        uint64_t _count, _capacity;
        drogon::SubscriberID _inner_id;
        std::unordered_map<drogon::SubscriberID, MessageHandler> _handlersMap;
        std::unordered_map<drogon::SubscriberID, std::shared_ptr<tech::services::Player>> _playersMap;
        std::unordered_map<unsigned int, unsigned int> _groupsMap{};
        bool _isReady{}, _isIndividual{};
        mutable SharedMutex _sharedMutex;

        drogon::SubscriberID _loop_inner_id();
    };
}

