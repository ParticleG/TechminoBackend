//
// Created by Parti on 2021/2/5.
//

#include <utils/WebSocket.h>

using namespace tech::utils;
using namespace drogon;
using namespace std;

string WebSocket::fromJson(const Json::Value &value) {
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder.settings_["indentation"] = "";
    unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    ostringstream oss;
    jsonWriter->write(value, &oss);
    return oss.str();
}

string WebSocket::toJson(
        const string &rawData,
        Json::Value &result
) {
    string errorMessage;
    auto charReader = Json::CharReaderBuilder().newCharReader();

    charReader->parse(rawData.data(), rawData.data() + rawData.size(), &result, &errorMessage);
    return errorMessage;
}

void WebSocket::initPing(
        const WebSocketConnectionPtr &wsConnPtr,
        const chrono::duration<long double> &interval
) {
    wsConnPtr->send("", WebSocketMessageType::Ping);
    wsConnPtr->setPingMessage("", interval);
}

void WebSocket::close(
        const WebSocketConnectionPtr &webSocketConnectionPtr,
        CloseCode _code,
        const string &_reason
) {
    webSocketConnectionPtr->shutdown(_code, _reason);
}
