//
// Created by payloads on 18-3-5.
//
#ifndef NEWPRO_MYPORTSCAN_H
#define NEWPRO_MYPORTSCAN_H
#include <iostream>
#include <vector>
#include <mutex>
#include "Mythread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class MyportScan{

public:
    MyportScan();
    bool setHost(std::string host);
    bool addPort(int port);
    bool runThread(int threadNum);
    std::vector<uint16_t>getOpenList();
public:
    std::vector<uint16_t >_portList;
private:
    bool isOpen(int port);
    void  _scanPort();

private:
    int _sock;
    int _it;
    std::string  _hostName;
    std::mutex _portScanMutex;
    std::vector <uint16_t>_openList;

};


#endif //NEWPRO_MYPORTSCAN_H
