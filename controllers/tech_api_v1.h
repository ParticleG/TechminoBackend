#pragma once

#include <drogon/HttpController.h>
#include <openssl/sha.h>

using namespace drogon;
namespace tech::api::v1 {
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
            METHOD_ADD(Users::registration, "", Post);
            METHOD_ADD(Users::login, "", Get);

        METHOD_LIST_END

        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
        // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
        static void registration(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    private:
        static std::string sha256(const std::string &srcStr) {
            unsigned char mdStr[33] = {0};
            SHA256((const unsigned char *) srcStr.c_str(), srcStr.length(), mdStr);
            char buf[65] = {0};
            char tmp[3] = {0};
            for (int i = 0; i < 32; i++) {
                sprintf_s(tmp, "%02x", mdStr[i]);
                strcat_s(buf, tmp);
            }
            buf[32] = '\0';
            return std::string(buf);
        }
    };
}
