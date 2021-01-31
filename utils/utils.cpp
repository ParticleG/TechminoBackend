//
// Created by Parti on 2021/1/31.
//

#include "utils.h"

std::string tech::utils::utils::jsonToPlainText(const Json::Value &value) {
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    std::ostringstream oss;
    jsonWriter->write(value, &oss);
    return oss.str();
}

unsigned long tech::utils::utils::uniform_random() {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0, 99999999);
    return dis(generator);
}
