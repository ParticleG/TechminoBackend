//
// Created by Parti on 2021/2/23.
//

#pragma once

#include <services/Base_WS.h>

namespace tech::socket::v1 {
    template<class controllerType, class serviceType>
    class Base : public drogon::WebSocketController<controllerType> {
    public:
        virtual void handleNewMessage(
                const drogon::WebSocketConnectionPtr &wsConnPtr,
                std::string &&message,
                const drogon::WebSocketMessageType &type
        ) {
            if (type == drogon::WebSocketMessageType::Ping) {
                wsConnPtr->send(message, drogon::WebSocketMessageType::Pong);
            } else if (type == drogon::WebSocketMessageType::Text || type == drogon::WebSocketMessageType::Binary) {
                LOG_DEBUG << "New message: " << message;
                Json::Value request, response;
                std::string parseError = tech::utils::websocket::toJson(message, request);
                if (!parseError.empty()) {
                    response["type"] = "Warn";
                    response["reason"] = "Wrong format: " + parseError;
                    wsConnPtr->send(tech::utils::websocket::fromJson(response));
                } else {
                    drogon::CloseCode code = _service.requestHandler(wsConnPtr, request, response);
                    if (code == drogon::CloseCode::kNormalClosure) {
                        wsConnPtr->send(tech::utils::websocket::fromJson(response));
                    } else if (code != drogon::CloseCode::kNone) {
                        tech::utils::websocket::close(wsConnPtr, code, tech::utils::websocket::fromJson(response));
                    }
                }
            } else if (type == drogon::WebSocketMessageType::Close) {
                wsConnPtr->forceClose();
            } else if (type == drogon::WebSocketMessageType::Unknown) {
                LOG_DEBUG << "Message is Unknown";
            } else if (type != drogon::WebSocketMessageType::Pong) {
                LOG_DEBUG << "Message is " << static_cast<int>(type) << ": " << message;
            }
        }

        virtual void handleNewConnection(
                const drogon::HttpRequestPtr &req,
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) {
            _service.establish(wsConnPtr, req->getAttributes());
        }

        virtual void handleConnectionClosed(
                const drogon::WebSocketConnectionPtr &wsConnPtr
        ) {
            _service.close(wsConnPtr);
        }

        virtual ~Base() = default;

    protected:
        serviceType _service{};
    };
}
