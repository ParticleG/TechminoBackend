//
// Created by Parti on 2021/2/4.
//

#include <services/App.h>

void tech::services::App::info(HttpStatusCode &code, Json::Value &body) {

    try {
        auto matchedApps = appMapper->findAll();
        auto matchedContent = messageMapper->orderBy(Techmino::Message::Cols::_id, SortOrder::DESC)
                .findBy(Criteria(Techmino::Message::Cols::_type, CompareOperator::EQ, "notice"));

        code = k200OK;
        body["message"] = "OK";
        body["version_code"] = matchedApps[matchedApps.size() - 1].getValueOfVersionCode();
        body["version_name"] = matchedApps[matchedApps.size() - 1].getValueOfVersionName();
        body["version_content"] = matchedApps[matchedApps.size() - 1].getValueOfVersionContent();
        body["notice"] = matchedContent[0].getValueOfContent();
    } catch (const orm::DrogonDbException &e) {
        code = k500InternalServerError;
        LOG_ERROR << "error:" << e.base().what();
        body["message"] = "Internal error";
    }
}

tech::services::App::App() {
    appMapper = make_shared<orm::Mapper<Techmino::App>>(app().getDbClient());
    messageMapper = make_shared<orm::Mapper<Techmino::Message>>(app().getDbClient());
}
