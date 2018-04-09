//
// Created by payloads on 18-3-6.
//

#ifndef NEWPRO_SERVER_H
#define NEWPRO_SERVER_H
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "GLOBAL.h"
#include <signal.h>
#include <wait.h>
#include <vector>
#include <fstream>
#include <cstring>
#include "MysqlClass.h"
#include "dns.h"
#include <cctype>
#include "Mystr.h"
#include "MyportScan.h"
#include "http.h"
class ServerClass {
public:
    ServerClass();
    bool startServer();
private:
    static void _signalHandler(int sig);
    bool _runDnsThread();
    std::string _readConfig(std::string fileName,std::string key);
    bool _loadSubDomain();
    bool _loadConfig(std::string fileName);
    bool _setListenPort(unsigned short int port);
    bool _setClientNum(int num);
    bool _authUser();
    bool _sendFail(std::string failStr);
    int _createJob();
    void _getMySQL();
    void _saveResult();
    std::tuple<std::string,std::string,std::string,std::string> _getDomainInfo(std::string host,std::string ip);
private:
    PlsConfig _Config;
    int _jobID;
    int _domainParentID;
    bool _debug;
    MysqlClass _sql;
    socklen_t _clientSize;
    sockaddr_in _serverAddr;
    sockaddr_in _clientAddr;
    int _clientMaxNum;
    int _serverSock;
    int _clientSock;
    int _listenPort;
    pid_t _clientPID;
    char _fromClientBUFF[520];
    Pls * _clientData;
    std::vector<uint16_t >_portList;
    std::vector<std::string> _dnsSubDomains;
    std::vector<std::tuple<int,std::string,std::string>> _dnsResult;
};


#endif //NEWPRO_SERVER_H
