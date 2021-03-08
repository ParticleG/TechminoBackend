//
// Created by Particle_G on 2021/2/23.
//

#include "filters/Chat.h"
#include "plugins/Configurator.h"
#include "utils/Authorizer.h"
#include "utils/Http.h"
#include "utils/Utils.h"

using namespace drogon;
using namespace tech::filters;
using namespace tech::plugins;
using namespace tech::utils;
using namespace std;

void Chat::doFilter(
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
            request.isMember("accessToken") && request["accessToken"].isString()
    )) {
        if (!(
                request.isMember("id") && request["id"].isInt64() &&
                request.isMember("authToken") && request["authToken"].isString()
        )) {
            code = k400BadRequest;
            response["message"] = "Wrong format";
            response["reason"] = "Requires positive Int64 type 'id', string type 'accessToken'";
            Http::fromJson(code, response, filterCallback);
            return;
        }
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["id"] = newAuth.getValueOfId();
         * type = Authorizer::Type::GetAccessToken;
         */
        switch (Authorizer::authToken(
                request["id"].asInt(),
                request["authToken"].asString(),
                Utils::fromDate(configurator->getAuthExpire()),
                response)) {
            case Authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", Authorizer::Type::GetAccessToken);
                filterChainCallback();
                break;
            case Authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "RequiredRequired positive Int64 type 'id', string type 'authToken' in 'data'";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::NotFound:
                code = k404NotFound;
                response["message"] = "ID not found";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["message"] = "Access Token is incorrect";
                Http::fromJson(code, response, filterCallback);
                break;
            case Authorizer::Status::Expired:
                code = k401Unauthorized;
                response["message"] = "Access Token is expired";
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
         * result["id"] = newAuth.getValueOfId();
         * type = Authorizer::Type::CheckAccessToken;
         */
        switch (Authorizer::accessToken(
                request["id"].asInt(),
                request["accessToken"].asString(),
                Utils::fromDate(configurator->getAccessExpire()),
                response)) {
            case Authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", Authorizer::Type::CheckAccessToken);
                filterChainCallback();
                break;
            case Authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires positive Int64 type 'id', string type 'accessToken' in 'data'";
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