//
// Created by Parti on 2021/2/4.
//

#include "authorizer.h"

using namespace drogon_model;

bool tech::utils::authorizer::authToken(
        const std::string &email,
        const std::string &authToken,
        HttpStatusCode &code,
        Json::Value &body
) {
    if (email.empty() || authToken.empty()) {
        code = k400BadRequest;
        body["message"] = "Invalid parameters";
        return false;
    }
    try {
        orm::Mapper<Techmino::Auth> authMapper(app().getDbClient());
        auto matchedUsers = authMapper.findBy(Criteria(Techmino::Auth::Cols::_email, CompareOperator::EQ, email));
        if (matchedUsers.empty()) {
            code = k404NotFound;
            body["message"] = "User not found";
            return false;
        }
        if (authToken != matchedUsers[0].getValueOfAuthToken()) {
            code = k403Forbidden;
            body["message"] = "Auth Token is incorrect";
            return false;
        }
        if (trantor::Date::now() >
            trantor::Date::fromDbStringLocal(matchedUsers[0].getValueOfAuthTokenExpireTime())) {
            code = k401Unauthorized;
            body["message"] = "Auth Token is expired";
            return false;
        }
        return true;
    } catch (const orm::DrogonDbException &e) {
        LOG_ERROR << "error:" << e.base().what();
        code = k500InternalServerError;
        body["message"] = "Internal error";
        return false;
    }
}
