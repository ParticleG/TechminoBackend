//
// Created by Parti on 2021/2/4.
//

#include "httpResponse.h"

void tech::utils::httpResponse::json(
        const HttpStatusCode &code,
        const Json::Value &body,
        const function<void(const HttpResponsePtr &)> &callback
) {
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(body);
    httpJsonResponse->setStatusCode(code);
    callback(httpJsonResponse);
}

void tech::utils::httpResponse::plain(
        const HttpStatusCode &code, const string &body,
        const function<void(const HttpResponsePtr &)> &callback
) { normal(code, CT_TEXT_PLAIN, body, callback); }

void tech::utils::httpResponse::html(
        const HttpStatusCode &code,
        const string &body,
        const function<void(const HttpResponsePtr &)> &callback
) { normal(code, CT_TEXT_HTML, body, callback); }

void tech::utils::httpResponse::normal(
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