//
// Created by Parti on 2021/2/5.
//

#include "controllers/Play_WS.h"
#include "services/Play_WS.h"

using namespace tech::socket::v1;
using namespace std;

Play::Play() {
    _service = make_shared<tech::services::websocket::Play>();
}
