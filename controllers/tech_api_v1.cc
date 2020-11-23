#include "tech_api_v1.h"

using namespace tech::api::v1;

//add definition of your processing function here
void App::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto clientPtr = drogon::app().getDbClient();
    clientPtr->execSqlAsync("select * from app",
                            [callback](const orm::Result &r) {
                                Json::Value responseBody;
                                responseBody["message"] = "OK";
                                responseBody["version_code"] = r[r.size() - 1]["version_code"].as<int>();
                                responseBody["version_name"] = r[r.size() - 1]["version_name"].as<std::string>();
                                responseBody["notice"] = r[r.size() - 1]["notice"].as<std::string>();
                                auto httpJsonResponse = HttpResponse::newHttpJsonResponse(responseBody);
                                httpJsonResponse->setStatusCode(k200OK);
                                callback(httpJsonResponse);
                            },
                            [callback](const orm::DrogonDbException &e) {
                                LOG_ERROR << "error:" << e.base().what();
                                Json::Value responseBody;
                                responseBody["message"] = "Database corrupted!";
                                auto httpJsonResponse = HttpResponse::newHttpJsonResponse(responseBody);
                                httpJsonResponse->setStatusCode(k500InternalServerError);
                                callback(httpJsonResponse);
                            });
}

void Users::registration(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto clientPtr = drogon::app().getDbClient();
    Json::Value responseBody;
    HttpStatusCode responseCode;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        responseBody["message"] = "Wrong format";
        responseCode = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                username = (*requestBody)["username"].asString(),
                password = (*requestBody)["password"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where email=$1", email);
            if (!result.empty()) {
                responseCode = k403Forbidden;
                responseBody["message"] = "Email already used";
            } else {
                std::string passwordDigest = sha256(password);
                std::cout << passwordDigest << std::endl;

                result = clientPtr->execSqlSync("insert into users (email, username, password) values ($1, $2, $3)",
                                                email,
                                                username,
                                                passwordDigest);
                responseCode = k200OK;
                responseBody["message"] = "OK";
            }
        } catch (const orm::DrogonDbException &e) {
            responseCode = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            responseBody["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(responseBody);
    httpJsonResponse->setStatusCode(responseCode);
    callback(httpJsonResponse);
}

void Users::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto clientPtr = drogon::app().getDbClient();
    Json::Value responseBody;
    HttpStatusCode responseCode;
    auto requestBody = req->getJsonObject();
    if (!req->getJsonError().empty()) {
        responseBody["message"] = "Wrong format.";
        responseCode = k400BadRequest;
    } else {
        std::string email = (*requestBody)["email"].asString(),
                password = (*requestBody)["password"].asString();
        try {
            auto result = clientPtr->execSqlSync("select * from users where email=$1", email);
            if (result.empty()) {
                responseCode = k403Forbidden;
                responseBody["message"] = "Email not registered!";
            } else if (sha256(password) != result[0]["password"].as<std::string>()) {
                responseCode = k401Unauthorized;
                responseBody["message"] = "Wrong password!";
            } else {
                std::string token = drogon::utils::getUuid();
                std::string username = result[0]["username"].as<std::string>();
                responseCode = k200OK;
                responseBody["message"] = "OK";
                responseBody["token"] = token;
                responseBody["user_id"] = result[0]["id"].as<int>();
                result = clientPtr->execSqlSync("update users set token = $1 where email = $2",
                                                token,
                                                email);
            }
        } catch (const orm::DrogonDbException &e) {
            responseCode = k500InternalServerError;
            LOG_ERROR << "error:" << e.base().what();
            responseBody["message"] = "Database corrupted!";
        }
    }
    auto httpJsonResponse = HttpResponse::newHttpJsonResponse(responseBody);
    httpJsonResponse->setStatusCode(responseCode);
    callback(httpJsonResponse);
}
