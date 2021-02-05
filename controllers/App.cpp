//
// Created by Parti on 2021/2/3.
//

#include <controllers/App.h>

using namespace tech;

void api::v1::App::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    HttpStatusCode code;
    Json::Value body;

    _service.info(code, body);

    utils::Http::fromJson(code, body, callback);
}
