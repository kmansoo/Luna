/*
 * ccRESTfulWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <iostream>
#include <functional>

#include <iomanip>
#include <ctime>

#include <sstream>

#include "ccCore/ccString.h"

#include "ccJsonParser/json/value.h"
#include "ccJsonParser/json/writer.h"
#include "ccJsonParser/json/reader.h"

#include "../ccRESTfulChatting.h"

///
ccRESTfulChattingSessionInfo::ccRESTfulChattingSessionInfo(const std::string& strSessionName, const std::string& strOwnerID) : 
    _strName(strSessionName),
    _strOwnerID(strOwnerID)
{
    _aMembers.push_back(strOwnerID);
 
}

bool ccRESTfulChattingSessionInfo::join(const std::string& strUserID)
{
    for (auto user : _aMembers)
    {
        if (user == strUserID)
            return true;
    }

    _aMembers.push_back(strUserID);

    return true;
}

bool ccRESTfulChattingSessionInfo::leave(const std::string& strUserID)
{
    for (auto it = _aMembers.begin(); it != _aMembers.end(); it++)
    {
        if (*it == strUserID)
        {
            _aMembers.erase(it);
            return true;
        }
    }

    return false;
}

bool  ccRESTfulChattingSessionInfo::getMemberList(std::vector<std::string>& aUserIDs)
{
    for (auto user : _aMembers)
        aUserIDs.push_back(user);

    return true;
}

bool ccRESTfulChattingSessionInfo::newMessage(const std::string& strUserID, const std::string& strMessage)
{
    std::shared_ptr<ccRESTfulChattingMessageInfo> pNewMessage(new ccRESTfulChattingMessageInfo);

    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream ss;
    
#ifndef	LUNA_PLATFORM_RASPBERRY_PI2
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
#else
    //	std::put_time can use in over gcc 5.2 version
#endif

    pNewMessage->_strDateTime   = ss.str();
    pNewMessage->_strFromID     = strUserID;
    pNewMessage->_strMessage    = strMessage;

    return true;
}

//--
//  ccRESTfulChattingSessionInfo
//
bool ccRESTfulChattingSessionManager::createID(const std::string& strID, const std::string& strName)
{
    for (auto userInfo : _aUserInfos)
    {
        if (userInfo->_strID == strID)
        {
            std::cout << "[ccRESTfulChattingSessionManager::CreateID] '" << strID << "' is a registered ID!" << std::endl;
            return false;
        }
    }

    std::shared_ptr<ccRESTfulChattingUserInfo> pNewUser(new ccRESTfulChattingUserInfo);

    pNewUser->_strID = strID;
    pNewUser->_strName = strName;

    _aUserInfos.push_back(pNewUser);

    std::cout << "[ccRESTfulChattingSessionManager::CreateID] '" << strID << "'/'" << strName << "'." << std::endl;

    return true;
}

bool ccRESTfulChattingSessionManager::deleteID(const std::string& strID)
{
    for (auto it = _aUserInfos.begin(); it != _aUserInfos.end(); it++)
    {
        if ((*it)->_strID == strID)
        {
            std::cout << "[ccRESTfulChattingSessionManager::DeleteID] '" << strID << "' was deleted!" << std::endl;

            _aUserInfos.erase(it);
            return true;
        }
    }

    std::cout << "[ccRESTfulChattingSessionManager::DeleteID] '" << strID << "' is not a registered ID!" << std::endl;

    return false;
}

bool ccRESTfulChattingSessionManager::createSession(const std::string& strName, const std::string& strOwnerID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return false;
    }

    std::shared_ptr<ccRESTfulChattingSessionInfo> pNewSession(new ccRESTfulChattingSessionInfo(strName, strOwnerID));

    _aSessionInfos.push_back(pNewSession);

    return true;
}

bool ccRESTfulChattingSessionManager::deleteSession(const std::string& strName, const std::string& strOwnerID)
{
    for (auto it = _aSessionInfos.begin(); it != _aSessionInfos.end(); it++)
    {
        if ((*it)->_strName == strName && (*it)->_strOwnerID == strOwnerID)
        {
            _aSessionInfos.erase(it);
            return true;
        }
    }

    return false;
}

bool ccRESTfulChattingSessionManager::joinSession(const std::string& strName, const std::string& strUserID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return sessionInfo->join(strUserID);
    }

    return false;
}

bool ccRESTfulChattingSessionManager::leaveSession(const std::string& strName, const std::string& strUserID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return sessionInfo->leave(strUserID);
    }

    return false;
}

bool ccRESTfulChattingSessionManager::getUserInfo(const std::string& strID, ccRESTfulChattingUserInfo& oUserInfo)
{
    for (auto info : _aUserInfos)
    {
        if (strID == info->_strID)
        {
            oUserInfo = *info;

            return true;
        }
    }

    return false;
}


bool ccRESTfulChattingSessionManager::getUserList(std::vector<std::string>& aUserIDs)
{
    aUserIDs.clear();

    for (auto info : _aUserInfos)
        aUserIDs.push_back(info->_strID);

    return true;
}

bool ccRESTfulChattingSessionManager::getSessionList(std::vector<std::string>& aSessionNames, std::vector<std::string>& aOwnerIDs)
{
    aSessionNames.clear();

    for (auto info : _aSessionInfos)
    {
        aSessionNames.push_back(info->_strName);
        aOwnerIDs.push_back(info->_strOwnerID);
    }

    return true;
}

bool ccRESTfulChattingSessionManager::getSessionInfo(const std::string& strSessionName, std::vector<std::string>& aMembers)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strSessionName)
            return sessionInfo->getMemberList(aMembers);
    }


    return false;
}

bool  ccRESTfulChattingSessionManager::broadcastMessage(const std::string& strSessionName, const std::string& strUserID, const std::string& strMessage)
{

    return false;
}


/////
ccRESTfulChatting::ccRESTfulChatting()
{
    // TODO Auto-generated constructor stub
    addAPI(std::string("/user"), std::bind(&ccRESTfulChatting::user, this, std::placeholders::_1, std::placeholders::_2));
    addAPI(std::string("/session"), std::bind(&ccRESTfulChatting::session, this, std::placeholders::_1, std::placeholders::_2));
    addAPI(std::string("/session/member"), std::bind(&ccRESTfulChatting::session_member, this, std::placeholders::_1, std::placeholders::_2));
    addAPI(std::string("/session/member/*"), std::bind(&ccRESTfulChatting::session_member_id, this, std::placeholders::_1, std::placeholders::_2));
    //  AddAPI(std::string("/session/message"), std::bind(&ccRESTfulChatting::session_message, this, std::placeholders::_1, std::placeholders::_2));
}

ccRESTfulChatting::~ccRESTfulChatting()
{
    // TODO Auto-generated destructor stub
}

bool ccRESTfulChatting::user(std::shared_ptr<Luna::ccWebServerRequest> pRequest, std::shared_ptr<Luna::ccWebServerResponse> pResponse)
{
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->getMethod())
    {
    case Luna::ccWebServerRequest::HttpMethod_Post:   //  Create
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->getContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            if (oRootValue["user_info"].isObject() == false)
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            //  Json::Value oUserInfp = oRootValue["user_info"];
            if (_oSessionManager.createID(oRootValue["user_info"]["ID"].asString(), oRootValue["user_info"]["Name"].asString()) == true)
                pResponse->status(201, std::string("Created"));
            else
                pResponse->status(500, std::string("Server Error!"));
    
            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    case Luna::ccWebServerRequest::HttpMethod_Get:    //  Read
        {
            std::vector<std::string> aMembers;

            _oSessionManager.getUserList(aMembers);

            pResponse->status(200, std::string("OK"));

            Json::Value oResponseJsonData;
            Json::Value oUserList;
            Json::StyledWriter oWriter;
            
            oUserList["count"] = (int)aMembers.size();

            for (std::size_t nIndex = 0; nIndex < aMembers.size(); nIndex++)
                oUserList["info"][nIndex]["id"] = aMembers[nIndex];

            oResponseJsonData["user_list"] = oUserList;

            std::string strJsonData = oWriter.write(oResponseJsonData);

            pResponse->contentType("application/javascript", strJsonData.length());
            pResponse->write(strJsonData);

            return true;
        }
        break;

    case Luna::ccWebServerRequest::HttpMethod_Delete:   //  Delete
        {
            std::string strID = pRequest->getVar("ID");

            if (_oSessionManager.deleteID(strID) == true)
                pResponse->status(202, std::string("OK"));
            else
                pResponse->status(500, std::string("Server Error!"));
    
            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
    }
        break;

    default:
        pResponse->status(500, std::string("Server Error!"));
        pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return false;
}

bool ccRESTfulChatting::session(std::shared_ptr<Luna::ccWebServerRequest> pRequest, std::shared_ptr<Luna::ccWebServerResponse> pResponse)
{
    switch (pRequest->getMethod())
    {
    case Luna::ccWebServerRequest::HttpMethod_Get:
        {
            std::cout << "enter ccRESTfulChatting::session" << std::endl;

            Json::Value oResponseJsonValue;
            Json::Value oSessionList;
            Json::StyledWriter oWriter;

            std::vector<std::string> aSessionNames, aOwnerIDs;

            _oSessionManager.getSessionList(aSessionNames, aOwnerIDs);

            oSessionList["count"] = (int)aSessionNames.size();

            for (std::size_t nIndex = 0; nIndex < aSessionNames.size(); nIndex++)
            {
                oSessionList["info"][nIndex]["Name"] = aSessionNames[nIndex];
                oSessionList["info"][nIndex]["OwnerID"] = aOwnerIDs[nIndex];
            }

            oResponseJsonValue["session_list"] = oSessionList;

            std::string strJsonData = oWriter.write(oResponseJsonValue);

            pResponse->status(200, std::string("OK"));
            pResponse->contentType("application/javascript", strJsonData.length());
            pResponse->write(strJsonData);
        }
        break;

    case Luna::ccWebServerRequest::HttpMethod_Post:
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->getContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            if (oRootValue["session_info"].isObject() == false)
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            std::string strSessionName = oRootValue["session_info"]["Name"].asString();
            std::string strOwnerID = oRootValue["session_info"]["OwnerID"].asString();

            if (_oSessionManager.createSession(strSessionName, strOwnerID) == true)
            {
                pResponse->status(201, std::string("Created"));

                std::string strWSUrl;

                Luna::ccString::format(strWSUrl, "/session/chat/%s", strSessionName.c_str());
                
                addFunction(strWSUrl, std::bind(&ccRESTfulChatting::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            }
            else
                pResponse->status(500, std::string("Server Error!"));
    
            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    case Luna::ccWebServerRequest::HttpMethod_Delete:
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->getContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            if (oRootValue["session_info"].isObject() == false)
            {
                pResponse->status(500, std::string("Server Error!"));
                return false;
            }

            std::string strSessionName = oRootValue["session_info"]["Name"].asString();
            std::string strOwnerID = oRootValue["session_info"]["OwnerID"].asString();

            if (_oSessionManager.deleteSession(strSessionName, strOwnerID) == true)
            {
                pResponse->status(202, std::string("OK"));

                std::string strWSUrl;

                Luna::ccString::format(strWSUrl, "/session/chat/%s", strSessionName.c_str());
                
                removeFunction(strWSUrl);
            }
            else
                pResponse->status(500, std::string("Server Error!"));

            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    default:
        pResponse->status(500, std::string("Server Error!"));
        pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return true;
}

bool ccRESTfulChatting::session_member(std::shared_ptr<Luna::ccWebServerRequest> pRequest, std::shared_ptr<Luna::ccWebServerResponse> pResponse)
{
    switch (pRequest->getMethod())
    {
        case Luna::ccWebServerRequest::HttpMethod_Get:
        {
            std::string strSessionName = pRequest->getVar("Name");
            std::string strUserID = pRequest->getVar("UserID");

            std::vector<std::string> aMemberList;

            _oSessionManager.getSessionInfo(strSessionName, aMemberList);

            Json::Value oResponseJsonValue;
            Json::Value oSessionMemberList;
            Json::StyledWriter oWriter;
            oSessionMemberList["Name"] = strSessionName;
            oSessionMemberList["count"] = (int)aMemberList.size();

            for (std::size_t nIndex = 0; nIndex < aMemberList.size(); nIndex++)
                oSessionMemberList["info"][nIndex]["ID"] = aMemberList[nIndex];

            oResponseJsonValue["session_member_list"] = oSessionMemberList;

            std::string strJsonData = oWriter.write(oResponseJsonValue);

            pResponse->status(200, std::string("OK"));
            pResponse->contentType("application/javascript", strJsonData.length());
            pResponse->write(strJsonData);
        }
        break;

        case Luna::ccWebServerRequest::HttpMethod_Put:    //  join
        {
            std::string strSessionName = pRequest->getVar("Name");
            std::string strUserID = pRequest->getVar("UserID");

            if (_oSessionManager.joinSession(strSessionName, strUserID) == true)
                pResponse->status(200, std::string("OK"));
            else
                pResponse->status(500, std::string("Server Error!"));

            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

        case Luna::ccWebServerRequest::HttpMethod_Delete:
        {
            std::string strSessionName = pRequest->getVar("Name");
            std::string strUserID = pRequest->getVar("UserID");

            if (_oSessionManager.leaveSession(strSessionName, strUserID) == true)
                pResponse->status(202, std::string("OK"));
            else
                pResponse->status(500, std::string("Server Error!"));

            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

        default:
            pResponse->status(500, std::string("Server Error!"));
            pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
            break;
    }

    return true;
}

bool ccRESTfulChatting::session_member_id(std::shared_ptr<Luna::ccWebServerRequest> pRequest, std::shared_ptr<Luna::ccWebServerResponse> pResponse)
{
    //  I'll implement this method as soon as possible.
    std::string strUserID = pRequest->getResource();

    pResponse->status(500, std::string("Server Error!"));
    pResponse->contentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);

    return true;
}

//  
bool ccRESTfulChatting::ws_chat(Luna::ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<Luna::ccWebsocket> pWS, const std::string& strData)
{
    switch ((int)eEvent)
    {
    case Luna::ccWebsocket::ccWebSocketEvent_Connected:
    {
        std::string strMessage;

        Luna::ccString::format(strMessage, "%08x: Join", pWS->getInstance());

        pWS->getGroup()->broadcast(strMessage);
    }
    break;

    case Luna::ccWebsocket::ccWebSocketEvent_ReceivedData:
    {
        std::string strBroadcastMsg;

        Luna::ccString::format(strBroadcastMsg, "%08x: %s", pWS->getInstance(), strData.c_str());

        pWS->getGroup()->broadcast(strBroadcastMsg);
    }
    break;

    case Luna::ccWebsocket::ccWebSocketEvent_Disconnected:
    {
        std::string strMessage;

        Luna::ccString::format(strMessage, "%08x: Leave", pWS->getInstance());

        pWS->getGroup()->broadcast(strMessage);
    }
    break;
    }
    return true;
}
