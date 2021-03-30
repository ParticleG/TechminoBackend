//
// Created by Particle_G on 2021/3/03.
//

#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;
namespace tech::filters {
    class Play : public HttpFilter<Play> {
    public:
        Play() = default;

        void doFilter(
                const HttpRequestPtr &req,
                FilterCallback &&filterCallback,
                FilterChainCallback &&filterChainCallback
        ) override;
    };
}
