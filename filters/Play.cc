//
// Created by Particle_G on 2021/3/03.
//

#include "filters/Play.h"
#include "plugins/Configurator.h"
#include "utils/Authorizer.h"
#include "utils/Http.h"
#include "utils/Utils.h"

using namespace drogon;
using namespace tech::filters;
using namespace tech::plugins;
using namespace tech::utils;
using namespace std;

void Play::doFilter(
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
        code = k400BadRequest;
        response["message"] = "Wrong format";
        response["reason"] = "Requires positive Int64 type 'id', string type 'accessToken'";
        Http::fromJson(code, response, filterCallback);
        return;
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