//
// Created by Parti on 2021/2/4.
//

#include "Auth.h"

void tech::api::v1::Auth::updateAuthToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;
    _service.updateAuthToken(req, code, body);
    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::Auth::getAuthToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;
    _service.getAuthToken(req, code, body);
    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::Auth::updateAccessToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;
    _service.updateAccessToken(req, code, body);
    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::Auth::getAccessToken(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;
    _service.getAccessToken(req, code, body);
    utils::httpResponse::json(code, body, callback);
}
