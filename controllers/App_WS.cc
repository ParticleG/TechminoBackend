//
// Created by Parti on 2021/2/16.
//

#include "controllers/App_WS.h"
#include "services/App_WS.h"

using namespace tech::socket::v1;
using namespace std;

App::App() {
    _service = make_shared<tech::services::websocket::App>();
}