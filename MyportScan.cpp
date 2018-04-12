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
    serverAddr.sin_addr.s_addr = inet_addr(this->_hostName.c_str());
    std::cout << "[*] PortScan set hostname is " << hostname << std::endl;
}

bool MyportScan::addPort(int port) {
    _portList.emplace_back(port);
}

bool MyportScan::runThread(int threadNum) {
    _threadNum = threadNum;
    this->run();
}

std::vector<uint16_t> MyportScan::getOpenList() {
    return _openList;
}


MyportScan::MyportScan() {
    this->_sock = socket(AF_INET,SOCK_STREAM,0);
    struct timeval timeout = {1,0};
    setsockopt(this->_sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
    setsockopt(this->_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    serverAddr.sin_family = AF_INET;
}


void MyportScan::run() {
    it = _portList.begin();
    for (int i = 0; i < _threadNum; ++i) {

        _threadLists.emplace_back(std::thread([=](){

            while(it != _portList.end()){
                _portScanMutex.lock();
                int port = *it;
                std::cout << "[*] Port :" << port << "\n";
                it ++;
                serverAddr.sin_port = htons(port);
                _portScanMutex.unlock();

                if(connect(this->_sock,(sockaddr *)&serverAddr, sizeof(serverAddr)) == 0){
                    _portScanMutex.lock();
                    std::cout << "[*] Open :" << port << std::endl;
                    _openList.emplace_back(port);
                    _portScanMutex.unlock();
                }
                _portScanMutex.lock();
                close(this->_sock);
                _portScanMutex.unlock();
            }
        }));
    }
    for(auto it = _threadLists.begin();it!=_threadLists.end();it++){
        (*it).join();
    }
}