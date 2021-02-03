#pragma once

#include <drogon/HttpController.h>
#include <models/App.h>
#include <models/Auth.h>
#include <models/Data.h>
#include <models/Message.h>
#include <models/Users.h>

using namespace drogon;
using namespace drogon_model;
namespace tech::api::v1 {
    struct JsonResponse {
        HttpStatusCode code;
        Json::Value body;

        void send(std::function<void(const HttpResponsePtr &)> callback) {
            auto httpJsonResponse = HttpResponse::newHttpJsonResponse(body);
            httpJsonResponse->setStatusCode(code);
            callback(httpJsonResponse);
        }
    };





    class Auth : public drogon::HttpController<Auth> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Auth::login, "", Get);
            // METHOD_ADD(Auth::create, "", Post);
            METHOD_ADD(Auth::checkAccessToken, "/access", Get);
            METHOD_ADD(Auth::getAccessToken, "/access", Post);
        METHOD_LIST_END

        // static void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void
        checkAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void
        getAccessToken(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    private:
        static bool
        _refreshToken(JsonResponse &jsonResponse, const std::string &email, const std::string &auth_token);

        static bool
        _updateToken(JsonResponse &jsonResponse, const std::string &email, const std::string &password);
    };

    namespace online {
        class Rooms : public drogon::HttpController<Rooms> {
        public:
            METHOD_LIST_BEGIN
                METHOD_ADD(Rooms::list, "", Get);
                METHOD_ADD(Rooms::info, "/{1}", Get);
                METHOD_ADD(Rooms::create, "/{1}", Post);
            METHOD_LIST_END

            static void list(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

            static void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                             const std::string &roomType);

            static void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                               const std::string &roomType);

        private:
            static bool
            _validate(JsonResponse &jsonResponse, const std::string &email, const std::string &accessToken);
        };
    }
}