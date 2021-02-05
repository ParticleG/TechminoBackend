//
// Created by Parti on 2021/2/4.
//

#include "Http.h"

using namespace tech::utils;

void Http::fromJson(
        const HttpStatusCode &code,
        const Json::Value &body,
        const function<void(const HttpResponsePtr &)> &callback
) {
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(body);
    httpJsonResponse->setStatusCode(code);
    callback(httpJsonResponse);
}

void Http::fromPlain(
        const HttpStatusCode &code, const string &body,
        const function<void(const HttpResponsePtr &)> &callback
) { from(code, CT_TEXT_PLAIN, body, callback); }

void Http::fromHtml(
        const HttpStatusCode &code,
        const string &body,
        const function<void(const HttpResponsePtr &)> &callback
) { from(code, CT_TEXT_HTML, body, callback); }

void Http::from(
        const HttpStatusCode &code,
        const ContentType &type,
        const string &body,
        const function<void(const HttpResponsePtr &)> &callback
) {
    auto httpResponse = HttpResponse::newHttpResponse();
    httpResponse->setStatusCode(code);
    httpResponse->setContentTypeCode(type);
    httpResponse->setBody(body);
    callback(httpResponse);
}

string Http::toJson(
        const HttpRequestPtr &req,
        Json::Value &result
) {
    result = *req->getJsonObject();
    return req->getJsonError();
}