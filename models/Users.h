/**
 *
 *  Users.h
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

        class Users {
        public:
            struct Cols {
                static const std::string __id;
                static const std::string _email;
                static const std::string _username;
                static const std::string _motto;
                static const std::string _avatar;
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
            explicit Users(const Row &r, const ssize_t indexOffset = 0) noexcept;

            /**
             * @brief constructor
             * @param pJson The json object to construct a new instance.
             */
            explicit Users(const Json::Value &pJson) noexcept(false);

            /**
             * @brief constructor
             * @param pJson The json object to construct a new instance.
             * @param pMasqueradingVector The aliases of table columns.
             */
            Users(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

            Users() = default;

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

            /**  For column _id  */
            ///Get the value of the column _id, returns the default value if the column is null
            const int64_t &getValueOfId() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<int64_t> &getId() const noexcept;

            ///Set the value of the column _id
            void setId(const int64_t &pId) noexcept;


            /**  For column email  */
            ///Get the value of the column email, returns the default value if the column is null
            const std::string &getValueOfEmail() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getEmail() const noexcept;

            ///Set the value of the column email
            void setEmail(const std::string &pEmail) noexcept;

            void setEmail(std::string &&pEmail) noexcept;


            /**  For column username  */
            ///Get the value of the column username, returns the default value if the column is null
            const std::string &getValueOfUsername() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getUsername() const noexcept;

            ///Set the value of the column username
            void setUsername(const std::string &pUsername) noexcept;

            void setUsername(std::string &&pUsername) noexcept;


            /**  For column motto  */
            ///Get the value of the column motto, returns the default value if the column is null
            const std::string &getValueOfMotto() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getMotto() const noexcept;

            ///Set the value of the column motto
            void setMotto(const std::string &pMotto) noexcept;

            void setMotto(std::string &&pMotto) noexcept;

            void setMottoToNull() noexcept;


            /**  For column avatar  */
            ///Get the value of the column avatar, returns the default value if the column is null
            const std::string &getValueOfAvatar() const noexcept;

            ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
            const std::shared_ptr<std::string> &getAvatar() const noexcept;

            ///Set the value of the column avatar
            void setAvatar(const std::string &pAvatar) noexcept;

            void setAvatar(std::string &&pAvatar) noexcept;

            void setAvatarToNull() noexcept;


            static size_t getColumnNumber() noexcept { return 5; }

            static const std::string &getColumnName(size_t index) noexcept(false);

            Json::Value toJson() const;

            Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
            /// Relationship interfaces
        private:
            friend Mapper<Users>;

            static const std::vector<std::string> &insertColumns() noexcept;

            void outputArgs(drogon::orm::internal::SqlBinder &binder) const;

            const std::vector<std::string> updateColumns() const;

            void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

            ///For mysql or sqlite3
            void updateId(const uint64_t id);

            std::shared_ptr<int64_t> Id_;
            std::shared_ptr<std::string> email_;
            std::shared_ptr<std::string> username_;
            std::shared_ptr<std::string> motto_;
            std::shared_ptr<std::string> avatar_;
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
            bool dirtyFlag_[5] = {false};
        public:
            static const std::string &sqlForFindingByPrimaryKey() {
                static const std::string sql = "select * from " + tableName + " where _id = $1";
                return sql;
            }

            static const std::string &sqlForDeletingByPrimaryKey() {
                static const std::string sql = "delete from " + tableName + " where _id = $1";
                return sql;
            }

            std::string sqlForInserting(bool &needSelection) const {
                std::string sql = "insert into " + tableName + " (";
                size_t parametersCount = 0;
                needSelection = false;
                sql += "_id,";
                ++parametersCount;
                if (dirtyFlag_[1]) {
                    sql += "email,";
                    ++parametersCount;
                }
                if (dirtyFlag_[2]) {
                    sql += "username,";
                    ++parametersCount;
                }
                if (dirtyFlag_[3]) {
                    sql += "motto,";
                    ++parametersCount;
                }
                if (dirtyFlag_[4]) {
                    sql += "avatar,";
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
                }
                if (dirtyFlag_[2]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[3]) {
                    n = sprintf(placeholderStr, "$%d,", placeholder++);
                    sql.append(placeholderStr, n);
                }
                if (dirtyFlag_[4]) {
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