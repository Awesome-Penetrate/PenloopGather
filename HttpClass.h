//
// Created by payloads on 18-4-9.
//

#ifndef PENLOOPGATHER_HTTPCLASS_H
#define PENLOOPGATHER_HTTPCLASS_H

#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <htmlcxx/html/ParserDom.h>
class HttpClass {

public:
    HttpClass();
    void setHost(std::string);
    void setIP(std::string);
    void addHeader(std::string);
    void setPort(unsigned short int);
    bool request();
    std::string responseHeader;
    std::string responseBody;
    std::string title;
private:
    std::vector<std::string>_header;
    int _serverSock;
    std::string _url;
    unsigned short int _port;
    struct sockaddr_in _serverAddr;
    in_addr_t _serverIP;
};


#endif //PENLOOPGATHER_HTTPCLASS_H
