//
// Created by Parti on 2021/2/4.
//

#include "httpRequest.h"

string tech::utils::httpRequest::toJson(const HttpRequestPtr &req, Json::Value &result) {
    result = *req->getJsonObject();
    return req->getJsonError();
}
