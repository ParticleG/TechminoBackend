//
// Created by Parti on 2021/2/4.
//

#include <services/App.h>

using namespace tech::services;
using namespace drogon_model;
using namespace drogon;
using namespace std;

void App::info(HttpStatusCode &code, Json::Value &body) {
    try {
        auto matchedApps = appMapper->orderBy(Techmino::App::Cols::_version_code, SortOrder::DESC).limit(1).findAll();
        auto matchedContent = messageMapper->orderBy(Techmino::Message::Cols::_id, SortOrder::DESC)
                .findBy(Criteria(Techmino::Message::Cols::_type, CompareOperator::EQ, "notice"));
        code = k200OK;
        body["message"] = "OK";
        body["version_code"] = matchedApps[0].getValueOfVersionCode();
        body["version_name"] = matchedApps[0].getValueOfVersionName();
        body["version_content"] = matchedApps[0].getValueOfVersionContent();
        body["notice"] = matchedContent[0].getValueOfContent();
    } catch (const orm::DrogonDbException &e) {
        code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        body["message"] = "Internal error";
    }
}

App::App() {
    appMapper = make_shared<orm::Mapper<Techmino::App>>(app().getDbClient());
    messageMapper = make_shared<orm::Mapper<Techmino::Message>>(app().getDbClient());
}
