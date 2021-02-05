//
// Created by Parti on 2021/2/4.
//

#include <services/Auth.h>
#include <utils/Authorizer.h>
#include <utils/Crypto.h>
#include <plugins/Configurator.h>

using namespace tech::services;
using namespace tech::utils;
using namespace drogon_model;
using namespace drogon;
using namespace std;

Auth::Auth() {
    authMapper = make_shared<orm::Mapper<Techmino::Auth>>(app().getDbClient());
}

void Auth::updateAuthToken(
        const std::string &email,
        const std::string &authToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::authToken(email, authToken, code, responseBody)) {
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

void Auth::getAuthToken(
        const std::string &email,
        const std::string &password,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::password(email, password, code, responseBody)) {
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

void Auth::updateAccessToken(
        const std::string &email,
        const std::string &accessToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::accessToken(email, accessToken, code, responseBody)) {
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


void Auth::getAccessToken(
        const std::string &email,
        const std::string &authToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::authToken(email, authToken, code, responseBody)) {
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