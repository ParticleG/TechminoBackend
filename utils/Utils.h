//
// Created by Parti on 2021/1/31.
//

#pragma once

#include <json/json.h>
#include <string>

namespace tech::utils {
    class Utils {
    public:
        static std::string jsonToPlainText(const Json::Value &value);

        static unsigned long uniform_random(unsigned int size = 99999999);
    };
}
