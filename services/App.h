//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <utils/httpResponse.h>
#include <models/App.h>
#include <models/Message.h>

namespace tech::services {
    class App {
    public:
        void info(HttpStatusCode &code, Json::Value &body);
    };
}
