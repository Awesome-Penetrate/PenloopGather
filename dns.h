//
// Created by payloads on 18-2-27.
//

#ifndef NEWPRO_DNS_H
#define NEWPRO_DNS_H

#include <cstring>
#include <sys/socket.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include "Mythread.h"
#include <mutex>
extern std::mutex mu;
#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server
struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

class dns {

public:
    std::string dnsServer;
    std::vector<std::string> _subdomain = {};
public:
    dns();
    dns(std::string dnsServer);
    ~dns();
    void setHostname(std::string hostname);
    int  setDnsServer(std::string  dnsServer);
    static void threadRequest(void * hostname);
    void runThread(int threadNum);
    bool addSubdomain(std::string subdomainName);
    std::vector<std::tuple<int,std::string,std::string>> getResult();
private:
    void request(std::string hostname);
    void setDnsHeader(struct DNS_HEADER * dns);
    void setQname(unsigned char * hostname);
    void setQinfo(struct QUESTION *qinfo);
    void requestDnsServer(struct DNS_HEADER *dns);
    u_char* ReadName(unsigned char* reader,unsigned char* buffer,int* count);
    void changeToDnsNameFormat(unsigned char* dns,unsigned char* host);
private:
    int dnsSock;
    std::string _dnsServer;
    unsigned char _hostname[100];
    unsigned char _buff[65535];
    unsigned char * _qname;
    unsigned char * _reader;
    struct RES_RECORD _answers[20],_auth[20],_addit[20];
    std::vector<std::tuple<int,std::string,std::string>> _aResult;
    sockaddr_in _dest,_a;
    std::mutex _mutex;
    std::string _currentHostname;
};


#endif //NEWPRO_DNS_H
