//
// Created by Parti on 2021/1/31.
//
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <json/json.h>

namespace tech::utils {
    class utils {
    public:
        static std::string jsonToPlainText(const Json::Value &value);

        static unsigned long uniform_random();
    };
}
