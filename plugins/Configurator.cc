/**
 *
 *  tech_RoomManager.cc
 *
 */

#include "Configurator.h"

using namespace tech::plugin;
using namespace tech::utils;

void Configurator::initAndStart(const Json::Value &config) {
    if (config.isMember("auth") && config["auth"].isObject()) {
        _authTokenExpireTime = config["auth"]["auth_token_expire_time"].asUInt();
        _accessTokenExpireTime = config["auth"]["access_token_expire_time"].asUInt();
    } else {
        LOG_ERROR << R"(Requires "auth" in plugin's config')";
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
