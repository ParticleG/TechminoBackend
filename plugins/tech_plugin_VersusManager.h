/**
 *
 *  tech_RoomManager.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
#include <drogon/PubSubService.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace drogon;

namespace tech::plugin {
    std::string md5(const std::string &srcStr);

    std::string sha256(const std::string &srcStr);

    template<typename MessageType>
    class Room : public trantor::NonCopyable {
    public:
        using MessageHandler = std::function<void(const MessageType &)>;
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
            _password = sha256(password);
            _count = 0;
            _type = roomType;
            _capacity = capacity;
        }

        void publish(const MessageType &message) const;

        SubscriberID subscribe(const MessageHandler &handler);

        SubscriberID subscribe(MessageHandler &&handler);

        void unsubscribe(SubscriberID id);

        bool checkPassword(const std::string &password);

        bool changePassword(std::string password, std::string newPassword);

        bool empty() const;

        bool full() const;

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

    template<typename MessageType>
    class RoomManager : public trantor::NonCopyable {
    public:
        using MessageHandler =
        std::function<void(const std::string &, const MessageType &)>;
#if __cplusplus >= 201703L | defined _WIN32
        using SharedMutex = std::shared_mutex;
#else
        using SharedMutex = std::shared_timed_mutex;
#endif

        void publish(const std::string &roomID, const MessageType &message) const;

        SubscriberID subscribe(const std::string &roomID, const MessageHandler &handler);

        SubscriberID subscribe(const std::string &roomID, MessageHandler &&handler);

        void unsubscribe(const std::string &roomID, SubscriberID id);

        size_t size() const;

        bool full(const std::string &roomID);

        bool empty(const std::string &roomID);

        void clear();

        void createRoom(const std::string &roomID, const std::string &name, const std::string &password,
                        const std::string &roomType, const uint64_t &capacity);

        void removeRoom(const std::string &roomID);

        bool checkPassword(const std::string &roomID, const std::string &password);

        Json::Value getRoomJson(const std::string &roomID);

        Json::Value getRoomList(const std::string &roomType);

    private:
        std::unordered_map<std::string, std::shared_ptr<Room<MessageType>>> _roomIDMap;
        mutable SharedMutex _sharedMutex;

        SubscriberID
        _subscribeToRoom(const std::string &roomID, typename Room<MessageType>::MessageHandler &&handler);
    };

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
        subscribe(const std::string &roomID, const RoomManager<std::string>::MessageHandler &handler);

        void unsubscribe(const std::string &roomID, const SubscriberID &playerID);

        bool checkPassword(const std::string &roomID, const std::string &password);

        void publish(const std::string &roomID, const std::string &message);

        size_t size();

        Json::Value getRoomList(const std::string &roomType);

    private:
        std::unordered_map<std::string, uint64_t> _roomTypes;
        RoomManager<std::string> _roomManager;
        mutable SharedMutex _sharedMutex;
    };

}
