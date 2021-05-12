//
// Created by Parti on 2021/2/3.
//

#include <controllers/User_HTTP.h>
#include <utils/websocket.h>

using namespace tech::api::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void User::validateAccount(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    http::fromJson(code, _service.validateAccount(code, request), callback);
}

void User::loginAccount(const HttpRequestPtr &req, function<void(const drogon::HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    vector<Cookie> cookies;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    request["uid"] = req->getCookie("uid");
    request["webToken"] = req->getCookie("webToken");
    response = _service.loginAccount(code, request, cookies);
    if (cookies.empty()) {
        http::fromJson(code, response, callback);
    } else {
        http::fromJson(code, response, cookies, callback);
    }
}

void User::profileInfo(const HttpRequestPtr &req, function<void(const drogon::HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    http::fromJson(code, _service.profileInfo(code, request), callback);
}

void User::updateInfo(const HttpRequestPtr &req, function<void(const drogon::HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    http::fromJson(code, _service.updateInfo(code, request), callback);
}

void User::profileAvatar(const HttpRequestPtr &req, function<void(const drogon::HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    http::fromJson(code, _service.profileAvatar(code, request), callback);
}

void User::updateAvatar(const HttpRequestPtr &req, function<void(const drogon::HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request, response;
    string parseError = http::toJson(req, request);
    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, callback);
        return;
    }
    http::fromJson(code, _service.updateAvatar(code, request), callback);
}
