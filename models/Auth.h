/**
 *
 *  Auth.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once

#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

using namespace drogon::orm;
namespace drogon {
    namespace orm {
        class DbClient;

        using DbClientPtr = std::shared_ptr<DbClient>;
    }
}
namespace drogon_model {
    namespace Techmino {

        class Auth {
        public:
            struct Cols {
                static const std::string __id;
                static const std::string _email;
                static const std::string _password;
                static const std::string _auth_token;
                static const std::string _access_token;
                static const std::string _auth_token_expire_time;
                static const std::string _access_token_expire_time;
                static const std::string _qq;
            };

            const static int primaryKeyNumber;
            const static std::string tableName;
            const static bool hasPrimaryKey;
            const static std::string primaryKeyName;
            using PrimaryKeyType = int64_t;

            const PrimaryKeyType &getPrimaryKey() const;

            /**
             * @brief constructor
             * @param r One row of records in the SQL query result.
             * @param indexOffset Set the offset to -1 to access all columns by column names,
             * otherwise access all columns by offsets.
             * @note If the SQL is not a style of 'select * from table_name ...' (select all
             * columns by an asterisk), please set the offset to -1.
             */
            explicit Auth(const Row &r, const ssize_t indexOffset = 0) noexcept;

            /**
             * @brief constructor
             * @param pJson The fromJson object to construct a new instance.
             */
            explicit Auth(const Json::Value &pJson) noexcept(false);

            /**
             * @brief constructor
             * @param pJson The fromJson object to construct a new instance.
             * @param pMasqueradingVector The aliases of table columns.
             */
            Auth(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

            Auth() = default;

            void updateByJson(const Json::Value &pJson) noexcept(false);

            void updateByMasqueradedJson(const Json::Value &pJson,
                                         const std::vector<std::string> &pMasqueradingVector) noexcept(false);

            static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);

            static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                           const std::vector<std::string> &pMasqueradingVector,
                                                           std::string &err);

            static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);

            static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                                         const std::vector<std::string> &pMasqueradingVector,
                                                         std::string &err);

            static bool validJsonOfField(size_t index,
                                         const std::string &fieldName,
                                         const Json::Value &pJson,
                                         std::string &err,
                                         bool isForCreation);

            /**  For column _roomID  */
            ///Get the value of the column _roomID, returns the default value if the column is null
            const int64_t &getValueOfId() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<int64_t> &getId() const noexcept;

            ///Set the value of the column _roomID
            void setId(const int64_t &pId) noexcept;


            /**  For column email  */
            ///Get the value of the column email, returns the default value if the column is null
            const std::string &getValueOfEmail() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getEmail() const noexcept;

            ///Set the value of the column email
            void setEmail(const std::string &pEmail) noexcept;

            void setEmail(std::string &&pEmail) noexcept;


            /**  For column password  */
            ///Get the value of the column password, returns the default value if the column is null
            const std::string &getValueOfPassword() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getPassword() const noexcept;

            ///Set the value of the column password
            void setPassword(const std::string &pPassword) noexcept;

            void setPassword(std::string &&pPassword) noexcept;


            /**  For column auth_token  */
            ///Get the value of the column auth_token, returns the default value if the column is null
            const std::string &getValueOfAuthToken() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getAuthToken() const noexcept;

            ///Set the value of the column auth_token
            void setAuthToken(const std::string &pAuthToken) noexcept;

            void setAuthToken(std::string &&pAuthToken) noexcept;

            void setAuthTokenToNull() noexcept;


            /**  For column access_token  */
            ///Get the value of the column access_token, returns the default value if the column is null
            const std::string &getValueOfAccessToken() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getAccessToken() const noexcept;

            ///Set the value of the column access_token
            void setAccessToken(const std::string &pAccessToken) noexcept;

            void setAccessToken(std::string &&pAccessToken) noexcept;

            void setAccessTokenToNull() noexcept;


            /**  For column auth_token_expire_time  */
            ///Get the value of the column auth_token_expire_time, returns the default value if the column is null
            const std::string &getValueOfAuthTokenExpireTime() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getAuthTokenExpireTime() const noexcept;

            ///Set the value of the column auth_token_expire_time
            void setAuthTokenExpireTime(const std::string &pAuthTokenExpireTime) noexcept;

            void setAuthTokenExpireTime(std::string &&pAuthTokenExpireTime) noexcept;

            void setAuthTokenExpireTimeToNull() noexcept;


            /**  For column access_token_expire_time  */
            ///Get the value of the column access_token_expire_time, returns the default value if the column is null
            const std::string &getValueOfAccessTokenExpireTime() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getAccessTokenExpireTime() const noexcept;

            ///Set the value of the column access_token_expire_time
            void setAccessTokenExpireTime(const std::string &pAccessTokenExpireTime) noexcept;

            void setAccessTokenExpireTime(std::string &&pAccessTokenExpireTime) noexcept;

            void setAccessTokenExpireTimeToNull() noexcept;


            /**  For column qq  */
            ///Get the value of the column qq, returns the default value if the column is null
            const int64_t &getValueOfQq() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<int64_t> &getQq() const noexcept;

            ///Set the value of the column qq
            void setQq(const int64_t &pQq) noexcept;


            static size_t getColumnNumber() noexcept { return 8; }

            static const std::string &getColumnName(size_t index) noexcept(false);

            Json::Value toJson() const;

            Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
            /// Relationship interfaces
        private:
            friend Mapper<Auth>;

            static const std::vector<std::string> &insertColumns() noexcept;

            void outputArgs(drogon::orm::internal::SqlBinder &binder) const;

            const std::vector<std::string> updateColumns() const;

            void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

            ///For mysql or sqlite3
            void updateId(const uint64_t id);

            std::shared_ptr<int64_t> Id_;
            std::shared_ptr<std::string> email_;
            std::shared_ptr<std::string> password_;
            std::shared_ptr<std::string> authToken_;
            std::shared_ptr<std::string> accessToken_;
            std::shared_ptr<std::string> authTokenExpireTime_;
            std::shared_ptr<std::string> accessTokenExpireTime_;
            std::shared_ptr<int64_t> qq_;
            struct MetaData {
                const std::string colName_;
                const std::string colType_;
                const std::string colDatabaseType_;
                const ssize_t colLength_;
                const bool isAutoVal_;
                const bool isPrimaryKey_;
                const bool notNull_;
            };
            static const std::vector<MetaData> metaData_;
            bool dirtyFlag_[8] = {false};
        public:
            static const std::string &sqlForFindingByPrimaryKey() {
                static const std::string sql = "select * from " + tableName + " where _roomID = $1";
                return sql;
            }

            static const std::string &sqlForDeletingByPrimaryKey() {
                static const std::string sql = "delete from " + tableName + " where _roomID = $1";
                return sql;
            }

            std::string sqlForInserting(bool &needSelection) const {
                std::string sql = "insert into " + tableName + " (";
                size_t parametersCount = 0;
                needSelection = false;
                sql += "_roomID,";
                ++parametersCount;
                sql += "email,";
                ++parametersCount;
                if (!dirtyFlag_[1]) {
                    needSelection = true;
                }
                sql += "password,";
                ++parametersCount;
                if (!dirtyFlag_[2]) {
                    needSelection = true;
                }
                if (dirtyFlag_[3]) {
                    sql += "auth_token,";
                    ++parametersCount;
                }
                if (dirtyFlag_[4]) {
                    sql += "access_token,";
                    ++parametersCount;
                }
                if (dirtyFlag_[5]) {
                    sql += "auth_token_expire_time,";
                    ++parametersCount;
                }
                if (dirtyFlag_[6]) {
                    sql += "access_token_expire_time,";
                    ++parametersCount;
                }
                if (dirtyFlag_[7]) {
                    sql += "qq,";
                    ++parametersCount;
                }
                needSelection = true;
                if (parametersCount > 0) {
                    sql[sql.length() - 1] = ')';
                    sql += " values (";
                } else
                    sql += ") values (";

                int placeholder = 1;
                char placeholderStr[64];
                size_t n = 0;
                sql += "default,";
                if (dirtyFlag_[1]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                } else {
                    sql += "default,";
                }
                if (dirtyFlag_[2]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                } else {
                    sql += "default,";
                }
                if (dirtyFlag_[3]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[4]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[5]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[6]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[7]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (parametersCount > 0) {
                    sql.resize(sql.length() - 1);
                }
                if (needSelection) {
                    sql.append(") returning *");
                } else {
                    sql.append(1, ')');
                }
                LOG_TRACE << sql;
                return sql;
            }
        };
    } // namespace Techmino
} // namespace drogon_model
