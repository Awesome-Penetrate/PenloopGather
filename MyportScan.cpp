#include "MyportScan.h"
#include "Mythread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
void MyportScan::_scanPort(void * Port) {
    MyportScan * Scan = (MyportScan *)Port;
    static auto _it = Scan->_portList.cbegin();
    while(_it != Scan->_portList.cend()){
        Scan->_portScanMutex.lock();
        if(_it >= Scan->_portList.cend() || *_it == 0){
            Scan->_portScanMutex.unlock();
            break;
        }
        int port = *_it;
        ++_it;
        Scan->_sock = socket(AF_INET,SOCK_STREAM,0);
        struct timeval timeout = {1,0};
        setsockopt(Scan->_sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
        setsockopt(Scan->_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
        sockaddr_in serverAddr;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(Scan->_hostName.c_str());
        serverAddr.sin_family = AF_INET;
        if(connect(Scan->_sock,(sockaddr *)&serverAddr, sizeof(serverAddr)) == 0){
            Scan->_openList.push_back(port);
        }
        close(Scan->_sock);
        Scan->_portScanMutex.unlock();
    }
}


bool MyportScan::setHost(std::string hostname) {
    _hostName = hostname;
    std::cout << "[*] PortScan set hostname is " << hostname << std::endl;
}

bool MyportScan::addPort(int port) {
    _portList.push_back(port);
}

bool MyportScan::runThread(int threadNum) {
    Mythread th;
    th.setThreads(threadNum);
    th.runThreads(this->_scanPort,(void *)this);
    th.setJoin();
}

bool MyportScan::isOpen(int port) {
    int sock = socket(AF_INET,PF_INET,IPPROTO_TCP);
    struct timeval timeout = {2,0};
    setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
    sockaddr_in serverAddr;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(_hostName.c_str());
    serverAddr.sin_family = AF_INET;
    if(connect(sock,(sockaddr *)&serverAddr, sizeof(serverAddr)) == 0){
        close(sock);
        return true;
    }else{
        return false;
    }
}


std::vector<uint16_t> MyportScan::getOpenList() {
    return _openList;
}


MyportScan::MyportScan() {

}
