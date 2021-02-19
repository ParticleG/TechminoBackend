//
// Created by Parti on 2021/2/3.
//

#include <controllers/App_HTTP.h>

using namespace tech::api::v1;
using namespace tech::utils;
using namespace drogon;
using namespace std;

void App::info(const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;

    _service.info(code, body);

    Http::fromJson(code, body, callback);
}
