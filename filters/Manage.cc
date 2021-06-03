//
// Created by Particle_G on 2021/2/23.
//

#include <filters/Manage.h>
#include <plugins/Configurator.h>
#include <utils/authorizer.h>
#include <utils/http.h>
#include <utils/misc.h>

using namespace drogon;
using namespace tech::filters;
using namespace tech::plugins;
using namespace tech::utils;
using namespace std;

void Manage::doFilter(
        const HttpRequestPtr &req,
        FilterCallback &&filterCallback,
        FilterChainCallback &&filterChainCallback
) {
    if (req->peerAddr().isIntranetIp()) {
        filterChainCallback();
        return;
    }

    HttpStatusCode code;
    Json::Value request, response;
    string parseError = http::toJson(req, request);

    if (!parseError.empty()) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: " + parseError;
        http::fromJson(code, response, filterCallback);
        return;
    }

    if (!(
            request.isMember("uid") && request["uid"].isInt64() &&
            request.isMember("authToken") && request["authToken"].isString()
    )) {
        code = k400BadRequest;
        response["type"] = "Error";
        response["reason"] = "Wrong format: Requires positive Int64 type 'uid', string type 'authToken'";
        http::fromJson(code, response, filterCallback);
        return;
    } else {
        auto attributes = req->getAttributes();
        auto configurator = app().getPlugin<Configurator>();
        /**
         * data["uid"] = newAuth.getValueOfId();
         * type = authorizer::Type::CheckAuthToken;
         */
        switch (authorizer::authToken(
                request["uid"].asInt64(),
                request["authToken"].asString(),
                misc::fromDate(configurator->getAuthExpire()),
                response)) {
            case authorizer::Status::OK:
                if (configurator->isSuperusers(request["uid"].asInt64())) {
                    filterChainCallback();
                } else {
                    code = k403Forbidden;
                    response["type"] = "Error";
                    response["reason"] = "Permission denied";
                    http::fromJson(code, response, filterCallback);
                }
                break;
            case authorizer::Status::InvalidComponents:
                code = k400BadRequest;
                response["type"] = "Error";
                response["reason"] = "Wrong format: Requires positive Int64 type 'uid', string type 'authToken' in 'data'";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::NotFound:
                code = k404NotFound;
                response["type"] = "Error";
                response["reason"] = "ID not found";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::Incorrect:
                code = k403Forbidden;
                response["type"] = "Error";
                response["reason"] = "Auth Token is incorrect";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::Expired:
                code = k401Unauthorized;
                response["type"] = "Error";
                response["reason"] = "Auth Token is expired";
                http::fromJson(code, response, filterCallback);
                break;
            case authorizer::Status::InternalError:
                code = k500InternalServerError;
                response["type"] = "Error";
                response["reason"] = "Internal error";
                http::fromJson(code, response, filterCallback);
                break;
        }
    }
}