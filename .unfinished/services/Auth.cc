//
// Created by Parti on 2021/2/4.
//

#include <.unfinished/services/Auth.h>
#include <utils/authorizer.h>
#include <utils/crypto.h>
#include <utils/misc.h>
#include <plugins/Configurator.h>

using namespace drogon;
using namespace drogon_model;
using namespace tech::plugins;
using namespace tech::services::http;
using namespace tech::utils;
using namespace std;

Auth::Auth() {
    authMapper = make_shared<orm::Mapper<Techmino::Auth>>(app().getDbClient());
}

void Auth::updateAuthToken(
        const int64_t &id,
        const std::string &authToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (authorizer::authToken(id, authToken, code, responseBody)) {
        code = k200OK;
        responseBody["message"] = "OK";
    }
}

void Auth::getAuthToken(
        const std::string &email,
        const std::string &password,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (authorizer::password(email, password, code, responseBody)) {
        code = k200OK;
        responseBody["message"] = "OK";
    }
}

void Auth::updateAccessToken(
        const int64_t &id,
        const std::string &accessToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (authorizer::accessToken(id, accessToken, code, responseBody)) {
        code = k200OK;
        responseBody["message"] = "OK";
    }
}

void Auth::getAccessToken(
        const int64_t &id,
        const std::string &authToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (authorizer::authToken(id, authToken, code, responseBody)) {
        try {
            auto *configurator = app().getPlugin<Configurator>();
            Techmino::Auth auth;
            auth.setId(id);
            auth.setAccessToken(Crypto::keccak(drogon::utils::getUuid()));
            auth.setAccessTokenExpireTime(Utils::fromDate(configurator->getAccessExpire()));
            authMapper->update(auth);
            code = k200OK;
            responseBody["message"] = "OK";
            responseBody["id"] = id;
            responseBody["access_token"] = auth.getValueOfAccessToken();
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            code = k500InternalServerError;
            responseBody["message"] = "Internal error";
        }
    }
}