//
// Created by Parti on 2021/2/16.
//

#include <services/App.h>

using namespace tech::services;
using namespace drogon_model;
using namespace drogon;
using namespace std;

App::App() {
    appMapper = make_shared<orm::Mapper<Techmino::App>>(app().getDbClient());
    messageMapper = make_shared<orm::Mapper<Techmino::Message>>(app().getDbClient());
}

bool App::establish(
        const WebSocketConnectionPtr &wsConnPtr,
        const int &versionCode,
        CloseCode &code,
        Json::Value &response
) {
    try {
        auto newestApp = appMapper->orderBy(Techmino::App::Cols::_version_code, SortOrder::DESC).limit(1).findAll()[0];
        auto leastApp = appMapper->orderBy(Techmino::App::Cols::_version_code, SortOrder::DESC).limit(1)
                .findBy(Criteria(Techmino::App::Cols::_compatible, CompareOperator::EQ, false))[0];
        auto matchedContent = messageMapper->orderBy(Techmino::Message::Cols::_id, SortOrder::DESC)
                .findBy(Criteria(Techmino::Message::Cols::_type, CompareOperator::EQ, "notice"));

        response["newest"] = newestApp.getValueOfVersionCode();
        response["least"] = newestApp.getValueOfVersionCode();
        response["name"] = newestApp.getValueOfVersionName();
        response["content"] = newestApp.getValueOfVersionContent();
        response["notice"] = matchedContent[0].getValueOfContent();

        if (versionCode < leastApp.getValueOfVersionCode()) {
            code = CloseCode::kViolation;
            response["message"] = "Outdated version";
            return false;
        } else {
            response["message"] = "OK";
            wsConnPtr->setContext(make_shared<_App>(versionCode));
            return true;
        }
    } catch (const orm::DrogonDbException &e) {
        code = CloseCode::kUnexpectedCondition;
        LOG_ERROR << "error:" << e.base().what();
        response["message"] = "Internal error";
        return false;
    }
}

void App::messageHandler(
        const WebSocketConnectionPtr &wsConnPtr,
        const Json::Value &response
) {

}

App::_App::_App(const int &versionCode) : _versionCode(versionCode) {}
