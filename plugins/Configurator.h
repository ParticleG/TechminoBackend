//
// Created by Parti on 2021/2/4.
//

#pragma once

#include <drogon/plugins/Plugin.h>
#include <services/RoomManager.h>
#include <services/Room.h>
#include <utils/Utils.h>

using namespace drogon;
using namespace trantor;

namespace tech::plugin {
    class Configurator : public drogon::Plugin<Configurator> {
    public:
        Configurator() {}

        virtual void initAndStart(const Json::Value &config) override;

        virtual void shutdown() override;

        int getAuthExpire() const;

        int getAccessExpire() const;

    private:
        uint64_t _authTokenExpireTime, _accessTokenExpireTime;
    };
}

