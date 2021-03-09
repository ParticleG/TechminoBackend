//
// Created by Particle_G on 2021/2/23.
//

#include "filters/User.h"
#include "plugins/Configurator.h"
#include "utils/Authorizer.h"
#include "utils/Http.h"
#include "utils/Utils.h"

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
    string parseError = Http::toJson(req, request);

    if (!parseError.empty()) {
        code = k400BadRequest;
        response["message"] = "Wrong format.";
        response["reason"] = parseError;
        Http::fromJson(code, response, filterCallback);
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
            Http::fromJson(code, response, filterCallback);
            return;
        }
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["id"] = newAuth.getValueOfId();
         * type = Authorizer::Type::GetAuthToken;
         */
        switch (Authorizer::password(
                request["email"].asString(),
                request["password"].asString(),
                Utils::fromDate(configurator->getAuthExpire()),
                response)) {
            case Authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", Authorizer::Type::GetAuthToken);
                filterChainCallback();
                break;
            case Authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires string type 'email' and 'password' in 'data'";
                Http::fromJson(code, response, filterCallback);
                break;
                [[unlikely]] case Authorizer::Status::NotFound:
                code = k404NotFound;
                response["message"] = "Something went wrong";
                response["reason"] = "Impossible auth status.";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["message"] = "Email or Password is incorrect";
                Http::fromJson(code, response, filterCallback);
                break;
                [[unlikely]] case Authorizer::Status::Expired:
                code = k401Unauthorized;
                response["message"] = "Something went wrong";
                response["reason"] = "Impossible auth status.";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::InternalError:
                code = k500InternalServerError;
                response["message"] = "Internal error";
                Http::fromJson(code, response, filterCallback);
                break;
        }
    } else {
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["id"] = newAuth.getValueOfId();
         * type = Authorizer::Type::CheckAuthToken;
         */
        switch (Authorizer::authToken(
                request["id"].asInt(),
                request["authToken"].asString(),
                Utils::fromDate(configurator->getAuthExpire()),
                response)) {
            case Authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", Authorizer::Type::CheckAuthToken);
                filterChainCallback();
                break;
            case Authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires positive Int64 type 'id', string type 'authToken' in 'data'";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::NotFound:
                code = k404NotFound;
                response["message"] = "ID not found";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["message"] = "Auth Token is incorrect";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::Expired:
                code = k401Unauthorized;
                response["message"] = "Auth Token is expired";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::InternalError:
                code = k500InternalServerError;
                response["message"] = "Internal error";
                Http::fromJson(code, response, filterCallback);
                break;
        }
    }
}