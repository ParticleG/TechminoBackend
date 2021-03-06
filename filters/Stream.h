//
// Created by Particle_G on 2021/3/06.
//

#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;
namespace tech::filters {
    class Stream : public HttpFilter<Stream> {
    public:
        Stream() {}

        virtual void doFilter(
                const HttpRequestPtr &req,
                FilterCallback &&filterCallback,
                FilterChainCallback &&filterChainCallback
        ) override;
    };
}
