//
// Created by Parti on 2021/2/4.
//

#include <plugins/Configurator.h>
#include <services/User_HTTP.h>
#include <utils/authorizer.h>
#include <utils/crypto.h>
#include <utils/misc.h>
#include <utils/websocket.h>

using namespace drogon_model;
using namespace drogon;
using namespace tech::plugins;
using namespace tech::services::http;
using namespace tech::utils;
using namespace std;

User::User() : _authMapper(app().getDbClient()), _infoMapper(app().getDbClient()) {}

Json::Value User::validateAccount(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (!(
            request.isMember("id") && request["id"].isString() &&
            request.isMember("code") && request["code"].isString()
    )) {
        response["type"] = "Error";
        response["reason"] = "Requires parameter 'id' and 'code'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        int64_t uid = strtoll(request["id"].asString().c_str(), nullptr, 10);
        string cipher = request["code"].asString();
        try {
            auto matchedAuths = _authMapper.findBy(
                    Criteria(Techmino::Auth::Cols::__id, CompareOperator::EQ, uid) &&
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
            code = HttpStatusCode::k500InternalServerError;
            return response;
        }
    }
    return response;
}

Json::Value User::loginAccount(
        HttpStatusCode &code,
        const Json::Value &request,
        vector<Cookie> &cookies
) {
    bool remember = false;
    Json::Value response;
    if (!(
            request.isMember("email") && request["email"].isString() &&
            request.isMember("password") && request["password"].isString()
    )) {
        if (request["uid"].asString().empty() || request["authToken"].asString().empty()) {
            code = k400BadRequest;
            response["type"] = "Error";
            response["reason"] = "Wrong format: Requires 'email' and 'password'";
        } else {
            Json::Value result;
            int64_t uid = strtoll(request["uid"].asString().c_str(), nullptr, 10);
            auto newExpireTime = misc::fromDate(app().getPlugin<Configurator>()->getAuthExpire());
            switch (authorizer::authToken(
                    uid,
                    request["authToken"].asString(),
                    newExpireTime,
                    result)) {
                case authorizer::Status::OK:
                    response["type"] = "Success";
                    break;
                case authorizer::Status::InvalidComponents:
                case authorizer::Status::NotFound:
                case authorizer::Status::Incorrect:
                case authorizer::Status::Expired:
                case authorizer::Status::InternalError:
                    code = HttpStatusCode::k403Forbidden;
                    response["type"] = "Error";
                    break;
            }
        }
        return response;
    }
    if (request.isMember("remember") && request["remember"].asBool()) {
        remember = true;
    }
    auto newExpireTime = misc::fromDate(app().getPlugin<Configurator>()->getAuthExpire());
    switch (authorizer::password(
            request["email"].asString(),
            request["password"].asString(),
            newExpireTime,
            response)) {
        case authorizer::Status::OK: {
            auto auth = _authMapper.findOne(
                    Criteria(Techmino::Auth::Cols::__id, CompareOperator::EQ, response["uid"].asInt64())
            );
            response.clear();
            response["type"] = "Success";
            response["data"]["uid"] = auth.getValueOfId();
            response["data"]["authToken"] = auth.getValueOfAuthToken();
            if (remember) {
                Cookie uidCookie("uid", to_string(auth.getValueOfId())),
                        authTokenCookie("authToken", auth.getValueOfAuthToken());
                uidCookie.setExpiresDate(misc::toDate(newExpireTime));
                authTokenCookie.setExpiresDate(misc::toDate(newExpireTime));
                uidCookie.setPath("/");
                authTokenCookie.setPath("/");
                uidCookie.setHttpOnly(false);
                authTokenCookie.setHttpOnly(false);
                cookies.push_back(uidCookie);
                cookies.push_back(authTokenCookie);
            }
        }
            break;
        case authorizer::Status::InvalidComponents:
            code = k400BadRequest;
            response["type"] = "Error";
            response["reason"] = "Wrong format: Requires string type 'email' and 'password' in 'data'";
            break;
            [[unlikely]] case authorizer::Status::NotFound:
            code = k404NotFound;
            response["type"] = "Error";
            response["reason"] = "Impossible auth status.";
            break;
        case authorizer::Status::Incorrect:
            code = k403Forbidden;
            response["type"] = "Error";
            response["reason"] = "Email or Password is incorrect";
            break;
        case authorizer::Status::Expired:
            code = k401Unauthorized;
            response["type"] = "Error";
            response["reason"] = "Account not validated";
            break;
        case authorizer::Status::InternalError:
            code = k500InternalServerError;
            response["type"] = "Error";
            response["reason"] = "Internal error";
            break;
    }
    return response;
}

Json::Value User::profileInfo(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (!(
            request.isMember("uid") && request["uid"].isInt64()
    )) {
        response["type"] = "Error";
        response["reason"] = "Requires 'uid'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        string authToken;
        if (!(
                request.isMember("authToken") &&
                request["authToken"].isString() &&
                !request["authToken"].asString().empty()
        )) {
            authToken = "null";
        } else {
            authToken = request["authToken"].asString();
        }
        try {
            auto info = _infoMapper.findOne(
                    Criteria(Techmino::Info::Cols::__id, CompareOperator::EQ, request["uid"].asInt64())
            );
            response["type"] = "Success";
            response["data"]["uid"] = info.getValueOfId();
            response["data"]["username"] = info.getValueOfUsername();
            response["data"]["motto"] = info.getValueOfMotto();

            Json::Value result;
            auto newExpireTime = misc::fromDate(app().getPlugin<Configurator>()->getAuthExpire());
            switch (authorizer::authToken(
                    request["uid"].asInt64(),
                    authToken,
                    newExpireTime,
                    result)) {
                case authorizer::Status::OK:
                    response["data"]["verified"] = true;
                    break;
                case authorizer::Status::InvalidComponents:
                case authorizer::Status::NotFound:
                case authorizer::Status::Incorrect:
                case authorizer::Status::Expired:
                case authorizer::Status::InternalError:
                    response["data"]["verified"] = false;
                    break;
            }
        } catch (const UnexpectedRows &e) {
            LOG_WARN << e.what();
            response["type"] = "Warn";
            response["reason"] = "ID not found";
            code = HttpStatusCode::k404NotFound;
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            response["type"] = "Error";
            response["reason"] = "Internal error";
            code = HttpStatusCode::k500InternalServerError;
            return response;
        }
    }
    return response;
}

Json::Value User::updateInfo(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (!(
            request.isMember("uid") && request["uid"].isInt64() &&
            request.isMember("authToken") && request["authToken"].isString()
    )) {
        response["type"] = "Error";
        response["reason"] = "Requires 'uid', 'authToken'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        try {
            Json::Value result;
            auto newExpireTime = misc::fromDate(app().getPlugin<Configurator>()->getAuthExpire());
            switch (authorizer::authToken(
                    request["uid"].asInt64(),
                    request["authToken"].asString(),
                    newExpireTime,
                    result)) {
                case authorizer::Status::OK: {
                    auto info = _infoMapper.findOne(
                            Criteria(Techmino::Info::Cols::__id, CompareOperator::EQ, request["uid"].asInt64())
                    );
                    if (request.isMember("username") && request["username"].isString()) {
                        info.setUsername(request["username"].asString());
                    }
                    if (request.isMember("motto") && request["motto"].isString()) {
                        info.setMotto(request["motto"].asString());
                    }
                    _infoMapper.update(info);
                    response["type"] = "Success";
                    response["data"]["username"] = info.getValueOfUsername();
                    response["data"]["motto"] = info.getValueOfMotto();
                }
                    break;
                case authorizer::Status::InvalidComponents:
                    code = k400BadRequest;
                    response["type"] = "Error";
                    response["reason"] = "Wrong format: Requires string type 'email' and 'password' in 'data'";
                    break;
                case authorizer::Status::NotFound:
                    code = k404NotFound;
                    response["type"] = "Error";
                    response["reason"] = "Impossible auth status.";
                    break;
                case authorizer::Status::Incorrect:
                    code = k403Forbidden;
                    response["type"] = "Error";
                    response["reason"] = "Email or Password is incorrect";
                    break;
                case authorizer::Status::Expired:
                    code = k401Unauthorized;
                    response["type"] = "Error";
                    response["reason"] = "Account not validated";
                    break;
                case authorizer::Status::InternalError:
                    code = k500InternalServerError;
                    response["type"] = "Error";
                    response["reason"] = "Internal error";
                    break;
            }
        } catch (const UnexpectedRows &e) {
            LOG_WARN << e.what();
            response["type"] = "Warn";
            response["reason"] = "ID not found";
            code = HttpStatusCode::k404NotFound;
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            response["type"] = "Error";
            response["reason"] = "Internal error";
            code = HttpStatusCode::k500InternalServerError;
            return response;
        }
    }
    return response;
}

