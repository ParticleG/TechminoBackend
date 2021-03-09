//
// Created by Particle_G on 2021/2/23.
//

#include <filters/App.h>
#include <utils/authorizer.h>
#include <utils/http.h>

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
    string parseError = http::toJson(req, request);

    if (!parseError.empty()) {
        code = k400BadRequest;
        response["message"] = "Wrong format.";
        response["reason"] = parseError;
        http::fromJson(code, response, filterCallback);
        return;
    }

    if (!(
            request.isMember("version") && request["version"].isInt()
    )) {
        code = k400BadRequest;
        response["message"] = "Wrong format";
        response["reason"] = "Requires int type 'version'";
        http::fromJson(code, response, filterCallback);
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
    switch (authorizer::versionCode(
            request["version"].asInt(),
            response
    )) {
        case authorizer::Status::OK:
            attributes->insert("data", response);
            filterChainCallback();
            break;
        case authorizer::Status::InvalidComponents:
            code = k400BadRequest;
            response["message"] = "Wrong format";
            response["reason"] = "Requires positive int type 'version' in 'data'";
            http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case authorizer::Status::NotFound:
            code = k404NotFound;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible auth status.";
            http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case authorizer::Status::Incorrect:
            code = k403Forbidden;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible auth status.";
            http::fromJson(code, response, filterCallback);
            break;
        case authorizer::Status::Expired:
            code = k401Unauthorized;
            response["message"] = "Outdated version";
            http::fromJson(code, response, filterCallback);
            break;
        case authorizer::Status::InternalError:
            code = k500InternalServerError;
            response["message"] = "Internal error";
            http::fromJson(code, response, filterCallback);
            break;
    }
}
