#include "ServerClass.h"


int main(int argc,char * argv[]) {


    ServerClass MyServer;
    MyServer.startServer();


    /*
    std::fstream ss;

    ss.open("subnames.txt");
    dns dnsRequest;
    dnsRequest.setDnsServer("114.114.114.114");
    dnsRequest.setHostname("baidu.com");
    while(!ss.eof()){
        std::string lin;
        getline(ss,lin);
        dnsRequest._subdomain.emplace_back(lin);
    }

    dnsRequest.runThread(1);
     */
/*
    HttpClass Http;
    Http.setIP("115.182.41.160");
    Http.setHost("ccaa.kaoshi.open.com.cn");
    Http.setPort(80);
    Http.request();
    MyportScan portScan;
    portScan.setHost("192.168.3.7");
    /*
135/tcp   open  msrpc
139/tcp   open  netbios-ssn
445/tcp   open  microsoft-ds
2869/tcp  open  icslap
3389/tcp  open  ms-wbt-server
6002/tcp  open  X11:2
7001/tcp  open  afs3-callback
7002/tcp  open  afs3-prserver
11111/tcp open  vce
49152/tcp open  unknown
49153/tcp open  unknown
49154/tcp open  unknown
49159/tcp open  unknown
     */
/*
    MyportScan portScan;
    portScan.setHost("192.168.30.245");
    portScan.addPort(8081);
    portScan.addPort(80);
    portScan.addPort(3306);
    portScan.addPort(49153);
    portScan.addPort(80);
    portScan.addPort(3306);
    portScan.addPort(80);
    portScan.addPort(80);
    portScan.addPort(80);
    portScan.addPort(3306);
    portScan.addPort(80);
    portScan.addPort(1111);
    portScan.addPort(80);
    portScan.addPort(3306);
    portScan.addPort(445);
    portScan.runThread(10);
/*
    ThreadClass th;
    th.setThreadNum(10);
    th.runThread();
*/
}
