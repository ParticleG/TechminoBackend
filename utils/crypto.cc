//
// Created by Parti on 2020/12/7.
//

#include "crypto.h"

using namespace tech::utils;

std::string Crypto::md5(const std::string &srcStr) {
    unsigned char mdStr[33] = {0};
    MD5((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);
    std::ostringstream oss;
    for (int i = 0; i < 32; i++) {
        oss << std::hex << static_cast<unsigned short>(mdStr[i]);
    }
    return oss.str();
}

std::string Crypto::sha256(const std::string &srcStr) {
    unsigned char mdStr[33] = {0};
    SHA256((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);
    std::ostringstream oss;
    for (int i = 0; i < 32; i++) {
        oss << std::hex << static_cast<unsigned short>(mdStr[i]);
    }
    return oss.str();
}
