#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>

using namespace drogon;
namespace tech::socket::v1 {
    struct Player {
        std::string _email{}, _accessToken{}, _name{}, _roomID{}, _roomPassword{};
        drogon::SubscriberID _subscriberID{};
    };

    struct WSCloser {
        CloseCode _code;
        std::string _reason;

        void close(const WebSocketConnectionPtr &wsConnPtr) {
            wsConnPtr->shutdown(_code, _reason);
        }
    };

    bool authorization(WSCloser &wsCloser, Player &player) {
        if (player._email.empty() || player._accessToken.empty() || player._roomID.empty()) {
            wsCloser._code = CloseCode::kInvalidMessage;
            wsCloser._reason = "Invalid parameters";
            return false;
        }
        try {
            auto clientPtr = app().getDbClient();
            auto matchedAuths = clientPtr->execSqlSync("select * from auth where email = $1", player._email);
            if (matchedAuths.empty()) {
                wsCloser._code = CloseCode::kInvalidMessage;
                wsCloser._reason = "User not found";
                return false;
            }
            if (player._accessToken != matchedAuths[0]["access_token"].as<std::string>()) {
                wsCloser._code = CloseCode::kInvalidMessage;
                wsCloser._reason = "Access_token is incorrect";
                return false;
            }
            if (trantor::Date::now() >
                trantor::Date::fromDbStringLocal(matchedAuths[0]["access_token_expire_time"].as<std::string>())) {
                wsCloser._code = CloseCode::kInvalidMessage;
                wsCloser._reason = "Access_token is expired";
                return false;
            }
            auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", player._email);
            player._name = matchedUsers[0]["username"].as<std::string>();
            return true;
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            wsCloser._code = CloseCode::kUnexpectedCondition;
            wsCloser._reason = "Database corrupted!";
            return false;
        }
    }

    class Chat : public drogon::WebSocketController<Chat> {
    public:
        virtual void handleNewMessage(const WebSocketConnectionPtr &,
                                      std::string &&,
                                      const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &,
                                         const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/chat");
        WS_PATH_LIST_END
    };

    class Solo : public drogon::WebSocketController<Solo> {
    public:
        virtual void handleNewMessage(const WebSocketConnectionPtr &,
                                      std::string &&,
                                      const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &,
                                         const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/solo");
        WS_PATH_LIST_END
    };

    class SmallRoom : public drogon::WebSocketController<SmallRoom> {
    public:
        virtual void handleNewMessage(const WebSocketConnectionPtr &,
                                      std::string &&,
                                      const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &,
                                         const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/small_room");
        WS_PATH_LIST_END
    };

    class LargeRoom : public drogon::WebSocketController<LargeRoom> {
    public:
        virtual void handleNewMessage(const WebSocketConnectionPtr &,
                                      std::string &&,
                                      const WebSocketMessageType &) override;

        virtual void handleNewConnection(const HttpRequestPtr &,
                                         const WebSocketConnectionPtr &) override;

        virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/tech/socket/v1/LargeRoom");
        WS_PATH_LIST_END
    };
}
