//
// Created by Parti on 2021/2/5.
//

#include "controllers/Stream_WS.h"
#include "services/Stream_WS.h"

using namespace tech::socket::v1;
using namespace std;

Stream::Stream() {
    _service = make_shared<tech::services::websocket::Stream>();
}
