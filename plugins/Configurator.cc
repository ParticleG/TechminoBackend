/**
 *
 *  tech_RoomManager.cc
 *
 */

#include "Configurator.h"

using namespace tech::plugin;
using namespace tech::utils;

void Configurator::initAndStart(const Json::Value &config) {
    if (config.isMember("auth_token_expire_time") && config["access_token_expire_time"].isUInt()) {
        _authTokenExpireTime = config["auth_token_expire_time"].asUInt();
    } else {
        LOG_ERROR << R"(Requires unsigned int value "auth_token_expire_time" in plugin Configurator's config')";
        abort();
    }
    if (config.isMember("access_token_expire_time") && config["access_token_expire_time"].isUInt()) {
        _accessTokenExpireTime = config["access_token_expire_time"].asUInt();
    } else {
        LOG_ERROR << R"(Requires unsigned int value "access_token_expire_time" in plugin Configurator's config')";
        abort();
    }
}

void Configurator::shutdown() {
    /// Shutdown the plugin
}

int Configurator::getAuthExpire() const {
    return _authTokenExpireTime;
}

int Configurator::getAccessExpire() const {
    return _accessTokenExpireTime;
}
