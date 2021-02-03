#define EXPIRATION_AUTH_TOKEN (4 * 24 * 3600)
#define EXPIRATION_ACCESS_TOKEN (30 * 60)

#include <plugins/Configurator.h>
#include "api.h"



void Auth::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                auth_token = (*requestBody)["auth_token"].asString(),
                password = (*requestBody)["password"].asString();
        try {
            auto clientPtr = app().getDbClient();
            auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", email);
            if (!auth_token.empty()) {
                _refreshToken(jsonResponse, email, auth_token);
            } else if (_updateToken(jsonResponse, email, password)) {
                jsonResponse.body["id"] = matchedUsers[0]["_id"].as<Json::Int64>();
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.respond(callback);
}

void Auth::checkAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                accessToken = (*requestBody)["access_token"].asString();
        try {
            auto clientPtr = app().getDbClient();
            auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
            if (matchedUsers.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (accessToken != matchedUsers[0]["access_token"].as<std::string>()) {
                jsonResponse.code = k403Forbidden;
                jsonResponse.body["message"] = "Access_token is incorrect";
            } else if (trantor::Date::now() > trantor::Date::fromDbStringLocal(
                    matchedUsers[0]["access_token_expire_time"].as<std::string>())) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Access_token is expired";
            } else {
                clientPtr->execSqlSync(
                        "modify auth set access_token_expire_time = $1 where email = $2",
                        trantor::Date::now().after(EXPIRATION_ACCESS_TOKEN).toDbStringLocal(),
                        email);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
            }
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.code = k500InternalServerError;
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.respond(callback);
}

void Auth::getAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                authToken = (*requestBody)["auth_token"].asString();
        try {
            auto clientPtr = app().getDbClient();
            auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
            if (matchedUsers.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (authToken != matchedUsers[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k403Forbidden;
                jsonResponse.body["message"] = "Auth_token is incorrect";
            } else if (trantor::Date::now() >
                       trantor::Date::fromDbStringLocal(matchedUsers[0]["auth_token_expire_time"].as<std::string>())) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Auth_token is expired";
            } else {
                std::string newToken = tech::utils::Crypto::keccak(drogon::utils::getUuid());
                clientPtr->execSqlSync(
                        "modify auth set access_token = $1, access_token_expire_time = $2 where email = $3",
                        newToken,
                        trantor::Date::now().after(EXPIRATION_ACCESS_TOKEN).toDbStringLocal(),
                        email);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["email"] = email;
                jsonResponse.body["access_token"] = newToken;
            }
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.code = k500InternalServerError;
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.respond(callback);
}

bool Auth::_refreshToken(JsonResponse &jsonResponse, const std::string &email, const std::string &auth_token) {
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "User not found";
            return false;
        }
        if (auth_token != matchedUsers[0]["auth_token"].as<std::string>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Auth_token is incorrect";
            return false;
        }
        if (trantor::Date::now() >
            trantor::Date::fromDbStringLocal(matchedUsers[0]["auth_token_expire_time"].as<std::string>())) {
            jsonResponse.code = k401Unauthorized;
            jsonResponse.body["message"] = "Auth_token is expired";
            return false;
        }
        clientPtr->execSqlSync("modify auth set auth_token_expire_time = $1 where email = $2",
                               trantor::Date::now().after(EXPIRATION_AUTH_TOKEN).toDbStringLocal(),
                               email);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        return true;
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Internal error";
        return false;
    }
}

bool Auth::_updateToken(JsonResponse &jsonResponse, const std::string &email, const std::string &password) {
    try {
        auto clientPtr = app().getDbClient();
        auto result = clientPtr->execSqlSync("select crypt($1, password) = password "
                                             "from auth "
                                             "where email = $2",
                                             password, email);
        if (result.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "Email not found";
            return false;
        }
        if (!result[0]["?column?"].as<bool>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Password is incorrect";
            return false;
        }
        std::string newToken = drogon::utils::getUuid();
        clientPtr->execSqlSync("modify auth set auth_token = $1, auth_token_expire_time = $2 where email = $3",
                               newToken,
                               trantor::Date::now().after(EXPIRATION_AUTH_TOKEN).toDbStringLocal(),
                               email);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        jsonResponse.body["email"] = email;
        jsonResponse.body["auth_token"] = newToken;
        return true;
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Internal error";
        return false;
    }
}

void online::Rooms::list(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                accessToken = (*requestBody)["access_token"].asString();
        if (_validate(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::Configurator>();
                auto roomList = roomManager->getRoomList();
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["room_list"] = roomList;
            } catch (const std::out_of_range &e) {
                LOG_ERROR << "error:" << e.what();
                jsonResponse.code = k416RequestedRangeNotSatisfiable;
                jsonResponse.body["message"] = "Unsupported room_type";
            }
        }
    }
    jsonResponse.respond(callback);
}

void online::Rooms::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                         const std::string &roomType) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                accessToken = (*requestBody)["access_token"].asString();
        if (_validate(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::Configurator>();
                auto roomList = roomManager->getRoomList(roomType);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["room_list"] = roomList;
            } catch (const std::out_of_range &e) {
                LOG_ERROR << "error:" << e.what();
                jsonResponse.code = k416RequestedRangeNotSatisfiable;
                jsonResponse.body["message"] = "Unsupported room_type";
            }
        }
    }
    jsonResponse.respond(callback);
}

void online::Rooms::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                           const std::string &roomType) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                accessToken = (*requestBody)["access_token"].asString(),
                roomName = (*requestBody)["room_name"].asString(),
                roomPassword = (*requestBody)["room_password"].asString();
        if (_validate(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::Configurator>();
                auto tempRoom = roomManager->createRoom("room_" + drogon::utils::getUuid(), roomName, roomPassword,
                                                        roomType);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["room"] = tempRoom;
            } catch (const std::out_of_range &e) {
                LOG_ERROR << "error:" << e.what();
                jsonResponse.code = k416RequestedRangeNotSatisfiable;
                jsonResponse.body["message"] = "Unsupported room_type";
            }
        }
    }
    jsonResponse.respond(callback);
}

bool
online::Rooms::_validate(JsonResponse &jsonResponse, const std::string &email, const std::string &accessToken) {
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "User not found";
            return false;
        }
        if (accessToken != matchedUsers[0]["access_token"].as<std::string>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Access_token is incorrect";
            return false;
        }
        if (trantor::Date::now() > trantor::Date::fromDbStringLocal(
                matchedUsers[0]["access_token_expire_time"].as<std::string>())) {
            jsonResponse.code = k401Unauthorized;
            jsonResponse.body["message"] = "Access_token is expired";
            return false;
        }
        return true;
    } catch (const orm::DrogonDbException &e) {
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.code = k500InternalServerError;
        jsonResponse.body["message"] = "Internal error";
        return false;
    }
}


