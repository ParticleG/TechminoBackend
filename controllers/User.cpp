//
// Created by Parti on 2021/2/4.
//

#include "User.h"

void tech::api::v1::User::create(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;

    _service.create(req, code, body);
    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::User::info(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;


    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::User::modify(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;


    utils::httpResponse::json(code, body, callback);
}

void tech::api::v1::User::erase(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;


    utils::httpResponse::json(code, body, callback);
}
