//
// Created by Parti on 2021/1/31.
//

#pragma once

#include <json/json.h>
#include <string>

namespace tech::utils {
    class Utils {
    public:
        static unsigned long uniform_random(unsigned int size = 99999999);
    };
}
