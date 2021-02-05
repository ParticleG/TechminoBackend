//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include <services/Play_HTTP.h>

namespace tech::api::v1::online {
    class Play : public drogon::HttpController<Play> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Play::list, "", Get);
            METHOD_ADD(Play::info, "/{1}", Get);
            METHOD_ADD(Play::create, "/{1}", Post);
        METHOD_LIST_END

        void list(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                  const std::string &roomType);

        void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                    const std::string &roomType);

    private:
        tech::services::Play _service;
    };
}
