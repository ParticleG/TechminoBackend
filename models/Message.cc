/**
 *
 *  Message.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Message.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon_model::Techmino;

const std::string Message::Cols::_id = "id";
const std::string Message::Cols::_type = "type";
const std::string Message::Cols::_content = "content";
const std::string Message::primaryKeyName = "id";
const bool Message::hasPrimaryKey = true;
const std::string Message::tableName = "message";

const std::vector<typename Message::MetaData> Message::metaData_={
{"id","int32_t","integer",4,1,1,1},
{"type","std::string","text",0,0,0,1},
{"content","std::string","text",0,0,0,0}
};
const std::string &Message::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Message::Message(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>());
        }
        if(!r["type"].isNull())
        {
            type_=std::make_shared<std::string>(r["type"].as<std::string>());
        }
        if(!r["content"].isNull())
        {
            content_=std::make_shared<std::string>(r["content"].as<std::string>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 3 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            id_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            type_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            content_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

Message::Message(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            type_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            content_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}

Message::Message(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("type"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["type"].isNull())
        {
            type_=std::make_shared<std::string>(pJson["type"].asString());
        }
    }
    if(pJson.isMember("content"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["content"].isNull())
        {
            content_=std::make_shared<std::string>(pJson["content"].asString());
        }
    }
}

void Message::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            type_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            content_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}
                                                                    
void Message::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("type"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["type"].isNull())
        {
            type_=std::make_shared<std::string>(pJson["type"].asString());
        }
    }
    if(pJson.isMember("content"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["content"].isNull())
        {
            content_=std::make_shared<std::string>(pJson["content"].asString());
        }
    }
}

const int32_t &Message::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Message::getId() const noexcept
{
    return id_;
}
void Message::setId(const int32_t &pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}



const typename Message::PrimaryKeyType & Message::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string &Message::getValueOfType() const noexcept
{
    const static std::string defaultValue = std::string();
    if(type_)
        return *type_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Message::getType() const noexcept
{
    return type_;
}
void Message::setType(const std::string &pType) noexcept
{
    type_ = std::make_shared<std::string>(pType);
    dirtyFlag_[1] = true;
}
void Message::setType(std::string &&pType) noexcept
{
    type_ = std::make_shared<std::string>(std::move(pType));
    dirtyFlag_[1] = true;
}




const std::string &Message::getValueOfContent() const noexcept
{
    const static std::string defaultValue = std::string();
    if(content_)
        return *content_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Message::getContent() const noexcept
{
    return content_;
}
void Message::setContent(const std::string &pContent) noexcept
{
    content_ = std::make_shared<std::string>(pContent);
    dirtyFlag_[2] = true;
}
void Message::setContent(std::string &&pContent) noexcept
{
    content_ = std::make_shared<std::string>(std::move(pContent));
    dirtyFlag_[2] = true;
}


void Message::setContentToNull() noexcept
{
    content_.reset();
    dirtyFlag_[2] = true;
}


void Message::updateId(const uint64_t id)
{
}

const std::vector<std::string> &Message::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "type",
        "content"
    };
    return inCols;
}

void Message::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getType())
        {
            binder << getValueOfType();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getContent())
        {
            binder << getValueOfContent();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Message::updateColumns() const
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
    return ret;
}

void Message::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getType())
        {
            binder << getValueOfType();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getContent())
        {
            binder << getValueOfContent();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value Message::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getType())
    {
        ret["type"]=getValueOfType();
    }
    else
    {
        ret["type"]=Json::Value();
    }
    if(getContent())
    {
        ret["content"]=getValueOfContent();
    }
    else
    {
        ret["content"]=Json::Value();
    }
    return ret;
}

Json::Value Message::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 3)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getId())
            {
                ret[pMasqueradingVector[0]]=getValueOfId();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getType())
            {
                ret[pMasqueradingVector[1]]=getValueOfType();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getContent())
            {
                ret[pMasqueradingVector[2]]=getValueOfContent();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getType())
    {
        ret["type"]=getValueOfType();
    }
    else
    {
        ret["type"]=Json::Value();
    }
    if(getContent())
    {
        ret["content"]=getValueOfContent();
    }
    else
    {
        ret["content"]=Json::Value();
    }
    return ret;
}

bool Message::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("type"))
    {
        if(!validJsonOfField(1, "type", pJson["type"], err, true))
            return false;
    }
    else
    {
        err="The type column cannot be null";
        return false;
    }
    if(pJson.isMember("content"))
    {
        if(!validJsonOfField(2, "content", pJson["content"], err, true))
            return false;
    }
    return true;
}
bool Message::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                 const std::vector<std::string> &pMasqueradingVector,
                                                 std::string &err)
{
    if(pMasqueradingVector.size() != 3)
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
    }
    return true;
}
bool Message::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("type"))
    {
        if(!validJsonOfField(1, "type", pJson["type"], err, false))
            return false;
    }
    if(pJson.isMember("content"))
    {
        if(!validJsonOfField(2, "content", pJson["content"], err, false))
            return false;
    }
    return true;
}
bool Message::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                               const std::vector<std::string> &pMasqueradingVector,
                                               std::string &err)
{
    if(pMasqueradingVector.size() != 3)
    {
        err = "Bad masquerading vector";
        return false;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
            return false;
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false))
            return false;
    }
    return true;
}
bool Message::validJsonOfField(size_t index,
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
            if(!pJson.isInt())
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
