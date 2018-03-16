//
// Created by payloads on 18-3-6.
//

#ifndef NEWPRO_GLOBAL_H
#define NEWPRO_GLOBAL_H
typedef struct shadowPls{
    unsigned short int id;
    unsigned short int  rq;
    unsigned short int  type;
    char  authcode[33];
    char  target[100];
    char  comment[100];
    char  jobname[100];
    char  username[100];
}Pls;

typedef struct ConfigPls{
    unsigned short int dbPort;
    std::string dbHost;
    std::string dbUser;
    std::string dbPass;
    std::string dbName;
    std::string dnsServer;
    int threads;
    unsigned short int listenPort;
    unsigned short int clientMaxNum;
    std::string sqlDebug;
}PlsConfig;

enum CURL_ERROR{
    CURL_REQUEST_ERROR,
    CURL_URL_ERROR,
    CURL_OK
};
#endif //NEWPRO_GLOBAL_H
