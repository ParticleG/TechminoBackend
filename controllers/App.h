//
// Created by Parti on 2021/2/3.
//

#pragma once

#include <drogon/HttpController.h>
#include <services/App.h>

using namespace drogon;

namespace tech::api::v1 {
    class App : public drogon::HttpController<App> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(App::info, "/info", Get);
        METHOD_LIST_END

        void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    private:
        services::App _service;
    };
}
