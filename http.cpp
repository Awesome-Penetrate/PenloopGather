//
// Created by payloads on 18-2-9.
//

#include "http.h"




/**
 * 初始化请求对象
 */
http::http() {
    if(curl == NULL){
        isRequested = false;
        curl = curl_easy_init();
    }
}

/**
 * 回收资源
 */
http::~http() {
    cleanUp();
}

/**
 * 设置请求地址和端口
 * @param url
 * @param port
 */
http::http(std::string url, unsigned int port) {
    if(curl == NULL){
        curl = curl_easy_init();
        _url  = url;
        if(url.substr(0,5) == "https"){
            _port = 443;
        }else{
            _port = port;
        }
        curl_easy_setopt(curl,CURLOPT_URL,_url.c_str());
        curl_easy_setopt(curl,CURLOPT_PORT,_port);

    }
}

/**
 * 设置请求地址
 * @param url
 * @return
 */
bool http::setURL(std::string url) {

    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
}

/**
 * 设置端口
 * @param port
 * @return
 */
bool http::setPort(unsigned int port) {
    curl_easy_setopt(curl,CURLOPT_PORT,port);
}


/**
 * 发送请求
 * @return
 */
bool http::_request() {

    curl_easy_setopt(curl,CURLOPT_TIMEOUT,_timeout);
    curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,_connectTimeout);
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,false);
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,false);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,_callBackWrite);
    curl_easy_setopt(curl,CURLOPT_WRITEHEADER,&_responseHeader);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&_responseBody);

    if(_requestHeaders != NULL){
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,_requestHeaders);
    }
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 0);
    _res = curl_easy_perform(curl);
    if(_res != CURLE_OK){
        ERROR = CURL_REQUEST_ERROR;
        getErrorMsg();
    }
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&_responseCode);
    curl_easy_getinfo(curl,CURLINFO_PRIMARY_IP,&_targetIP);
    isRequested = true;
}

/**
 * 回调函数
 * @param ptr
 * @param n
 * @param m
 * @param data
 * @return
 */
size_t http::_callBackWrite(char *ptr, size_t n, size_t m, std::string *data) {
    if (data == NULL) return 0;
    data->append(ptr,n*m);
    return n*m;
}

/**
 * get请求
 * @return
 */
bool http::getRequest() {
    _request();
}

/**
 * post请求
 * @param data
 * @return
 */
bool http::postRequest(std::string data) {
    curl_easy_setopt(curl,CURLOPT_POST,1);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data.c_str());
    _request();
}


/**
 * 设置Cookie
 * @param cookie
 * @return
 */
bool http::setCookie(std::string cookie) {
    curl_easy_setopt(curl,CURLOPT_COOKIE,cookie.c_str());
}

/**
 * 设置请求头
 * @param header
 * @return
 */
bool http::addRequestHeaders(std::string header) {
    _requestHeaders = curl_slist_append(_requestHeaders,header.c_str());
}

/**
 * 返回页面标题
 * @return
 */
std::string http::getTitle() {
    htmlcxx::HTML::ParserDom parserDom;
    tree<htmlcxx::HTML::Node>dom = parserDom.parseTree(_responseBody);
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    for(; it != end; ++it)
    {
        std::cout << "[*]Tage Name :" << it->tagName() << std::endl;
        if (it->tagName()=="title")
        {
            it++;
            _title.append(it->text());
        }
    }
    return _title;
}

/**
 * 返回响应头
 * @return
 */
std::string http::getResponse() {
    return _responseHeader;
}

/**
 * 通过键名获取键值
 * @param Key
 * @return
 */
std::string http::getResponse(std::string Key) {
    std::string _del = "\r\n";
    std::string::size_type pos1, pos2;
    std::string _lineStr;
    pos2 = _responseHeader.find(_del);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        _lineStr  =  _responseHeader.substr(pos1, pos2-pos1);
        if(_lineStr.find(Key) == 0){
            return _lineStr.substr(Key.length()+2);
        }

        pos1 = pos2 + _del.size();
        pos2 = _responseHeader.find(_del, pos1);
    }
    if(pos1 != _responseHeader.length()){
        if(_lineStr.find(Key) == 0){
            return _lineStr.substr(Key.length()+2);
        }
    }
    return NULL;
}

/**
 * get target ip address .
 * @return char *
 */
std::string http::getTargetIP() {
    return _targetIP;
}

/**
 *
 */
void http::getErrorMsg() {
    switch (ERROR){
        case CURL_REQUEST_ERROR:
        std::cout << "CURL REQUEST ERROR" << std::endl;
            break;
        case CURL_URL_ERROR:
            std::cout << "CURL URL ERROR" << std::endl;
            break;
        default:
            std::cout << "UNKNOWN ERROR" << std::endl;
    }
    exit(EXIT_FAILURE);
}

/**
 *
 */
void http::cleanUp() {
    if(_requestHeaders != NULL){
        curl_slist_free_all(_requestHeaders);
    }
    if(_responseCookie != NULL){
        curl_slist_free_all(_responseCookie);
    }
    _title.clear();
    _responseBody.clear();
    _responseCode = 0;
    _responseHeader.clear();
}
