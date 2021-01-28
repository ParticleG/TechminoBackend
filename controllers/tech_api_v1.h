#pragma once

#include <drogon/HttpController.h>

using namespace drogon;
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

    bool authorization(JsonResponse &jsonResponse, const std::string &email, const std::string &authToken);

    class App : public drogon::HttpController<App> {
    public:
        METHOD_LIST_BEGIN
            //use METHOD_ADD to add your custom processing function here;
            //METHOD_ADD(App::get,"/{2}/{1}",Get);//path is /tech/api/v1/App/{arg2}/{arg1}
            //METHOD_ADD(App::your_method_name,"/{1}/{2}/list",Get);//path is /tech/api/v1/App/{arg1}/{arg2}/list
            //ADD_METHOD_TO(App::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
            METHOD_ADD(App::info, "/info", Get);

        METHOD_LIST_END

        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
        // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
        static void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };

    class Users : public drogon::HttpController<Users> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Users::info, "", Get);
//            METHOD_ADD(Users::modify, "", Put);
            METHOD_ADD(Users::getData, "/{1}", Get);
            METHOD_ADD(Users::saveData, "/{1}", Put);
        METHOD_LIST_END

        static void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

//        static void modify(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void getData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                            std::string userId);

        static void saveData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                             std::string userId);

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
            authorization(JsonResponse &jsonResponse, const std::string &email, const std::string &accessToken);
        };
    }
}