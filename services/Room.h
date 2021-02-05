//
// Created by Parti on 2020/12/7.
//

#pragma once

#include <drogon/drogon.h>
#include <drogon/PubSubService.h>
#include <json/json.h>

#include "Crypto.h"

using namespace drogon;

namespace tech::utils {
    class Room : public trantor::NonCopyable {
    public:
        using MessageHandler = std::function<void(const std::string &)>;
#if __cplusplus >= 201703L | defined _WIN32
        using SharedMutex = std::shared_mutex;
#else
        using SharedMutex = std::shared_timed_mutex;
#endif

        Room(const std::string &id, const std::string &name, const std::string &password, const std::string &roomType,
             const uint64_t &capacity) {
            std::unique_lock<SharedMutex> lock(_sharedMutex);
            _id = id;
            _name = name;
            _password = Crypto::blake2b(password, 1);
            _count = 0;
            _type = roomType;
            _capacity = capacity;
        }

        void publish(const std::string &message) const;

        void publish(const std::string &message, const SubscriberID &excludedID) const;

        void tell(const std::string &message, const SubscriberID &targetID) const;

        SubscriberID subscribe(const MessageHandler &handler);

        SubscriberID subscribe(MessageHandler &&handler);

        void unsubscribe(SubscriberID id);

        bool checkPassword(const std::string &password);

        bool changePassword(const std::string &oldPassword, const std::string &newPassword);

        bool empty() const;

        bool full() const;

        uint64_t count() const;

        void clear();

        Json::Value toJson();

        std::string getType() {
            return _type;
        }

    private:
        std::string _id, _name, _password, _type;
        uint64_t _count, _capacity;
        std::unordered_map<SubscriberID, MessageHandler> _handlersMap;
        mutable SharedMutex _sharedMutex;
    };
}

