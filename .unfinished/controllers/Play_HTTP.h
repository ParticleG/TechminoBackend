//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/HttpController.h>
#include ".unfinished/services/Play_HTTP.h"

namespace tech::api::v1::online {
    class Play : public drogon::HttpController<Play> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Play::list, "", drogon::Get);
            METHOD_ADD(Play::info, "/{1}", drogon::Get);
            METHOD_ADD(Play::create, "/{1}", drogon::Post);
        METHOD_LIST_END

        void list(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

        void info(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                  const std::string &roomType);

        void create(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                    const std::string &roomType);

    private:
        tech::services::http::Play _service;
    };
}
