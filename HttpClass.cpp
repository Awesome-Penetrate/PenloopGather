//
// Created by payloads on 18-4-9.
//

#include "HttpClass.h"

HttpClass::HttpClass() {
    _serverSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}

void HttpClass::setHost(std::string url) {
    _url.swap(url);
}

void HttpClass::addHeader(std::string header) {
    _header.emplace_back(header);
}

void HttpClass::setIP(std::string ipAddress) {
    _serverIP=inet_addr(ipAddress.c_str());
}

void HttpClass::setPort(unsigned short int port) {
    _port = port;
}

bool HttpClass::request() {
    std::cout << "[*] Http request :" + _url << std::endl;
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = _serverIP;
    _serverAddr.sin_port = htons(_port);
    if(connect(_serverSock,(struct sockaddr *)&_serverAddr, sizeof(_serverAddr))!=0){
        perror("[*] WEB SERVER Connect Fail ... \n");
        return false;
    }
    std::stringstream ss;
    ss <<"GET / HTTP/1.1\r\n"
    <<"Host: " << _url << "\r\n"
    <<"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:52.0) Gecko/20100101 Firefox/52.0\r\n"
    <<"Accept: */*\r\n"
    <<"X-Forwarded-For: 10.74.183.12\r\n"
    <<"Connection: close\r\n"
    <<"Upgrade-Insecure-Requests: 1\r\n"
    <<"Cache-Control: max-age=0\r\n\r\n"<<std::endl;
    std::string buff = ss.str();
    send(_serverSock,buff.data(), buff.size(),0);
    std::stringstream responseStream;
    char recvHeader[2000];
    memset(recvHeader,0,0);
    while(int ret = recv(_serverSock,recvHeader, sizeof(recvHeader),0)){
        recvHeader[ret] = '\0';
        responseStream<<recvHeader;
    }
    memset(recvHeader,0,0);
    std::string response = responseStream.str();
    unsigned long responseLine = response.find("\r\n\r\n");
    responseHeader = response.substr(0,responseLine);
    responseBody = response.substr(responseLine,response.npos);
    htmlcxx::HTML::ParserDom parserDom;
    tree<htmlcxx::HTML::Node>dom = parserDom.parseTree(responseBody);
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    for(; it != end; ++it)
    {
        if (it->tagName()=="title")
        {
            it++;
            title.append(it->text());
            break;
        }
    }
    //std::cout << responseBody << std::endl;
    close(_serverSock);
}

