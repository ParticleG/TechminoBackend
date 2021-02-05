//
// Created by Parti on 2021/2/4.
//

#include <utils/Http.h>
#include <controllers/Play_HTTP.h>

using namespace tech::api::v1;
using namespace tech::utils;

void online::Play::list(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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
        _service.list(email, accessToken, code, responseBody);
    }
    utils::Http::fromJson(code, responseBody, callback);
}

void online::Play::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                        const std::string &roomType) {
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
        _service.info(email, accessToken, roomType, code, responseBody);
    }
    utils::Http::fromJson(code, responseBody, callback);
}

void online::Play::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                          const std::string &roomType) {
    HttpStatusCode code;
    Json::Value requestBody, responseBody;
    string parseError = Http::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                accessToken = requestBody["access_token"].asString(),
                roomName = requestBody["room_name"].asString(),
                roomPassword = requestBody["room_password"].asString();
        _service.create(email, accessToken, roomType, roomName, roomPassword, code, responseBody);
    }
    utils::Http::fromJson(code, responseBody, callback);
}
