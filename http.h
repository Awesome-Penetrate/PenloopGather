//
// Created by payloads on 18-2-9.
//

#ifndef NEWPRO_HTTP_H
#define NEWPRO_HTTP_H
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>
#include <htmlcxx/html/ParserDom.h>
#include <sstream>
#include "GLOBAL.h"

class http {
public:
    http();
    http(std::string url, unsigned int port = 80);  // 设置请求地址以及端口
    bool setURL(std::string url);    // 设置请求地址
    bool setPort(unsigned int port); // 设置端口
    bool getRequest();  // get 方法
    bool postRequest(std::string data = ""); // post 方法
    bool setCookie(std::string cookie = "");
    bool addRequestHeaders(std::string header);
    std::string getTitle(); // 获取标题
    std::string getResponse(); // 获取响应内容
    std::string getResponse(std::string Key);
    std::string getTargetIP();
    void cleanUp();
    ~http();
private:
    CURL_ERROR ERROR = CURL_OK;
    unsigned int _responseCode;
    unsigned int _timeout = 1;
    unsigned short int _connectTimeout = 1;
    bool _request();
    CURL * curl = NULL;
    CURLcode _res;
    std::string _url;
    unsigned int _port;
    char * _targetIP = NULL;
    std::string _title;
    std::string _responseHeader;
    std::string _responseBody;
    bool isRequested;
    curl_slist * _requestHeaders=NULL;
    curl_slist * _responseCookie=NULL;
    void getErrorMsg();
    static size_t _callBackWrite(char *ptr, size_t n, size_t m, std::string *data);
};


#endif //NEWPRO_HTTP_H
