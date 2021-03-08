//
// Created by Parti on 2021/2/16.
//

#include "controllers/User_WS.h"
#include "services/User_WS.h"

using namespace tech::socket::v1;
using namespace std;

User::User() {
    _service = make_shared<tech::services::websocket::User>();
}
