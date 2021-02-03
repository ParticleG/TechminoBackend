//
// Created by Parti on 2021/2/4.
//

#include <services/User.h>
#include <utils/httpRequest.h>
#include <utils/crypto.h>

using namespace tech::services;
using namespace tech::utils;

void User::create(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        //TODO: Process the response
    }
}

void User::info(const HttpRequestPtr &req, HttpStatusCode &code, Json::Value &responseBody) {
    orm::Mapper<Techmino::User> usersMapper(app().getDbClient());
    orm::Mapper<Techmino::Auth> authMapper(app().getDbClient());
    Json::Value requestBody;
    string parseError = httpRequest::toJson(req, requestBody);
    if (!parseError.empty()) {
        responseBody["message"] = "Wrong format.";
        responseBody["reason"] = parseError;
        code = k400BadRequest;
    } else {
        std::string email = requestBody["email"].asString(),
                auth_token = requestBody["auth_token"].asString();
        if (_validate(jsonResponse, authMapper, email, auth_token)) {
            try {
                auto matchedUsers = usersMapper.findBy(
                        Criteria(Techmino::User::Cols::_email, CompareOperator::EQ, email));
                code = k200OK;
                responseBody["message"] = "OK";
                responseBody["username"] = matchedUsers[0].getValueOfUsername();
                responseBody["motto"] = matchedUsers[0].getValueOfMotto();
                responseBody["avatar"] = matchedUsers[0].getValueOfAvatar();
            } catch (const orm::DrogonDbException &e) {
                LOG_ERROR << "error:" << e.base().what();
                code = k500InternalServerError;
                responseBody["message"] = "Internal error";
                return;
            }
        }
    }
    jsonResponse.respond(callback);
}

//void User::modify(HttpStatusCode &code, Json::Value &body) {
//    auto clientPtr = drogon::app().getDbClient();
//    JsonResponse jsonResponse;
//    auto requestBody = req->getJsonObject();
//    if (!req->getJsonError().empty()) {
//        jsonResponse.body["message"] = "Wrong format.";
//        jsonResponse.code = k400BadRequest;
//    } else {
//        std::string
//                id = (*requestBody)["id"].asString(),
//                auth_token = (*requestBody)["auth_token"].asString(),
//                password = (*requestBody)["password"].asString(),
//                newPassword = (*requestBody)["new_password"].asString();
//        try {
//            auto result = clientPtr->execSqlSync("select * from users where id = $1", id);
//            if (result.empty()) {
//                jsonResponse.code = k404NotFound;
//                jsonResponse.body["message"] = "User not found";
//            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
//                jsonResponse.code = k401Unauthorized;
//                jsonResponse.body["message"] = "Token expired";
//            } else if (tech::utils::Crypto::sha256(password) != result[0]["password"].as<std::string>()) {
//                jsonResponse.code = k403Forbidden;
//                jsonResponse.body["message"] = "Wrong password";
//            } else {
//                std::string new_token = drogon::utils::getUuid();
//                jsonResponse.code = k200OK;
//                jsonResponse.body["message"] = "OK";
//                jsonResponse.body["auth_token"] = new_token;
//                clientPtr->execSqlSync("modify users set password = $1, auth_token = $2 where id = $3",
//                                       tech::utils::Crypto::sha256(newPassword),
//                                       new_token,
//                                       id);
//            }
//        } catch (const orm::DrogonDbException &e) {
//            jsonResponse.code = k500InternalServerError;
//            LOG_ERROR << "error:" << e.base().what();
//            jsonResponse.body["message"] = "Internal error";
//        }
//    }
//    jsonResponse.respond(callback);
//}

void
User::getData(HttpStatusCode &code, Json::Value &body) {
    auto clientPtr = drogon::app().getDbClient();
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string auth_token = (*requestBody)["auth_token"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where id = $1", userId);
            if (result.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Token expired";
            } else {
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                //TODO: return data to the client
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.respond(callback);
}

void User::saveData(HttpStatusCode &code, Json::Value &body) {
    auto clientPtr = drogon::app().getDbClient();
    JsonResponse jsonResponse;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        jsonResponse.body["message"] = "Wrong format.";
        jsonResponse.code = k400BadRequest;
    } else {
        std::string auth_token = (*requestBody)["auth_token"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where id = $1", userId);
            if (result.empty()) {
                jsonResponse.code = k404NotFound;
                jsonResponse.body["message"] = "User not found";
            } else if (auth_token != result[0]["auth_token"].as<std::string>()) {
                jsonResponse.code = k401Unauthorized;
                jsonResponse.body["message"] = "Token expired";
            } else {
                jsonResponse.code = k200OK;
                jsonResponse.body["message"] = "OK";
                //TODO: save data from the client
            }
        } catch (const orm::DrogonDbException &e) {
            jsonResponse.code = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            jsonResponse.body["message"] = "Internal error";
        }
    }
    jsonResponse.respond(callback);
}

bool User::_validate(
        HttpStatusCode &code,
        Json::Value &body,
        orm::Mapper<Techmino::Auth> &authMapper,
        const std::string &email,
        const std::string &authToken
) {

}
