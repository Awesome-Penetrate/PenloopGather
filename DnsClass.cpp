//
// Created by payloads on 18-2-27.
//

#include "DnsClass.h"


dns::dns(){

}

dns::dns(std::string dnsServer){
    setDnsServer(dnsServer);
}

dns::~dns(){
/*
    std::vector<std::tuple<int,std::string,std::string>>::iterator it;

    for (it = _aResult.begin(); it != _aResult.end(); it++) {
        std::cout <<"[" <<std::get<0>(*it) << "] "<< std::get<1>(*it) << " => "<< std::get<2>(*it) << std::endl;
    }
*/
}

/**
 *
 * @param hostname
 */
void dns::request(std::string hostname) {

    strcpy((char *)this->_hostname,hostname.c_str());

    struct DNS_HEADER * dns = NULL;
    struct QUESTION *qinfo = NULL;

    setDnsHeader(dns);
    setQname((unsigned char *)hostname.c_str());
    setQinfo(qinfo);


    requestDnsServer(dns);
    dns = (struct DNS_HEADER*)_buff;



    _reader = &_buff[sizeof(struct DNS_HEADER) + (strlen((const char*)_qname)+1) + sizeof(struct QUESTION)];

    int stop=0;

    for(int i=0;i<ntohs(dns->ans_count);i++)
    {
        _answers[i].name=ReadName(_reader,_buff,&stop);
        _reader = _reader + stop;
        _answers[i].resource = (struct R_DATA*)(_reader);
        _reader = _reader + sizeof(struct R_DATA);
        if(ntohs(_answers[i].resource->type) == 1)
        {
            _answers[i].rdata = (unsigned char*)malloc(ntohs(_answers[i].resource->data_len));

            for(int j=0 ; j<ntohs(_answers[i].resource->data_len) ; j++)
            {
                _answers[i].rdata[j]=_reader[j];
            }

            _answers[i].rdata[ntohs(_answers[i].resource->data_len)] = '\0';

            _reader = _reader + ntohs(_answers[i].resource->data_len);
        }
        else
        {
            _answers[i].rdata = ReadName(_reader,_buff,&stop);
            _reader = _reader + stop;
        }
    }

    printf("\nAnswer Records : %d \n" , ntohs(dns->ans_count) );

    for(int i=0; i < ntohs(dns->ans_count) ; i++)
    {
        //printf("Name : %s ",answers[i].name);

        if( ntohs(_answers[i].resource->type) == T_A) //IPv4 address
        {

            std::string ipAddress;
            long *p;
            p=(long*)_answers[i].rdata;
            _a.sin_addr.s_addr=(*p); //working without ntohl
            //printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
            ipAddress=inet_ntoa(_a.sin_addr);
        }

        if(ntohs(_answers[i].resource->type)==5)
        {
            printf("has alias name : %s",_answers[i].rdata);
        }

    }
    close(dnsSock);

    /*
    auto it = _aResult.begin();
    for (; it != _aResult.end(); ++it) {
        std::cout << "[*]" << hostname << " => " << (*it).second << std::endl;
    }
*/
    for(int i=0; i < ntohs(dns->ans_count) ; i++){
        if(ntohs(_answers[i].resource->type) == 1) //if its an ipv4 address
        {
            free(_answers[i].rdata);
        }
    }
    _aResult.clear();
    this->_hostname[0]='\0';
}

/**
 *
 * @param dns
 * @param host
 */
