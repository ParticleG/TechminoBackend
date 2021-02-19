//
// Created by Parti on 2021/2/16.
//

#include <services/App_WS.h>

using namespace tech::services::websocket;
using namespace drogon_model;
using namespace drogon;
using namespace std;

App::App() {
    appMapper = make_shared<orm::Mapper<Techmino::App>>(app().getDbClient());
    messageMapper = make_shared<orm::Mapper<Techmino::Message>>(app().getDbClient());
}

bool App::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const int &versionCode,
        CloseCode &code,
        Json::Value &response
) {

}

CloseCode App::requestHandler(
        const WebSocketConnectionPtr &wsConnPtr,
        const Json::Value &request,
        Json::Value &response
) {
    CloseCode closeCode{};

    return closeCode;
}
