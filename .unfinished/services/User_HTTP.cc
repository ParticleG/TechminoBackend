//
// Created by Parti on 2021/2/4.
//

#include <.unfinished/services/User_HTTP.h>
#include <utils/Authorizer.h>

using namespace tech::services::http;
using namespace tech::utils;
using namespace drogon_model;
using namespace drogon;
using namespace std;

User::User() {
    userMapper = make_shared<orm::Mapper<Techmino::Info>>(app().getDbClient());
}

void User::create(
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    code = k501NotImplemented;
    responseBody["message"] = "API not implemented.";
    //TODO: Process the response
}

void User::info(
        const std::string &email,
        const std::string &authToken,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::authToken(email, authToken, code, responseBody)) {
        try {
            auto matchedUsers = userMapper->findBy(
                    Criteria(Techmino::Info::Cols::_email, CompareOperator::EQ, email)
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

void User::modify(
        const std::string &email,
        const std::string &authToken,
        const std::string &username,
        const std::string &motto,
        const std::string &avatar,
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    if (Authorizer::authToken(email, authToken, code, responseBody)) {
        try {
            Techmino::Info user;
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

void User::erase(
        drogon::HttpStatusCode &code,
        Json::Value &responseBody
) {
    code = k501NotImplemented;
    responseBody["message"] = "API not implemented.";
    //TODO: Process the response
}
