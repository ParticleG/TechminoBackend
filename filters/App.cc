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
    Json::Value response;
    auto attributes = req->getAttributes();
    /**
     * result["content"]["newestCode"] = newestApp.getValueOfVersionCode();
     * result["content"]["newestName"] = newestApp.getValueOfVersionName();
     * result["content"]["lowest"] = leastApp.getValueOfVersionCode();
     * result["content"]["notice"] = notice.getValueOfContent();
     */
    switch (authorizer::versionCode(
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
            response["reason"] = "Impossible app status.";
            http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case authorizer::Status::Incorrect:
            code = k403Forbidden;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible app status.";
            http::fromJson(code, response, filterCallback);
            break;
            [[unlikely]] case authorizer::Status::Expired:
            code = k401Unauthorized;
            response["message"] = "Something went wrong";
            response["reason"] = "Impossible app status.";
            http::fromJson(code, response, filterCallback);
            break;
        case authorizer::Status::InternalError:
            code = k500InternalServerError;
            response["message"] = "Internal error";
            http::fromJson(code, response, filterCallback);
            break;
    }
}
