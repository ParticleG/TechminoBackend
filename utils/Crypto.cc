//
// Created by Parti on 2020/12/7.
//

#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/blake2.h>
#include <cryptopp/keccak.h>
#include <sstream>
#include <utils/Crypto.h>

using namespace tech::utils;
using namespace CryptoPP;
using namespace std;

string Crypto::blake2b(const string &source, const unsigned int &divider) {
    stringstream tempStringStream;
    HexEncoder encoder(new FileSink(tempStringStream));
    string digest;
    BLAKE2b hash;
    hash.Update(reinterpret_cast<const CryptoPP::byte *>(source.data()), source.size());
    digest.resize(hash.DigestSize() / divider);
    hash.TruncatedFinal(reinterpret_cast<CryptoPP::byte *>(&digest[0]), digest.size());
    StringSource tempSource(digest, true, new Redirector(encoder));
    return tempStringStream.str();
}

string Crypto::keccak(const string &source, const unsigned int &divider) {
    stringstream tempStringStream;
    HexEncoder encoder(new FileSink(tempStringStream));
    string digest;
    Keccak_512 hash;
    hash.Update(reinterpret_cast<const CryptoPP::byte *>(source.data()), source.size());
    digest.resize(hash.DigestSize() / divider);
    hash.TruncatedFinal(reinterpret_cast<CryptoPP::byte *>(&digest[0]), digest.size());
    StringSource tempSource(digest, true, new Redirector(encoder));
    return tempStringStream.str();
}