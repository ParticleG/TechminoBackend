//
// Created by Parti on 2021/2/5.
//

#include "controllers/Chat_WS.h"
#include "services/Chat_WS.h"

using namespace tech::socket::v1;
using namespace std;

Chat::Chat() {
    _service = make_shared<tech::services::websocket::Chat>();
}