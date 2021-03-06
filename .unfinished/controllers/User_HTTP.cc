//
// Created by Parti on 2021/2/4.
//

#include <.unfinished/controllers/User_HTTP.h>

using namespace tech::api::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void User::create(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        _service.create(code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void User::info(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["auth_token"].asString();
        _service.info(email, authToken, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void User::modify(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["auth_token"].asString(),
                username = requestBody["username"].asString(),
                motto = requestBody["motto"].asString(),
                avatar = requestBody["avatar"].asString();
        _service.modify(email, authToken, username, motto, avatar, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void User::erase(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        _service.erase(code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}
