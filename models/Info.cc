/**
 *
 *  Info.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Info.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon_model::Techmino;

const std::string Info::Cols::__id = "_id";
const std::string Info::Cols::_email = "email";
const std::string Info::Cols::_username = "username";
const std::string Info::Cols::_motto = "motto";
const std::string Info::Cols::_avatar = "avatar";
const std::string Info::primaryKeyName = "email";
const bool Info::hasPrimaryKey = true;
const std::string Info::tableName = "info";

const std::vector<typename Info::MetaData> Info::metaData_={
{"_id","int64_t","bigint",8,1,0,1},
{"email","std::string","text",0,0,1,1},
{"username","std::string","text",0,0,0,1},
{"motto","std::string","text",0,0,0,0},
{"avatar","std::string","text",0,0,0,0}
};
const std::string &Info::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Info::Info(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["_id"].isNull())
        {
            Id_=std::make_shared<int64_t>(r["_id"].as<int64_t>());
        }
        if(!r["email"].isNull())
        {
            email_=std::make_shared<std::string>(r["email"].as<std::string>());
        }
        if(!r["username"].isNull())
        {
            username_=std::make_shared<std::string>(r["username"].as<std::string>());
        }
        if(!r["motto"].isNull())
        {
            motto_=std::make_shared<std::string>(r["motto"].as<std::string>());
        }
        if(!r["avatar"].isNull())
        {
            avatar_=std::make_shared<std::string>(r["avatar"].as<std::string>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 5 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            Id_=std::make_shared<int64_t>(r[index].as<int64_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            email_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            username_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 3;
        if(!r[index].isNull())
        {
            motto_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 4;
        if(!r[index].isNull())
        {
            avatar_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

Info::Info(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 5)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            Id_=std::make_shared<int64_t>((int64_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            username_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            motto_=std::make_shared<std::string>(pJson[pMasqueradingVector[3]].asString());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            avatar_=std::make_shared<std::string>(pJson[pMasqueradingVector[4]].asString());
        }
    }
}

Info::Info(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("_id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["_id"].isNull())
        {
            Id_=std::make_shared<int64_t>((int64_t)pJson["_id"].asInt64());
        }
    }
    if(pJson.isMember("email"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("username"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["username"].isNull())
        {
            username_=std::make_shared<std::string>(pJson["username"].asString());
        }
    }
    if(pJson.isMember("motto"))
    {
        dirtyFlag_[3]=true;
        if(!pJson["motto"].isNull())
        {
            motto_=std::make_shared<std::string>(pJson["motto"].asString());
        }
    }
    if(pJson.isMember("avatar"))
    {
        dirtyFlag_[4]=true;
        if(!pJson["avatar"].isNull())
        {
            avatar_=std::make_shared<std::string>(pJson["avatar"].asString());
        }
    }
}

void Info::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 5)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            Id_=std::make_shared<int64_t>((int64_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            username_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            motto_=std::make_shared<std::string>(pJson[pMasqueradingVector[3]].asString());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            avatar_=std::make_shared<std::string>(pJson[pMasqueradingVector[4]].asString());
        }
    }
}
                                                                    
void Info::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("_id"))
    {
        if(!pJson["_id"].isNull())
        {
            Id_=std::make_shared<int64_t>((int64_t)pJson["_id"].asInt64());
        }
    }
    if(pJson.isMember("email"))
    {
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("username"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["username"].isNull())
        {
            username_=std::make_shared<std::string>(pJson["username"].asString());
        }
    }
    if(pJson.isMember("motto"))
    {
        dirtyFlag_[3] = true;
        if(!pJson["motto"].isNull())
        {
            motto_=std::make_shared<std::string>(pJson["motto"].asString());
        }
    }
    if(pJson.isMember("avatar"))
    {
        dirtyFlag_[4] = true;
        if(!pJson["avatar"].isNull())
        {
            avatar_=std::make_shared<std::string>(pJson["avatar"].asString());
        }
    }
}

const int64_t &Info::getValueOfId() const noexcept
{
    const static int64_t defaultValue = int64_t();
    if(Id_)
        return *Id_;
    return defaultValue;
}
const std::shared_ptr<int64_t> &Info::getId() const noexcept
{
    return Id_;
}
void Info::setId(const int64_t &pId) noexcept
{
    Id_ = std::make_shared<int64_t>(pId);
    dirtyFlag_[0] = true;
}




const std::string &Info::getValueOfEmail() const noexcept
{
    const static std::string defaultValue = std::string();
    if(email_)
        return *email_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Info::getEmail() const noexcept
{
    return email_;
}
void Info::setEmail(const std::string &pEmail) noexcept
{
    email_ = std::make_shared<std::string>(pEmail);
    dirtyFlag_[1] = true;
}
void Info::setEmail(std::string &&pEmail) noexcept
{
    email_ = std::make_shared<std::string>(std::move(pEmail));
    dirtyFlag_[1] = true;
}



const typename Info::PrimaryKeyType & Info::getPrimaryKey() const
{
    assert(email_);
    return *email_;
}

const std::string &Info::getValueOfUsername() const noexcept
{
    const static std::string defaultValue = std::string();
    if(username_)
        return *username_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Info::getUsername() const noexcept
{
    return username_;
}
void Info::setUsername(const std::string &pUsername) noexcept
{
    username_ = std::make_shared<std::string>(pUsername);
    dirtyFlag_[2] = true;
}
void Info::setUsername(std::string &&pUsername) noexcept
{
    username_ = std::make_shared<std::string>(std::move(pUsername));
    dirtyFlag_[2] = true;
}




const std::string &Info::getValueOfMotto() const noexcept
{
    const static std::string defaultValue = std::string();
    if(motto_)
        return *motto_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Info::getMotto() const noexcept
{
    return motto_;
}
void Info::setMotto(const std::string &pMotto) noexcept
{
    motto_ = std::make_shared<std::string>(pMotto);
    dirtyFlag_[3] = true;
}
void Info::setMotto(std::string &&pMotto) noexcept
{
    motto_ = std::make_shared<std::string>(std::move(pMotto));
    dirtyFlag_[3] = true;
}


void Info::setMottoToNull() noexcept
{
    motto_.reset();
    dirtyFlag_[3] = true;
}


const std::string &Info::getValueOfAvatar() const noexcept
{
    const static std::string defaultValue = std::string();
    if(avatar_)
        return *avatar_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Info::getAvatar() const noexcept
{
    return avatar_;
}
void Info::setAvatar(const std::string &pAvatar) noexcept
{
    avatar_ = std::make_shared<std::string>(pAvatar);
    dirtyFlag_[4] = true;
}
void Info::setAvatar(std::string &&pAvatar) noexcept
{
    avatar_ = std::make_shared<std::string>(std::move(pAvatar));
    dirtyFlag_[4] = true;
}


void Info::setAvatarToNull() noexcept
{
    avatar_.reset();
    dirtyFlag_[4] = true;
}


void Info::updateId(const uint64_t id)
{
}

const std::vector<std::string> &Info::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "email",
        "username",
        "motto",
        "avatar"
    };
    return inCols;
}

void Info::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getEmail())
        {
            binder << getValueOfEmail();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getUsername())
        {
            binder << getValueOfUsername();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[3])
    {
        if(getMotto())
        {
            binder << getValueOfMotto();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[4])
    {
        if(getAvatar())
        {
            binder << getValueOfAvatar();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Info::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
    {
        ret.push_back(getColumnName(1));
    }
    if(dirtyFlag_[2])
    {
        ret.push_back(getColumnName(2));
    }
    if(dirtyFlag_[3])
    {
        ret.push_back(getColumnName(3));
    }
    if(dirtyFlag_[4])
    {
        ret.push_back(getColumnName(4));
    }
    return ret;
}

void Info::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getEmail())
        {
            binder << getValueOfEmail();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getUsername())
        {
            binder << getValueOfUsername();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[3])
    {
        if(getMotto())
        {
            binder << getValueOfMotto();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[4])
    {
        if(getAvatar())
        {
            binder << getValueOfAvatar();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value Info::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["_id"]=(Json::Int64)getValueOfId();
    }
    else
    {
        ret["_id"]=Json::Value();
    }
    if(getEmail())
    {
        ret["email"]=getValueOfEmail();
    }
    else
    {
        ret["email"]=Json::Value();
    }
    if(getUsername())
    {
        ret["username"]=getValueOfUsername();
    }
    else
    {
        ret["username"]=Json::Value();
    }
    if(getMotto())
    {
        ret["motto"]=getValueOfMotto();
    }
    else
    {
        ret["motto"]=Json::Value();
    }
    if(getAvatar())
    {
        ret["avatar"]=getValueOfAvatar();
    }
    else
    {
        ret["avatar"]=Json::Value();
    }
    return ret;
}

Json::Value Info::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 5)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getId())
            {
                ret[pMasqueradingVector[0]]=(Json::Int64)getValueOfId();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getEmail())
            {
                ret[pMasqueradingVector[1]]=getValueOfEmail();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getUsername())
            {
                ret[pMasqueradingVector[2]]=getValueOfUsername();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[3].empty())
        {
            if(getMotto())
            {
                ret[pMasqueradingVector[3]]=getValueOfMotto();
            }
            else
            {
                ret[pMasqueradingVector[3]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[4].empty())
        {
            if(getAvatar())
            {
                ret[pMasqueradingVector[4]]=getValueOfAvatar();
            }
            else
            {
                ret[pMasqueradingVector[4]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId())
    {
        ret["_id"]=(Json::Int64)getValueOfId();
    }
    else
    {
        ret["_id"]=Json::Value();
    }
    if(getEmail())
    {
        ret["email"]=getValueOfEmail();
    }
    else
    {
        ret["email"]=Json::Value();
    }
    if(getUsername())
    {
        ret["username"]=getValueOfUsername();
    }
    else
    {
        ret["username"]=Json::Value();
    }
    if(getMotto())
    {
        ret["motto"]=getValueOfMotto();
    }
    else
    {
        ret["motto"]=Json::Value();
    }
    if(getAvatar())
    {
        ret["avatar"]=getValueOfAvatar();
    }
    else
    {
        ret["avatar"]=Json::Value();
    }
    return ret;
}

bool Info::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("_id"))
    {
        if(!validJsonOfField(0, "_id", pJson["_id"], err, true))
            return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(1, "email", pJson["email"], err, true))
            return false;
    }
    else
    {
        err="The email column cannot be null";
        return false;
    }
    if(pJson.isMember("username"))
    {
        if(!validJsonOfField(2, "username", pJson["username"], err, true))
            return false;
    }
    else
    {
        err="The username column cannot be null";
        return false;
    }
    if(pJson.isMember("motto"))
    {
        if(!validJsonOfField(3, "motto", pJson["motto"], err, true))
            return false;
    }
    if(pJson.isMember("avatar"))
    {
        if(!validJsonOfField(4, "avatar", pJson["avatar"], err, true))
            return false;
    }
    return true;
}
bool Info::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                              const std::vector<std::string> &pMasqueradingVector,
                                              std::string &err)
{
    if(pMasqueradingVector.size() != 5)
    {
        err = "Bad masquerading vector";
        return false;
    }
    if(!pMasqueradingVector[0].empty())
    {
        if(pJson.isMember(pMasqueradingVector[0]))
        {
            if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true))
                return false;
        }
    }
    if(!pMasqueradingVector[1].empty())
    {
        if(pJson.isMember(pMasqueradingVector[1]))
        {
            if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true))
                return false;
        }
        else
        {
            err="The " + pMasqueradingVector[1] + " column cannot be null";
            return false;
        }
    }
    if(!pMasqueradingVector[2].empty())
    {
        if(pJson.isMember(pMasqueradingVector[2]))
        {
            if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true))
                return false;
        }
        else
        {
            err="The " + pMasqueradingVector[2] + " column cannot be null";
            return false;
        }
    }
    if(!pMasqueradingVector[3].empty())
    {
        if(pJson.isMember(pMasqueradingVector[3]))
        {
            if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, true))
                return false;
        }
    }
    if(!pMasqueradingVector[4].empty())
    {
        if(pJson.isMember(pMasqueradingVector[4]))
        {
            if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, true))
                return false;
        }
    }
    return true;
}
bool Info::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("_id"))
    {
        if(!validJsonOfField(0, "_id", pJson["_id"], err, false))
            return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(1, "email", pJson["email"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("username"))
    {
        if(!validJsonOfField(2, "username", pJson["username"], err, false))
            return false;
    }
    if(pJson.isMember("motto"))
    {
        if(!validJsonOfField(3, "motto", pJson["motto"], err, false))
            return false;
    }
    if(pJson.isMember("avatar"))
    {
        if(!validJsonOfField(4, "avatar", pJson["avatar"], err, false))
            return false;
    }
    return true;
}
bool Info::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector,
                                            std::string &err)
{
    if(pMasqueradingVector.size() != 5)
    {
        err = "Bad masquerading vector";
        return false;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
            return false;
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false))
            return false;
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, false))
            return false;
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, false))
            return false;
    }
    return true;
}
bool Info::validJsonOfField(size_t index,
                            const std::string &fieldName,
                            const Json::Value &pJson, 
                            std::string &err, 
                            bool isForCreation)
{
    switch(index)
    {
        case 0:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(isForCreation)
            {
                err="The automatic primary key cannot be set";
                return false;
            }
            else
            {
                err="The automatic primary key cannot be update";
                return false;
            }
            if(!pJson.isInt64())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 1:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
        case 2:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
        case 3:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
        case 4:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
     
        default:
            err="Internal error in the server";
            return false;
            break;
    }
    return true;
}
