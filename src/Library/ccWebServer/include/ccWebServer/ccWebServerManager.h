/*
 * ccWebServerManager.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#pragma once

#include <memory>

#include "ccCore/ccSingleton.h"

#include "ccWebServer.h"
#include "ccWebServerObjectFactory.h"
#include "ccWebServerEventListener.h"

#include "ccRESTfulApi.h"
#include "ccWebsocketManager.h"
#include "ccWebServerPageDirectory.h"

namespace Luna {

class ccWebServerManager : 
    public Luna::ccSingleton<ccWebServerManager>,
    public ccWebServerEventListener {
public:
    ccWebServerManager();
    virtual ~ccWebServerManager();

    friend class ccSingleton<ccWebServerManager>;

public:
    void attach_factory(std::shared_ptr<ccWebServerObjectFactory> factory);

    bool create_web_server(
        const std::string& name,
        const std::string& ports,
        bool enable_cors = false,
        std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);

    bool create_web_server(
        const std::string& name,
        const std::string& ports,
        const std::string& root_path,
        bool enable_cors = false,
        std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);

    std::shared_ptr<ccWebServer> get_web_server(const char* strName);

    void start();
    void stop();

    bool add_restful_api(std::shared_ptr<ccRESTfulApi> new_api);
    bool remove_restful_api(std::shared_ptr<ccRESTfulApi> new_api);
    bool remove_all_restful_api();

    bool add_websocket_manager(std::shared_ptr<ccWebsocketManager> new_websocket_manager);
    bool remove_websocket_manager(std::shared_ptr<ccWebsocketManager> new_websocket_manager);
    bool remove_all_websocket_manager();

protected:
    virtual bool on_web_request(
        std::shared_ptr<ccWebServerRequest> request,
        std::shared_ptr<ccWebServerResponse> response);

    virtual bool on_new_websocket_request(const std::string& websocketuri_);
    virtual void on_websocket_created(std::shared_ptr<ccWebsocket> new_websocket);
    virtual void on_websocket_connected(std::int32_t socket_id);
    virtual void on_websocket_received_data(std::int32_t socket_id, const char* data, uint32_t size, bool is_text);
    virtual void on_websocket_disconnected(std::int32_t socket_id);
    virtual int  on_websocket_check_instance(void* connection_info);

protected:
    void do_perform_websocket_event(ccWebsocket::ccWebSocketEvent event,
                                 std::int32_t socket_id,
                                 const char* data, 
                                 uint32_t size, 
                                 bool is_text);

private:
    std::shared_ptr<ccWebServerObjectFactory> obj_factory_;
    std::vector<std::shared_ptr<ccWebServer> > web_server_list_;

    std::vector<std::shared_ptr<ccRESTfulApi> > web_api_list_;
    std::vector<std::shared_ptr<ccWebsocketManager> > websocket_manager_list_;

    const std::string blank_string_;
    bool enable_cors_ = false;
};

}
