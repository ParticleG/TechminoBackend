//
// Created by Parti on 2021/2/4.
//

#include "".unfinished/controllers/Auth.h"

using namespace tech::api::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void Auth::updateAuthToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
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
        _service.updateAuthToken(email, authToken, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void Auth::getAuthToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                password = requestBody["password"].asString();
        _service.getAuthToken(email, password, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void Auth::updateAccessToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                accessToken = requestBody["access_token"].asString();
        _service.updateAccessToken(email, accessToken, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}

void Auth::getAccessToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
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
        _service.getAccessToken(email, authToken, code, responseBody);
    }
    Http::fromJson(code, responseBody, callback);
}
