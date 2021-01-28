#define EXPIRATION_AUTH_TOKEN (4 * 24 * 3600)
#define EXPIRATION_ACCESS_TOKEN (30 * 60)

#include <plugins/tech_plugin_VersusManager.h>
#include "tech_api_v1.h"

using namespace tech::api::v1;

bool tech::api::v1::authorization(JsonResponse &jsonResponse, const std::string &email, const std::string &authToken) {
    if (email.empty() || authToken.empty()) {
        jsonResponse.code = k400BadRequest;
        jsonResponse.body["message"] = "Invalid parameters";
        return false;
    }
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "User not found";
            return false;
        }
        if (authToken != matchedUsers[0]["auth_token"].as<std::string>()) {
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
        return true;
    } catch (const orm::DrogonDbException &e) {
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.code = k500InternalServerError;
        jsonResponse.body["message"] = "Internal error";
        return false;
    }
}

void App::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    try {
        auto clientPtr = drogon::app().getDbClient();
        auto matchedApps = clientPtr->execSqlSync("select * from app");
        auto matchedContents = clientPtr->execSqlSync(
                R"(select content from message where type = 'notice' order by id desc)");
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        jsonResponse.body["version_code"] = matchedApps[matchedApps.size() - 1]["version_code"].as<int>();
        jsonResponse.body["version_name"] = matchedApps[matchedApps.size() - 1]["version_name"].as<std::string>();
        jsonResponse.body["version_content"] = matchedApps[matchedApps.size() - 1]["version_content"].as<std::string>();
        jsonResponse.body["notice"] = matchedContents[0]["content"].as<std::string>();
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Internal error";
    }
    jsonResponse.send(callback);
}

//void Users::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
//    JsonResponse jsonResponse;
//    auto requestBody = req->getJsonObject();
//    if (!req->getJsonError().empty()) {
//        jsonResponse.body["message"] = "Wrong format";
//        jsonResponse.code = k400BadRequest;
//    } else {
//        std::string email = (*requestBody)["email"].asString(),
//                username = (*requestBody)["username"].asString(),
//                password = (*requestBody)["password"].asString();
//        try {
//            auto clientPtr = drogon::app().getDbClient();
//            auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
//            if (!matchedUsers.empty()) {
//                jsonResponse.code = k403Forbidden;
//                jsonResponse.body["message"] = "Email already used";
//            } else {
//                std::string passwordDigest = sha256(password);
//                clientPtr->execSqlSync("insert into auth (email, password) values ($1, $2)",
//                                       email,
//                                       passwordDigest);
//                clientPtr->execSqlSync("insert into users (email, username) values ($1, $2)",
//                                       email,
//                                       username);
//                jsonResponse.code = k200OK;
//                jsonResponse.body["message"] = "OK";
//            }
//        } catch (const orm::DrogonDbException &e) {
//            jsonResponse.code = k500InternalServerError;
//            LOG_ERROR << "error:" << e.base().what();
//            jsonResponse.body["message"] = "Internal error";
//        }
//    }
//    jsonResponse.send(callback);
//}

void Users::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                auth_token = (*requestBody)["auth_token"].asString();
        if (authorization(jsonResponse, email, auth_token)) {
            try {
                auto clientPtr = app().getDbClient();
                auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", email);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["email"] = matchedUsers[0]["email"].as<std::string>();
                jsonResponse.body["username"] = matchedUsers[0]["username"].as<std::string>();
                jsonResponse.body["motto"] = matchedUsers[0]["motto"].as<std::string>();
                jsonResponse.body["avatar"] = matchedUsers[0]["avatar"].as<std::string>();
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                jsonResponse.code = k500InternalServerError;
                jsonResponse.body["message"] = "Internal error";
                return;
            }
        }
    }
    jsonResponse.send(callback);
}

