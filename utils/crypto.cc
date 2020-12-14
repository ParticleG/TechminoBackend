//
// Created by Parti on 2020/12/7.
//

#include "crypto.h"

using namespace tech::utils;

std::string Crypto::md5(const std::string &srcStr) {
    unsigned char mdStr[33] = {0};
    MD5((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);

    char buf[65] = {0};
    char tmp[3] = {0};
    for (int i = 0; i < 32; i++) {
        sprintf_s(tmp, "%02x", mdStr[i]);
        strcat_s(buf, tmp);
    }
    buf[32] = '\0';
    return std::string(buf);
}

std::string Crypto::sha256(const std::string &srcStr) {
    unsigned char mdStr[33] = {0};
    SHA256((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);
    char buf[65] = {0};
    char tmp[3] = {0};
    for (int i = 0; i < 32; i++) {
        sprintf_s(tmp, "%02x", mdStr[i]);
        strcat_s(buf, tmp);
    }
    buf[32] = '\0';
    return std::string(buf);
}
