/*
 * ccMongooseWebsocket.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebsocket.h"

namespace Luna {

ccMongooseWebsocket::ccMongooseWebsocket(const std::string& uri, struct mg_connection* connection) :
    ccWebsocket(uri),
    mg_connection_(connection) {
}

ccMongooseWebsocket::ccMongooseWebsocket(const char* uri, std::size_t size, struct mg_connection* connection) :
    ccWebsocket(std::string(uri, size)),
    mg_connection_(connection) {
}


ccMongooseWebsocket::~ccMongooseWebsocket() {
}

bool ccMongooseWebsocket::open(const std::string& uri) {
    return false;
}

bool ccMongooseWebsocket::close() {
    return false;
}

std::int32_t ccMongooseWebsocket::get_instance()  // It may be a Socket ID. 
{
    if (mg_connection_ == NULL)
        return -1;

    return mg_connection_->sock;
}

bool ccMongooseWebsocket::send(const std::string& message) {
    if (mg_connection_ == NULL)
        return false;

    mg_send_websocket_frame(mg_connection_, WEBSOCKET_OP_TEXT, message.c_str(), message.length());

    return true;
}

bool ccMongooseWebsocket::send_binary(const void* buffer, std::size_t size) {
    if (mg_connection_ == NULL)
        return false;

    mg_send_websocket_frame(mg_connection_, WEBSOCKET_OP_BINARY, buffer, size);

    return true;
}

}
