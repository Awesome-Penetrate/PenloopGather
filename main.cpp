#include "ServerClass.h"
#include "HttpClass.h"
int main(int argc,char * argv[]) {


    ServerClass MyServer;
    MyServer.startServer();

    /*
     * HttpClass Http;
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
    portScan.setHost("192.168.3.7");
    portScan.addPort(135);
    portScan.addPort(139);
    portScan.addPort(139);
    portScan.addPort(139);
    portScan.addPort(445);
    portScan.addPort(445);
    portScan.addPort(445);
    portScan.addPort(2869);
    portScan.addPort(8080);
    portScan.addPort(8080);
    portScan.addPort(8080);
    portScan.addPort(8080);
    portScan.addPort(8080);
    portScan.addPort(8080);
    portScan.addPort(3389);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(6002);
    portScan.addPort(7001);
    portScan.addPort(7002);
    portScan.addPort(11111);
    portScan.addPort(11111);
    portScan.addPort(11111);
    portScan.addPort(11111);
    portScan.addPort(49152);
    portScan.addPort(49153);
    portScan.addPort(49153);
    portScan.addPort(49153);
    portScan.addPort(49153);
    portScan.addPort(49154);
    portScan.addPort(49159);
    portScan.addPort(80);
    portScan.addPort(80);
    portScan.addPort(80);
    portScan.runThread(10);
    */

}
