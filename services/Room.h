//
// Created by Parti on 2020/12/7.
//

#pragma once

#include <drogon/PubSubService.h>
#include <json/json.h>

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
                const std::string &id,
                const std::string &name,
                const std::string &password,
                const std::string &roomType,
                const uint64_t &capacity
        );

        void publish(const std::string &message) const;

        void publish(const std::string &message, const drogon::SubscriberID &excludedID) const;

        void tell(const std::string &message, const drogon::SubscriberID &targetID) const;

        drogon::SubscriberID subscribe(const MessageHandler &handler);

        drogon::SubscriberID subscribe(MessageHandler &&handler);

        void unsubscribe(drogon::SubscriberID id);

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
        std::unordered_map<drogon::SubscriberID, MessageHandler> _handlersMap;
        mutable SharedMutex _sharedMutex;
    };
}

