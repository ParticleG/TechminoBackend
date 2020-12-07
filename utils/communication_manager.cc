#include "communication_manager.h"

using namespace tech::utils;

void CommunicationManager::initAndStart(const uint16_t &port, const std::string &name, const size_t &ioLoopNum) {
    _tcpServer = new TcpServer(
            drogon::app().getLoop(),
            InetAddress(port),
            name,
            true,
            true
    );

    _tcpServer->setRecvMessageCallback([](const TcpConnectionPtr &connectionPtr, MsgBuffer *buffer) {
        // LOG_DEBUG<<"recv callback!";
        // std::cout << std::string(buffer->peek(), buffer->readableBytes()) << std::endl;
        connectionPtr->send(buffer->peek(), buffer->readableBytes());
        buffer->retrieveAll();
        // connectionPtr->forceClose();
    });

    _tcpServer->setConnectionCallback([](const TcpConnectionPtr &connPtr) {
        if (connPtr->connected()) {
            // LOG_DEBUG << "New connection";
        } else if (connPtr->disconnected()) {
            // LOG_DEBUG << "connection disconnected";
        }
    });
}