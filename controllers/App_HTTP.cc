//
// Created by Parti on 2021/2/3.
//

#include <controllers/App_HTTP.h>

using namespace tech::api::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void App::validateAccount(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code = HttpStatusCode::k200OK;
    Json::Value request;
    request["id"] = req->getParameter("id");
    request["code"] = req->getParameter("code");
    http::fromJson(code, _service.validateAccount(code, request), callback);
}
