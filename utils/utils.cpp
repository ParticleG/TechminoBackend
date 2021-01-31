//
// Created by Parti on 2021/1/31.
//

#include "utils.h"

std::string tech::utils::utils::_jsonToPlainText(const Json::Value &value) {
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    std::ostringstream oss;
    jsonWriter->write(value, &oss);
    return oss.str();
}
