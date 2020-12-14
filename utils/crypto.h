//
// Created by Parti on 2020/12/7.
//

#pragma once
#include <string>
#include <openssl/sha.h>
#include <openssl/md5.h>

namespace tech::utils{
    class Crypto {
    public:
        static std::string md5(const std::string &srcStr);
        static std::string sha256(const std::string &srcStr);
    };
}

