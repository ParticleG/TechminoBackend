#include <plugins/Configurator.h>

using namespace tech::utils;

void CommunicationManager::initAndStart(const uint16_t &port, const std::string &name, const size_t &ioLoopNum) {
    _tcpServer = new TcpServer(
            drogon::app().getLoop(),
            InetAddress(port),
            name,
            true,
            false
    );

    _tcpServer->setRecvMessageCallback([](const TcpConnectionPtr &connectionPtr, MsgBuffer *buffer) {
        std::vector<std::string> parameters;
        auto player = connectionPtr->getContext<Player>();
        std::string message(buffer->peek(), buffer->readableBytes()), response;
        std::cout << std::string(buffer->peek(), buffer->readableBytes()) << std::endl;
        buffer->retrieveAll();
        _split(message, parameters, '&');
        if (player->_email.empty() && _authorization(player, parameters, response)) {
            auto *roomManager = app().getPlugin<tech::plugin::Configurator>();
            try {
                if (!roomManager->checkPassword(player->_roomID, player->_roomPassword)) {
                    response = "Password is incorrect";
                    connectionPtr->send(response);
                    connectionPtr->forceClose();
                    return;
                }
            } catch (std::out_of_range &e) {
                LOG_ERROR << "error:" << e.what();
                response = "Room not found";
                connectionPtr->send(response);
                connectionPtr->forceClose();
                return;
            }
            try {
                player->_subscriberID = roomManager->subscribe(player->_roomID,
                                                               [connectionPtr](const std::string &topic,
                                                                               const std::string &message) {
                                                                   connectionPtr->send(message);
                                                               });
            } catch (std::range_error &e) {
                LOG_ERROR << "error:" << e.what();
                response = "Room is full";
                connectionPtr->send(response);
                connectionPtr->forceClose();
                return;
            } catch (std::out_of_range &e) {
                LOG_ERROR << "error:" << e.what();
                response = "Room is not available for now";
                connectionPtr->send(response);
                connectionPtr->forceClose();
                return;
            }
        } else {
            connectionPtr->send(response);
            connectionPtr->forceClose();
        }
    });

    _tcpServer->setConnectionCallback([](const TcpConnectionPtr &connectionPtr) {
        if (connectionPtr->connected()) {
            Player player;
            connectionPtr->setContext(std::make_shared<Player>(std::move(player)));
        } else if (connectionPtr->disconnected()) {
            auto player = connectionPtr->getContext<Player>();
            if (player->_subscriberID != -1) {
                auto *roomManager = app().getPlugin<tech::plugin::Configurator>();
                roomManager->unsubscribe(player->_roomID, player->_subscriberID);
            }
            connectionPtr->clearContext();
        }
    });
}

void CommunicationManager::_split(const std::string &source, std::vector<std::string> &outputs,
                                  const char &delimiter) {
    std::istringstream stream(source);
    std::string tempParam;
    while (std::getline(stream, tempParam, delimiter)) {
        outputs.push_back(tempParam);
    }
}

bool CommunicationManager::_authorization(const std::shared_ptr<Player> &player, const std::vector<std::string> &params,
                                          std::string &response) {
    if (params.size() != 4) {
        response = "Parameter count must be equal to 4";
        return false;
    }
    player->_email = params[0];
    player->_accessToken = params[1];
    player->_roomID = params[2];
    player->_roomPassword = params[3];
    if (player->_email.empty() || player->_accessToken.empty() || player->_roomID.empty()) {
        response = "Missing parameter(s)";
        return false;
    }
    try {
        auto clientPtr = app().getDbClient();
        auto matchedAuths = clientPtr->execSqlSync("select * from auth where email = $1", player->_email);
        if (matchedAuths.empty()) {
            response = "User not found";
            return false;
        }
        if (player->_accessToken != matchedAuths[0]["access_token"].as<std::string>()) {
            response = "Access_token is incorrect";
            return false;
        }
        if (trantor::Date::now() >
            trantor::Date::fromDbStringLocal(matchedAuths[0]["access_token_expire_time"].as<std::string>())) {
            response = "Access_token is expired";
            return false;
        }
        auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", player->_email);
        player->_id = matchedUsers[0]["_id"].as<int>();
        player->_name = matchedUsers[0]["username"].as<std::string>();
        return true;
    } catch (const orm::DrogonDbException &e) {
        LOG_ERROR << "error:" << e.base().what();
        response = "Internal error";
        return false;
    }
}