void dns::changeToDnsNameFormat(unsigned char* dns,unsigned char* host)
{
    int lock = 0 , i;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
            *dns++ = i-lock;
            for(;lock<i;lock++)
            {
                *dns++=host[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}

/**
 *
 * @param dns
 */
void dns::setDnsHeader(struct DNS_HEADER *dns) {
    dns = (struct DNS_HEADER * )&_buff;
    dns->id = (unsigned short) htons(random());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
}

/**
 *
 * @param hostname
 */
void dns::setQname(unsigned char *hostname) {
    _qname = (unsigned char *)&_buff[sizeof(struct DNS_HEADER)];
    changeToDnsNameFormat(_qname,hostname);
}

/**
 *
 * @param qinfo
 */
void dns::setQinfo(struct QUESTION *qinfo) {
    qinfo = (struct QUESTION *)&_buff[sizeof(struct DNS_HEADER)+strlen((const char*)_qname)+1];
    qinfo->qtype= htons(1);
    qinfo->qclass= htons(1);
}


/**
 *
 * @param reader
 * @param buffer
 * @param count
 * @return
 */
u_char * dns::ReadName(unsigned char* reader,unsigned char* buffer,int* count)
{
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;

    *count = 1;
    name = (unsigned char*)malloc(256);

    name[0]='\0';

    //read the names in 3www6google3com format
    while(*reader!=0)
    {
        if(*reader>=192)
        {
            offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
            reader = buffer + offset - 1;
            jumped = 1; //we have jumped to another location so counting wont go up!
        }
        else
        {
            name[p++]=*reader;
        }

        reader = reader+1;

        if(jumped==0)
        {
            *count = *count + 1; //if we havent jumped to another location then we can count up
        }
    }

    name[p]='\0'; //string complete
    if(jumped==1)
    {
        *count = *count + 1; //number of steps we actually moved forward in the packet
    }

    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='\0'; //remove the last dot
    return name;
}

/**
 * set name server
 * @param dnsServer
 * @return
 */
int dns::setDnsServer(std::string  dnsServer) {
    _dnsServer = dnsServer;
    return 0;
}


void dns::requestDnsServer(struct DNS_HEADER * dns) {
    dnsSock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    struct timeval timeout = {1,0};
    setsockopt(dnsSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    _dest.sin_family = AF_INET;
    _dest.sin_port = htons(53);
    _dest.sin_addr.s_addr = inet_addr(_dnsServer.c_str());
    int i = sizeof(_dest);
    int sendNum  = sendto(dnsSock,(char*)_buff,sizeof(struct DNS_HEADER) + (strlen((const char*)_qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&_dest,sizeof(_dest));
    if(sendNum < 0)
    {
        printf("Error %d \n",sendNum);
        return;
    }
    if(recvfrom (dnsSock,(char*)_buff , 65536 , 0 , (struct sockaddr*)&_dest , (socklen_t*)&i ) < 0)
    {
        //perror("recvfrom failed");
        memset(_buff,0,0);
        return;
    }
}


void dns::threadRequest(void * DnsRequest) {

    dns * Dns = this;

    int threadLock = 0;
    while(threadLock < Dns->_subdomain.size()){
        //Dns->_mutex.lock();
        struct DNS_HEADER * dns = NULL;
        struct QUESTION *qinfo = NULL;

        Dns->_currentHostname.append(Dns->_subdomain[threadLock]);
        threadLock++;
        char hostBuff[200];
        printf("[*] Try brute %s ...  -  %ld%% \n",Dns->_currentHostname.c_str(),threadLock/Dns->_currentHostname.size());
        sprintf(hostBuff,"%s.%s",Dns->_currentHostname.c_str(),Dns->_hostname);
        Dns->setDnsHeader(dns);
        Dns->setQname((unsigned char *)hostBuff);
        Dns->setQinfo(qinfo);
        Dns->requestDnsServer(dns);
        dns = (struct DNS_HEADER*)Dns->_buff;
        Dns->_reader = &Dns->_buff[sizeof(struct DNS_HEADER) + (strlen((const char*)Dns->_qname)+1) + sizeof(struct QUESTION)];
        int stop=0;

        for(int i=0;i<ntohs(dns->ans_count);i++)
        {
            Dns->_answers[i].name=Dns->ReadName(Dns->_reader,Dns->_buff,&stop);
            Dns->_reader = Dns->_reader + stop;
            Dns->_answers[i].resource = (struct R_DATA*)(Dns->_reader);
            Dns->_reader = Dns->_reader + sizeof(struct R_DATA);
            if(ntohs(Dns->_answers[i].resource->type) == 1) //if its an ipv4 address
            {
                Dns->_answers[i].rdata = (unsigned char*)malloc(ntohs(Dns->_answers[i].resource->data_len));

                for(int j=0 ; j<ntohs(Dns->_answers[i].resource->data_len) ; j++)
                {
                    Dns->_answers[i].rdata[j]=Dns->_reader[j];
                }

                Dns->_answers[i].rdata[ntohs(Dns->_answers[i].resource->data_len)] = '\0';

                Dns->_reader = Dns->_reader + ntohs(Dns->_answers[i].resource->data_len);
            }
            else
            {
                Dns->_answers[i].rdata = Dns->ReadName(Dns->_reader,Dns->_buff,&stop);
                Dns->_reader = Dns->_reader + stop;
            }
        }
        for(int i=0; i < ntohs(dns->ans_count) ; i++)
        {
            if( ntohs(Dns->_answers[i].resource->type) == T_A) //IPv4 address
            {
                std::string ipAddress;
                long *p;
                p=(long*)Dns->_answers[i].rdata;
                Dns->_a.sin_addr.s_addr=(*p); //working without ntohl
                printf("[*] %s has IPv4 address : %s \n",hostBuff,inet_ntoa(Dns->_a.sin_addr));
                ipAddress=inet_ntoa(Dns->_a.sin_addr);
                Dns->_aResult.emplace_back(std::make_tuple(threadLock,hostBuff,ipAddress));
            }
        }

        close(Dns->dnsSock);



        for(int i=0; i < ntohs(dns->ans_count) ; i++){
            if(ntohs(Dns->_answers[i].resource->type) == 1) //if its an ipv4 address
            {
                free(Dns->_answers[i].rdata);
            }
        }
        //Dns->_aResult.clear();
        memset(hostBuff,0,0);
        Dns->_currentHostname.clear();
        //Dns->_mutex.unlock();
    }
    return;
}


void dns::setHostname(std::string hostname) {
    strcpy((char *)this->_hostname,hostname.c_str());
    return;
}

void dns::runThread(int threadNum) {
    // void * host = (void *)this;
    /*Mythread th;
    th.setThreads(threadNum);

    th.runThreads(this->threadRequest,(void *)this);

    th.setJoin();
     */
    this->threadRequest(this);
    return;
}


bool dns::addSubdomain(std::string subdomainName) {
    _subdomain.push_back(subdomainName);
    return true;
}


std::vector<std::tuple<int,std::string,std::string>> dns::getResult() {
    return _aResult;
}