//
// Created by Parti on 2021/2/4.
//

#include <services/Play_HTTP.h>
#include <utils/Authorizer.h>
#include <utils/Crypto.h>

using namespace tech::services;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void Play::list(
        const string &email,
        const string &accessToken,
        HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::accessToken(email, accessToken, code, responseBody)) {
        try {
            auto roomList = _playManager->getRoomList();
            code = k200OK;
            responseBody["message"] = "OK";
            responseBody["room_list"] = roomList;
        } catch (const out_of_range &e) {
            LOG_ERROR << "error:" << e.what();
            code = k416RequestedRangeNotSatisfiable;
            responseBody["message"] = "Unsupported room_type";
        }
    }
}

void Play::info(
        const string &email,
        const string &accessToken,
        const string &roomType,
        HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::accessToken(email, accessToken, code, responseBody)) {
        try {
            auto roomList = _playManager->getRoomList(roomType);
            code = k200OK;
            responseBody["message"] = "OK";
            responseBody["room_list"] = roomList;
        } catch (const out_of_range &e) {
            LOG_ERROR << "error:" << e.what();
            code = k416RequestedRangeNotSatisfiable;
            responseBody["message"] = "Unsupported room_type";
        }
    }
}

void Play::create(
        const string &email,
        const string &accessToken,
        const string &roomType,
        const string &roomName,
        const string &roomPassword,
        HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::accessToken(email, accessToken, code, responseBody)) {
        try {
            string roomID = Crypto::blake2b("room_" + drogon::utils::getUuid());
            auto tempRoom = _playManager->createRoom(roomID, roomName, roomPassword, roomType);
            code = k200OK;
            responseBody["message"] = "OK";
            responseBody["room"] = tempRoom;
        } catch (const out_of_range &e) {
            LOG_ERROR << "error:" << e.what();
            code = k416RequestedRangeNotSatisfiable;
            responseBody["message"] = "Unsupported room_type";
        }
    }
}