//void Users::modify(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
//    auto clientPtr = drogon::app().getDbClient();
//    JsonResponse jsonResponse;
//    auto requestBody = req->getJsonObject();
//    if (!req->getJsonError().empty()) {
//        jsonResponse.body["message"] = "Wrong format.";
//        jsonResponse.code = k400BadRequest;
//    } else {
//        std::string
//                id = (*requestBody)["id"].asString(),
//                auth_token = (*requestBody)["auth_token"].asString(),
//                password = (*requestBody)["password"].asString(),
//                newPassword = (*requestBody)["new_password"].asString();
//        try {
//            auto result = clientPtr->execSqlSync("select * from users where id = $1", id);
//            if (result.empty()) {
//                jsonResponse.code = k404NotFound;
//                jsonResponse.body["message"] = "User not found";
//            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
//                jsonResponse.code = k401Unauthorized;
//                jsonResponse.body["message"] = "Token expired";
//            } else if (tech::utils::Crypto::sha256(password) != result[0]["password"].as<std::string>()) {
//                jsonResponse.code = k403Forbidden;
//                jsonResponse.body["message"] = "Wrong password";
//            } else {
//                std::string new_token = drogon::utils::getUuid();
//                jsonResponse.code = k200OK;
//                jsonResponse.body["message"] = "OK";
//                jsonResponse.body["auth_token"] = new_token;
//                clientPtr->execSqlSync("update users set password = $1, auth_token = $2 where id = $3",
//                                       tech::utils::Crypto::sha256(newPassword),
//                                       new_token,
//                                       id);
//            }
//        } catch (const orm::DrogonDbException &e) {
//            jsonResponse.code = k500InternalServerError;
//            LOG_ERROR << "error:" << e.base().what();
//            jsonResponse.body["message"] = "Internal error";
//        }
//    }
//    jsonResponse.send(callback);
//}

void
Users::getData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId) {
    auto clientPtr = drogon::app().getDbClient();
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string auth_token = (*requestBody)["auth_token"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where id = $1", userId);
            if (result.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Token expired";
            } else {
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                //TODO: return data to the client
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.send(callback);
}

void Users::saveData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                     std::string userId) {
    auto clientPtr = drogon::app().getDbClient();
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string auth_token = (*requestBody)["auth_token"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where id = $1", userId);
            if (result.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Token expired";
            } else {
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                //TODO: save data from the client
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.send(callback);
}

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
        if (!auth_token.empty()) {
            _refreshToken(jsonResponse, email, auth_token);
        } else {
            _updateToken(jsonResponse, email, password);
        }
    }
    jsonResponse.send(callback);
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
                        "update auth set access_token_expire_time = $1 where email = $2",
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
    jsonResponse.send(callback);
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
                        "update auth set access_token = $1, access_token_expire_time = $2 where email = $3",
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
    jsonResponse.send(callback);
}

void Auth::_refreshToken(JsonResponse &jsonResponse, const std::string &email, const std::string &auth_token) {
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from auth where email = $1", email);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "User not found";
            return;
        }
        if (auth_token != matchedUsers[0]["auth_token"].as<std::string>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Auth_token is incorrect";
            return;
        }
        if (trantor::Date::now() >
            trantor::Date::fromDbStringLocal(matchedUsers[0]["auth_token_expire_time"].as<std::string>())) {
            jsonResponse.code = k401Unauthorized;
            jsonResponse.body["message"] = "Auth_token is expired";
            return;
        }
        clientPtr->execSqlSync("update auth set auth_token_expire_time = $1 where email = $2",
                               trantor::Date::now().after(EXPIRATION_AUTH_TOKEN).toDbStringLocal(),
                               email);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Internal error";
    }
}

void Auth::_updateToken(JsonResponse &jsonResponse, const std::string &email, const std::string &password) {
    try {
        auto clientPtr = app().getDbClient();
        auto result = clientPtr->execSqlSync("select crypt($1, password) = password "
                                             "from auth "
                                             "where email = $2",
                                             password, email);
        if (result.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "Email not found";
            return;
        }
        if (!result[0]["?column?"].as<bool>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Password is incorrect";
            return;
        }
        std::string newToken = drogon::utils::getUuid();
        clientPtr->execSqlSync("update auth set auth_token = $1, auth_token_expire_time = $2 where email = $3",
                               newToken,
                               trantor::Date::now().after(EXPIRATION_AUTH_TOKEN).toDbStringLocal(),
                               email);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        jsonResponse.body["id"] = result[0]["_id"].as<Json::Int64>();
        jsonResponse.body["email"] = email;
        jsonResponse.body["auth_token"] = newToken;
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Internal error";
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
        if (authorization(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
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
    jsonResponse.send(callback);
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
        if (authorization(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
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
    jsonResponse.send(callback);
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
        if (authorization(jsonResponse, email, accessToken)) {
            try {
                auto *roomManager = app().getPlugin<tech::plugin::VersusManager>();
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
    jsonResponse.send(callback);
}

bool
online::Rooms::authorization(JsonResponse &jsonResponse, const std::string &email, const std::string &accessToken) {
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


