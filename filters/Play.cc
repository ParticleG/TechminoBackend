//
// Created by Particle_G on 2021/3/03.
//

#include <filters/Play.h>
#include <plugins/Configurator.h>
#include <utils/authorizer.h>
#include <utils/http.h>
#include <utils/misc.h>

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
            request.isMember("accessToken") && request["accessToken"].isString()
    )) {
        code = k400BadRequest;
        response["message"] = "Wrong format";
        response["reason"] = "Requires positive Int64 type 'id', string type 'accessToken'";
        http::fromJson(code, response, filterCallback);
        return;
    } else {
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * result["id"] = newAuth.getValueOfId();
         * type = authorizer::Type::CheckAccessToken;
         */
        switch (authorizer::accessToken(
                request["id"].asInt(),
                request["accessToken"].asString(),
                misc::fromDate(configurator->getAccessExpire()),
                response)) {
            case authorizer::Status::OK:
                attributes->insert("data", response);
                attributes->insert("type", authorizer::Type::CheckAccessToken);
                filterChainCallback();
                break;
            case authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["message"] = "Wrong format";
                response["reason"] = "Requires positive Int64 type 'id', string type 'accessToken' in 'data'";
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