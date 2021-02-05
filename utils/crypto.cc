//
// Created by Parti on 2020/12/7.
//

#include "crypto.h"

using namespace tech::utils;

std::string Crypto::blake2b(const std::string &source, const unsigned int &divider) {
    using namespace CryptoPP;
    std::stringstream tempStringStream;
    HexEncoder encoder(new FileSink(tempStringStream));
    std::string digest;
    BLAKE2b hash;
    hash.Update((const byte *) source.data(), source.size());
    digest.resize(hash.DigestSize() / divider);
    hash.TruncatedFinal((byte *) &digest[0], digest.size());
    StringSource tempSource(digest, true, new Redirector(encoder));
    return tempStringStream.str();
}

std::string Crypto::keccak(const std::string &source, const unsigned int &divider) {
    using namespace CryptoPP;
    std::stringstream tempStringStream;
    HexEncoder encoder(new FileSink(tempStringStream));
    std::string digest;
    Keccak_512 hash;
    hash.Update((const byte *) source.data(), source.size());
    digest.resize(hash.DigestSize() / divider);
    hash.TruncatedFinal((byte *) &digest[0], digest.size());
    StringSource tempSource(digest, true, new Redirector(encoder));
    return tempStringStream.str();
}