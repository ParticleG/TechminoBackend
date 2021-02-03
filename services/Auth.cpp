//
// Created by Parti on 2021/2/4.
//

#include <services/Auth.h>
#include <utils/authorizer.h>
#include <utils/crypto.h>
#include <plugins/Configurator.h>

using namespace tech::services;
using namespace tech::utils;

Auth::Auth() {
    authMapper = make_shared<orm::Mapper<Techmino::Auth>>(app().getDbClient());
}

void Auth::updateAuthToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["auth_token"].asString();
        if (authorizer::authToken(email, authToken, code, responseBody)) {
            try {
                auto *configurator = app().getPlugin<tech::plugin::Configurator>();
                Techmino::Auth auth;
                auth.setEmail(email);
                auth.setAuthTokenExpireTime(trantor::Date::now().after(configurator->getAuthExpire()).toDbStringLocal());
                authMapper->update(auth);
                code = k200OK;
                responseBody["message"] = "OK";
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
            }
        }
    }
}

void Auth::getAuthToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                password = requestBody["password"].asString();
        if (authorizer::password(email, password, code, responseBody)) {
            try {
                auto *configurator = app().getPlugin<tech::plugin::Configurator>();
                Techmino::Auth auth;
                auth.setEmail(email);
                auth.setAuthToken(drogon::utils::getUuid());
                auth.setAuthTokenExpireTime(trantor::Date::now().after(configurator->getAuthExpire()).toDbStringLocal());
                authMapper->update(auth);
                code = k200OK;
                responseBody["message"] = "OK";
                responseBody["email"] = email;
                responseBody["auth_token"] = auth.getValueOfAuthToken();
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
            }
        }
    }
}

void Auth::updateAccessToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                accessToken = requestBody["access_token"].asString();
        if (authorizer::accessToken(email, accessToken, code, responseBody)) {
            try {
                auto *configurator = app().getPlugin<tech::plugin::Configurator>();
                Techmino::Auth auth;
                auth.setEmail(email);
                auth.setAccessTokenExpireTime(trantor::Date::now().after(configurator->getAccessExpire()).toDbStringLocal());
                authMapper->update(auth);
                code = k200OK;
                responseBody["message"] = "OK";
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
            }
        }
    }
}

void Auth::getAccessToken(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["auth_token"].asString();
        if (authorizer::authToken(email, authToken, code, responseBody)) {
            try {
                auto *configurator = app().getPlugin<tech::plugin::Configurator>();
                Techmino::Auth auth;
                auth.setEmail(email);
                auth.setAccessToken(Crypto::keccak(drogon::utils::getUuid()));
                auth.setAccessTokenExpireTime(trantor::Date::now().after(configurator->getAccessExpire()).toDbStringLocal());
                authMapper->update(auth);
                code = k200OK;
                responseBody["message"] = "OK";
                responseBody["email"] = email;
                responseBody["access_token"] = auth.getValueOfAccessToken();
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
            }
        }
    }
}