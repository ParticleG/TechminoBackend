#pragma once

#include <drogon/HttpController.h>
#include <openssl/sha.h>

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
            //use METHOD_ADD to add your custom processing function here;
            //METHOD_ADD(Users::get,"/{2}/{1}",Get);//path is /tech/api/v1/Users/{arg2}/{arg1}
            //METHOD_ADD(Users::your_method_name,"/{1}/{2}/list",Get);//path is /tech/api/v1/Users/{arg1}/{arg2}/list
            //ADD_METHOD_TO(Users::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
            METHOD_ADD(Users::create, "", Post);
            METHOD_ADD(Users::login, "", Get);
            METHOD_ADD(Users::modify, "", Put);

            METHOD_ADD(Users::getData, "/{1}", Get);
            METHOD_ADD(Users::saveData, "/{1}", Put);

        METHOD_LIST_END

        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
        // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
        static void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void modify(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void getData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                            std::string userId);

        static void saveData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                             std::string userId);

    private:
        static std::string _sha256(const std::string &srcStr);

        static void
        _refreshToken(JsonResponse &jsonResponse, int id, const std::string &auth_token);

        static void
        _updateToken(JsonResponse &jsonResponse, std::string email, const std::string &password);

    };
}
