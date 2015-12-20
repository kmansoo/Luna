/*
 * ccMongooseWebsocket.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSOCKET_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSOCKET_H_

#include "ccWebServerAPI/ccWebsocket.h"

#include "mongoose/mongoose.h"

class ccMongooseWebsocket : public ccWebsocket
{
public:
    ccMongooseWebsocket(const std::string& strUri, struct mg_connection* con);
    ccMongooseWebsocket(const char* pUri, std::size_t size, struct mg_connection* con);

    virtual ~ccMongooseWebsocket();

protected:
    virtual std::int32_t    GetInstance();  // It may be a Socket ID. 
    virtual bool            Send(const char* strMessage, std::size_t size);

private:
    struct mg_connection*   _pMgConnection;
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */