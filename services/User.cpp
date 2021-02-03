//
// Created by Parti on 2021/2/4.
//

#include <services/User.h>
#include <utils/authorizer.h>
#include <utils/crypto.h>

using namespace tech::services;
using namespace tech::utils;

User::User() {
    userMapper = make_shared<orm::Mapper<Techmino::User>>(app().getDbClient());
}

void User::create(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        code = k501NotImplemented;
        responseBody["message"] = "API not implemented.";
        //TODO: Process the response
    }
}

void User::info(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["authToken"].asString();
        if (authorizer::authToken(email, authToken, code, responseBody)) {
            try {

                auto matchedUsers = userMapper->findBy(
                        Criteria(Techmino::User::Cols::_email, CompareOperator::EQ, email)
                );
                code = k200OK;
                responseBody["message"] = "OK";
                responseBody["username"] = matchedUsers[0].getValueOfUsername();
                responseBody["motto"] = matchedUsers[0].getValueOfMotto();
                responseBody["avatar"] = matchedUsers[0].getValueOfAvatar();
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
            }
        }
    }
}

void User::modify(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                authToken = requestBody["auth_token"].asString(),
                username = requestBody["username"].asString(),
                motto = requestBody["motto"].asString(),
                avatar = requestBody["motto"].asString();
        if (authorizer::authToken(email, authToken, code, responseBody)) {
            try {
                Techmino::User user;

                user.setEmail(email);
                user.setUsername(username);
                user.setMotto(motto);
                user.setAvatar(avatar);

                userMapper->update(user);

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

void User::erase(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        code = k400BadRequest;
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
    } else {
        code = k501NotImplemented;
        responseBody["message"] = "API not implemented.";
        //TODO: Process the response
    }
}
