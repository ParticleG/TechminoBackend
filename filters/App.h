/**
 *
 *  tech_filters_Version.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;
namespace tech::filters {
    class App : public HttpFilter<App> {
    public:
        App() {}

        virtual void doFilter(
                const HttpRequestPtr &req,
                FilterCallback &&filterCallback,
                FilterChainCallback &&filterChainCallback
        ) override;
    };
}
