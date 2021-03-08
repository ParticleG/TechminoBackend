//
// Created by Particle_G on 2021/2/23.
//

#include "filters/App.h"
#include "utils/Authorizer.h"
#include "utils/Http.h"

using namespace drogon;
using namespace tech::filters;
using namespace tech::utils;
using namespace std;

void App::doFilter(
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
            request.isMember("version") && request["version"].isInt()
    )) {
        code = k400BadRequest;
        response["message"] = "Wrong format";
        response["reason"] = "Requires int type 'version'";
        Http::fromJson(code, response, filterCallback);
        return;
    }
    auto attributes = req->getAttributes();
    /**
     * result["versionCode"] = versionCode
     * result["content"]["newest"]["code"] = newestApp.getValueOfVersionCode();
     * result["content"]["newest"]["name"] = newestApp.getValueOfVersionName();
     * result["content"]["newest"]["content"] = newestApp.getValueOfVersionContent();
     * result["content"]["notice"] = notice.getValueOfContent();
     */
    switch (Authorizer::versionCode(
            request["version"].asInt(),
            response
    )) {
        case Authorizer::Status::OK:
            attributes->insert("data", response);
            filterChainCallback();
            break;
        case Authorizer::Status::InvalidComponents:
            code = k400BadRequest;
            response["message"] = "Wrong format";
            response["reason"] = "Requires positive int type 'version' in 'data'";
            Http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case Authorizer::Status::NotFound:
            code = k404NotFound;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible auth status.";
            Http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case Authorizer::Status::Incorrect:
            code = k403Forbidden;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible auth status.";
            Http::fromJson(code, response, filterCallback);
            break;
        case Authorizer::Status::Expired:
            code = k401Unauthorized;
            response["message"] = "Outdated version";
            Http::fromJson(code, response, filterCallback);
            break;
        case Authorizer::Status::InternalError:
            code = k500InternalServerError;
            response["message"] = "Internal error";
            Http::fromJson(code, response, filterCallback);
            break;
    }
}
