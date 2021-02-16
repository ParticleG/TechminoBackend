//
// Created by Parti on 2021/1/31.
//

#include <chrono>
#include <random>
#include <utils/Utils.h>

unsigned long tech::utils::Utils::uniform_random(unsigned int size) {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0, size);
    return dis(generator);
}
