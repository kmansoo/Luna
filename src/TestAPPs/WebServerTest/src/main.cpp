
#include <memory>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"

#include "ccWebServer/ccWebServerManager.h"

#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"
#include "ccRESTfulChattingAPI/ccRESTfulChatting.h"

#include "ccWebServer/ccWebsocketManager.h"

#include "ccWebsocketClient/ccEasyWebsocketClient.h"

class ccChattingWSManager : public ccWebsocketManager
{
public:
    ccChattingWSManager()
    {
        this->AddFunction("/ws", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        this->AddFunction("/ws_chat", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

public:
    bool ws_chat(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string strData)
    {
        switch ((int)eEvent)
        {
        case ccWebsocket::ccWebSocketEvent_Connected:
            {
                std::string strMessage;

                ccString::format(strMessage, "%08x: Join", pWS->GetInstance());

                pWS->GetGroup()->Broadcast(strMessage);
            }
            break;

        case ccWebsocket::ccWebSocketEvent_ReceivedData:
            {
                std::string strBroadcastMsg;
                std::string strMessage(strData);

                ccString::format(strBroadcastMsg, "%08x: %s", pWS->GetInstance(), strMessage.c_str());

                pWS->GetGroup()->Broadcast(strBroadcastMsg);
            }
            break;

        case ccWebsocket::ccWebSocketEvent_Disconnected:
            {
                std::string strMessage;

                ccString::format(strMessage, "%08x: Leave", pWS->GetInstance());

                pWS->GetGroup()->Broadcast(strMessage);
            }
            break;
        }
        return true;
    }

};

#ifdef WIN32
#   include <Winsock2.h>
#endif

int main(int argc, char* argv[])
{
#ifdef WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err;

    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
    {
        //// could not find a usable WinSock DLL
        ////cerr << "Could not load winsock" << endl;
        //assert(0); // is this is failing, try a different version that 2.2, 1.0 or later will likely work
        //exit(1);
    }
#endif

    //  choose a Web Server : Abstract Factory Design Pattern

    ccWebServerManager::getInstance().AttachFactory(std::make_shared<ccMongooseWebServerObjectFactory>());
    //oManager.AttachFactory(std::make_shared<ccBoostWebServerObjectFactory>());

    std::shared_ptr<ccRESTfulChatting>      pChattingApi(new ccRESTfulChatting);
    std::shared_ptr<ccWebsocketManager>     pChattingWSM(new ccChattingWSManager);

    ccWebServerManager::getInstance().CreateWebServer("WebServer #1", "8000", ".");

    ccWebServerManager::getInstance().AddRESTfulApi(pChattingApi);
    ccWebServerManager::getInstance().AddWebsocketManager(pChattingApi);

    ccWebServerManager::getInstance().AddWebsocketManager(pChattingWSM);

    ccWebServerManager::getInstance().Start();

    while (1)
        Luna::sleep(100);
}
