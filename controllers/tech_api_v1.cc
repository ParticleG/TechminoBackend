#define EXPIRATION_SECONDS (4 * 24 * 3600)

#include "tech_api_v1.h"

using namespace tech::api::v1;

//add definition of your processing function here
void App::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    try {
        auto clientPtr = drogon::app().getDbClient();
        auto matchedApps = clientPtr->execSqlSync("select * from app");
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        jsonResponse.body["version_code"] = matchedApps[matchedApps.size() - 1]["version_code"].as<int>();
        jsonResponse.body["version_name"] = matchedApps[matchedApps.size() - 1]["version_name"].as<std::string>();
        jsonResponse.body["notice"] = matchedApps[matchedApps.size() - 1]["notice"].as<std::string>();
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Database corrupted!";
    }
    jsonResponse.send(callback);
}

void Users::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                username = (*requestBody)["username"].asString(),
                password = (*requestBody)["password"].asString();
        try {
            auto clientPtr = drogon::app().getDbClient();
            auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", email);
            if (!matchedUsers.empty()) {
                jsonResponse.code = k403Forbidden;
                jsonResponse.body["message"] = "Email already used";
            } else {
                std::string passwordDigest = _sha256(password);
                std::cout << passwordDigest << std::endl;

                clientPtr->execSqlSync("insert into users (email, username, password) values ($1, $2, $3)",
                                       email,
                                       username,
                                       passwordDigest);
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(jsonResponse.body);
    httpJsonResponse->setStatusCode(jsonResponse.code);
    callback(httpJsonResponse);
}

void Users::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        int id = (*requestBody)["id"].asInt();
        std::string auth_token = (*requestBody)["auth_token"].asString(),
                email = (*requestBody)["email"].asString(),
                password = (*requestBody)["password"].asString();
        if (!auth_token.empty() && id != 0) {
            _refreshToken(jsonResponse, id, auth_token);
        } else {
            _updateToken(jsonResponse, email, password);
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(jsonResponse.body);
    httpJsonResponse->setStatusCode(jsonResponse.code);
    callback(httpJsonResponse);
}

void Users::modify(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto clientPtr = drogon::app().getDbClient();
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string
                id = (*requestBody)["id"].asString(),
                auth_token = (*requestBody)["auth_token"].asString(),
                password = (*requestBody)["password"].asString(),
                newPassword = (*requestBody)["new_password"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where id = $1", id);
            if (result.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Token expired";
            } else if (_sha256(password) != result[0]["password"].as<std::string>()) {
                jsonResponse.code = k403Forbidden;
                jsonResponse.body["message"] = "Wrong password";
            } else {
                std::string new_token = drogon::utils::getUuid();
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                jsonResponse.body["auth_token"] = new_token;
                result = clientPtr->execSqlSync("update users set password = $1, auth_token = $2 where id = $3",
                                                _sha256(newPassword),
                                                new_token,
                                                id);
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(jsonResponse.body);
    httpJsonResponse->setStatusCode(jsonResponse.code);
    callback(httpJsonResponse);
}

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
            jsonResponse.body["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(jsonResponse.body);
    httpJsonResponse->setStatusCode(jsonResponse.code);
    callback(httpJsonResponse);
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
            jsonResponse.body["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(jsonResponse.body);
    httpJsonResponse->setStatusCode(jsonResponse.code);
    callback(httpJsonResponse);
}

std::string Users::_sha256(const std::string &srcStr) {
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

void Users::_refreshToken(JsonResponse &jsonResponse, int id, const std::string &auth_token) {
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from users where id = $1", id);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "User not found";
            return;
        }
        if (!matchedUsers[0]["enable"].as<bool>()) {
            jsonResponse.code = k451UnavailableForLegalReasons;
            jsonResponse.body["message"] = "User unavailable";
            return;
        }
        if (auth_token != matchedUsers[0]["auth_token"].as<std::string>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Wrong auth_token";
            return;
        }
        if (trantor::Date::now() >
            trantor::Date::fromDbStringLocal(matchedUsers[0]["auth_token_expire_time"].as<std::string>())) {
            jsonResponse.code = k401Unauthorized;
            jsonResponse.body["message"] = "Expired Token";
            return;
        }
        clientPtr->execSqlSync("update users set auth_token_expire_time = $1 where id = $2",
                               trantor::Date::now().after(EXPIRATION_SECONDS).toDbStringLocal(),
                               id);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Database corrupted!";
    }
}

void Users::_updateToken(JsonResponse &jsonResponse, std::string email, const std::string &password) {
    try {
        auto clientPtr = app().getDbClient();
        auto matchedUsers = clientPtr->execSqlSync("select * from users where email = $1", email);
        if (matchedUsers.empty()) {
            jsonResponse.code = k404NotFound;
            jsonResponse.body["message"] = "Email not found";
            return;
        }
        if (!matchedUsers[0]["enable"].as<bool>()) {
            jsonResponse.code = k451UnavailableForLegalReasons;
            jsonResponse.body["message"] = "User unavailable";
            return;
        }
        if (_sha256(password) != matchedUsers[0]["password"].as<std::string>()) {
            jsonResponse.code = k403Forbidden;
            jsonResponse.body["message"] = "Wrong password";
            return;
        }
        std::string newToken = drogon::utils::getUuid(), auth_token_expire_time = trantor::Date::now().toDbStringLocal();
        int id = matchedUsers[0]["id"].as<int>();
        clientPtr->execSqlSync("update users set auth_token = $1, auth_token_expire_time = $2 where id = $3",
                               newToken,
                               auth_token_expire_time,
                               id);
        jsonResponse.code = k200OK;
        jsonResponse.body["message"] = "OK";
        jsonResponse.body["auth_token"] = newToken;
        jsonResponse.body["id"] = id;
    } catch (const orm::DrogonDbException &e) {
        jsonResponse.code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        jsonResponse.body["message"] = "Database corrupted!";
    }
}

