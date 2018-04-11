#include "MyportScan.h"

void MyportScan::_scanPort() {
    auto _it = _portList.begin();
    while(_it != this->_portList.end()){
        //Scan->_portScanMutex.lock();
        int port = *_it;
        ++_it;
        std::cout << "[*] Scan port .. " << port << std::endl;
        //Scan->_portScanMutex.unlock();

        this->_sock = socket(AF_INET,SOCK_STREAM,0);
        struct timeval timeout = {1,0};
        setsockopt(this->_sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
        setsockopt(this->_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
        sockaddr_in serverAddr;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(this->_hostName.c_str());
        serverAddr.sin_family = AF_INET;
        if(connect(this->_sock,(sockaddr *)&serverAddr, sizeof(serverAddr)) == 0){
            this->_portScanMutex.lock();
            std::cout << "[*] Open :" << port << std::endl;
            this->_openList.push_back(port);
        }
        close(this->_sock);
    }
}


bool MyportScan::setHost(std::string hostname) {
    _hostName = hostname;
    std::cout << "[*] PortScan set hostname is " << hostname << std::endl;
}

bool MyportScan::addPort(int port) {
    _portList.emplace_back(port);
}

bool MyportScan::runThread(int threadNum) {
    /*
    Mythread th;
    th.setThreads(threadNum);
    th.runThreads(this->_scanPort,(void *)this);
    th.setJoin();
     */
    this->_scanPort();
}

bool MyportScan::isOpen(int port) {
    int sock = socket(AF_INET,PF_INET,IPPROTO_TCP);
    struct timeval timeout = {2,0};
    setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
    struct sockaddr_in serverAddr;
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
