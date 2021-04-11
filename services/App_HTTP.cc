//
// Created by Parti on 2021/2/4.
//

#include <plugins/Configurator.h>
#include <services/App_HTTP.h>
#include <utils/crypto.h>
#include <utils/misc.h>
#include <utils/websocket.h>

using namespace drogon_model;
using namespace drogon;
using namespace tech::plugins;
using namespace tech::services::http;
using namespace tech::utils;
using namespace std;

App::App() : _authMapper(app().getDbClient()) {}

Json::Value App::validateAccount(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (request["id"].asString().empty() || request["code"].asString().empty()) {
        response["type"] = "Error";
        response["message"] = "Requires parameter 'id' and 'code'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        uint64_t id = strtoull(request["id"].asString().c_str(), nullptr, 10);
        string cipher = request["code"].asString();
        try {
            auto matchedAuths = _authMapper.findBy(
                    Criteria(Techmino::Auth::Cols::__id, CompareOperator::EQ, id) &&
                    Criteria(Techmino::Auth::Cols::_validated, CompareOperator::EQ, false)
            );
            if (matchedAuths.size() != 1) {
                response["type"] = "Error";
                response["reason"] = "Account not found or already validated";
                code = HttpStatusCode::k404NotFound;
                return response;
            }
            auto tempAuth = matchedAuths[0];
            Json::Value recover;
            const string &encodedKey = tempAuth.getValueOfAuthToken();
            const string &encodedIV = tempAuth.getValueOfAccessToken();
            string parseError = websocket::toJson(
                    crypto::panama::decrypt(cipher, encodedKey, encodedIV),
                    recover);
            if (!parseError.empty()) {
                response["type"] = "Error";
                response["reason"] = "Decrypt failed: " + parseError;
                code = HttpStatusCode::k403Forbidden;
                return response;
            }
            if (misc::toDate(recover["expires"].asString()) >= trantor::Date::now()) {
                // TODO: Validate other values.
                if (recover["email"].asString() == tempAuth.getValueOfEmail()) {
                    auto configurator = app().getPlugin<Configurator>();
                    tempAuth.setValidated(true);
                    tempAuth.setAuthToken(drogon::utils::getUuid());
                    tempAuth.setAuthTokenExpireTime(misc::fromDate(configurator->getAuthExpire()));
                    tempAuth.setAccessToken(crypto::keccak(drogon::utils::getUuid()));
                    tempAuth.setAccessTokenExpireTime(misc::fromDate(configurator->getAccessExpire()));
                    _authMapper.update(tempAuth);

                    response["type"] = "Success";
                    response["reason"] = "Account successfully validated";
                    code = HttpStatusCode::k200OK;
                } else {
                    response["type"] = "Error";
                    response["reason"] = "Data mismatch";
                    code = HttpStatusCode::k406NotAcceptable;
                    return response;
                }
            } else {
                _authMapper.deleteOne(tempAuth);
                response["type"] = "Error";
                response["reason"] = "Validation link expired";
                code = HttpStatusCode::k401Unauthorized;
                return response;
            }
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            response["type"] = "Error";
            response["reason"] = "Internal error";
            return response;
        }
    }
    return response;
}