Json::Value User::profileAvatar(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (!(
            request.isMember("uid") && request["uid"].isInt64()
    )) {
        response["type"] = "Error";
        response["reason"] = "Requires 'uid'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        std::string hash;
        bool hasHash = false;
        if (request.isMember("hash") && request["hash"].isString()) {
            hasHash = true;
            hash = request["hash"].asString();
        }
        try {
            auto info = _infoMapper.findOne(
                    Criteria(Techmino::Info::Cols::__id, CompareOperator::EQ, request["uid"].asInt64())
            );
            if (hasHash && hash != info.getValueOfAvatarHash()) {
                response["data"]["hash"] = info.getValueOfAvatarHash();
                response["data"]["avatar"] = info.getValueOfAvatar();
            }
            response["type"] = "Success";
        } catch (const UnexpectedRows &e) {
            LOG_WARN << e.what();
            response["type"] = "Warn";
            response["reason"] = "ID not found";
            code = HttpStatusCode::k404NotFound;
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            response["type"] = "Error";
            response["reason"] = "Internal error";
            code = HttpStatusCode::k500InternalServerError;
            return response;
        }
    }
    return response;
}

Json::Value User::updateAvatar(HttpStatusCode &code, const Json::Value &request) {
    Json::Value response;
    if (!(
            request.isMember("uid") && request["uid"].isInt64() &&
            request.isMember("authToken") && request["authToken"].isString() &&
            request.isMember("avatar") && request["avatar"].isString()
    )) {
        response["type"] = "Error";
        response["reason"] = "Requires 'uid', 'authToken', and 'avatar'";
        code = HttpStatusCode::k400BadRequest;
    } else {
        try {
            Json::Value result;
            auto newExpireTime = misc::fromDate(app().getPlugin<Configurator>()->getAuthExpire());
            switch (authorizer::authToken(
                    request["uid"].asInt64(),
                    request["authToken"].asString(),
                    newExpireTime,
                    result)) {
                case authorizer::Status::OK: {
                    auto info = _infoMapper.findOne(
                            Criteria(Techmino::Info::Cols::__id, CompareOperator::EQ, request["uid"].asInt64())
                    );
                    info.setAvatar(request["avatar"].asString());
                    info.setAvatarHash(crypto::blake2b(request["avatar"].asString()));
                    _infoMapper.update(info);
                    response["type"] = "Success";
                    response["data"]["hash"] = info.getValueOfAvatarHash();
                }
                    break;
                case authorizer::Status::InvalidComponents:
                    code = k400BadRequest;
                    response["type"] = "Error";
                    response["reason"] = "Wrong format: Requires string type 'email' and 'password' in 'data'";
                    break;
                case authorizer::Status::NotFound:
                    code = k404NotFound;
                    response["type"] = "Error";
                    response["reason"] = "Impossible auth status.";
                    break;
                case authorizer::Status::Incorrect:
                    code = k403Forbidden;
                    response["type"] = "Error";
                    response["reason"] = "Email or Password is incorrect";
                    break;
                case authorizer::Status::Expired:
                    code = k401Unauthorized;
                    response["type"] = "Error";
                    response["reason"] = "Account not validated";
                    break;
                case authorizer::Status::InternalError:
                    code = k500InternalServerError;
                    response["type"] = "Error";
                    response["reason"] = "Internal error";
                    break;
            }
        } catch (const UnexpectedRows &e) {
            LOG_WARN << e.what();
            response["type"] = "Warn";
            response["reason"] = "ID not found";
            code = HttpStatusCode::k404NotFound;
        } catch (const orm::DrogonDbException &e) {
            LOG_ERROR << "error:" << e.base().what();
            response["type"] = "Error";
            response["reason"] = "Internal error";
            code = HttpStatusCode::k500InternalServerError;
            return response;
        }
    }
    return response;
}
