//
// Created by Particle_G on 2021/2/23.
//

#include <filters/User.h>
#include <plugins/Configurator.h>
#include <utils/authorizer.h>
#include <utils/http.h>
#include <utils/misc.h>

using namespace drogon;
using namespace tech::filters;
using namespace tech::plugins;
using namespace tech::utils;
using namespace std;

void User::doFilter(
        const HttpRequestPtr &req,
        FilterCallback &&filterCallback,
        FilterChainCallback &&filterChainCallback
) {
    HttpStatusCode code;
    Json::Value request, response;
    string parseError = http::toJson(req, request);

    if (!parseError.empty()) {
        code = k400BadRequest;
        response["message"] = "Wrong format.";
        response["reason"] = parseError;
        http::fromJson(code, response, filterCallback);
        return;
    }

    if (!(
            request.isMember("id") && request["id"].isInt64() &&
            request.isMember("authToken") && request["authToken"].isString()
    )) {
        if (!(
                request.isMember("email") && request["email"].isString() &&
                request.isMember("password") && request["password"].isString()
        )) {
            code = k400BadRequest;
            response["message"] = "Wrong format";
            response["reason"] = "Requires string type 'email', string type 'authToken' or int type 'id', string type 'authToken'";
            http::fromJson(code, response, filterCallback);
            return;
        }
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["id"] = newAuth.getValueOfId();
         * type = authorizer::Type::GetAuthToken;
         */
        switch (authorizer::password(
                request["email"].asString(),
                request["password"].asString(),
                misc::fromDate(configurator->getAuthExpire()),
                response)) {
            case authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", authorizer::Type::GetAuthToken);
                filterChainCallback();
                break;
            case authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires string type 'email' and 'password' in 'data'";
                http::fromJson(code, response, filterCallback);
                break;
                [[unlikely]] case authorizer::Status::NotFound:
                code = k404NotFound;
                response["message"] = "Something went wrong";
                response["reason"] = "Impossible auth status.";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["message"] = "Email or Password is incorrect";
                http::fromJson(code, response, filterCallback);
                break;
                [[unlikely]] case authorizer::Status::Expired:
                code = k401Unauthorized;
                response["message"] = "Something went wrong";
                response["reason"] = "Impossible auth status.";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::InternalError:
                code = k500InternalServerError;
                response["message"] = "Internal error";
                http::fromJson(code, response, filterCallback);
                break;
        }
    } else {
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["id"] = newAuth.getValueOfId();
         * type = authorizer::Type::CheckAuthToken;
         */
        switch (authorizer::authToken(
                request["id"].asInt(),
                request["authToken"].asString(),
                misc::fromDate(configurator->getAuthExpire()),
                response)) {
            case authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", authorizer::Type::CheckAuthToken);
                filterChainCallback();
                break;
            case authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires positive Int64 type 'id', string type 'authToken' in 'data'";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::NotFound:
                code = k404NotFound;
                response["message"] = "ID not found";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["message"] = "Auth Token is incorrect";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::Expired:
                code = k401Unauthorized;
                response["message"] = "Auth Token is expired";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::InternalError:
                code = k500InternalServerError;
                response["message"] = "Internal error";
                http::fromJson(code, response, filterCallback);
                break;
        }
    }
}