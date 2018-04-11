//
// Created by payloads on 18-3-6.
//

#include "ServerClass.h"

ServerClass::ServerClass() {
    memset(_fromClientBUFF,0,0);
    _clientData = NULL;
    _loadConfig("./pls.conf");
    _setListenPort(_Config.listenPort);
    _setClientNum(_Config.clientMaxNum);

}

bool ServerClass::_setListenPort(unsigned short int port) {
    _listenPort = port;
}

bool ServerClass::_setClientNum(int num) {
    _clientMaxNum = num;
}


bool ServerClass::startServer() {
    signal(SIGCHLD, _signalHandler);
    pid_t pid=fork(); // 创建子进程
    if(pid==0){  // 子进程处理开始
        // 创建SOCKET描述符
        _serverSock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        // 设置超时
        struct linger timeWit;
        timeWit.l_onoff = 0;
        timeWit.l_linger = 2;
        setsockopt(_serverSock,SOL_SOCKET,SO_LINGER,&timeWit, sizeof(timeWit));

        // 地址重用
        int reuse = 1;
        setsockopt(_serverSock,SOL_SOCKET,SO_REUSEADDR,&reuse, sizeof(reuse));

        _serverAddr.sin_family = AF_INET;
        _serverAddr.sin_addr.s_addr = INADDR_ANY;
        // 绑定端口
        _serverAddr.sin_port = htons((uint16_t)_listenPort);
        if(bind(_serverSock,(struct sockaddr *)&_serverAddr, sizeof(_serverAddr))== 0){
            std::cout << "[*] Server bind Success ..." << std::endl;
        }else{
            std::cout << "[!] Server bind Fail ..." << std::endl;
            exit(0);
        }
        _clientSize = sizeof(_clientAddr);
        while(true){
            ssize_t ret = recvfrom(_serverSock,_fromClientBUFF, sizeof(_fromClientBUFF),0,(struct sockaddr *)&_clientAddr,&_clientSize);
            // 接收客户端请求
            if(ret >0 ){
                _clientPID = fork();
                if(_clientPID == 0){
                    close(_serverSock);
                    _getMySQL();
                    _loadSubDomain();
                    _clientData = (Pls * )_fromClientBUFF;   // 格式化
                    std::cout << "[*] Username :" << _clientData->username << std::endl;

                    //_authUser();  // 验证权限
                    _createJob(); // 创建任务
                    _runDnsThread(); // 执行任务
                    //

                    for (auto it = _dnsResult.begin(); it != _dnsResult.end(); it++) {
                        std::cout <<"[" << std::get<0>(*it) << "] "<< std::get<1>(*it) << " => "<< std::get<2>(*it) << std::endl;
                    }

                    std::cout << "[*] Job " << _jobID << " done .\n" << std::endl;

                    shutdown(_clientSock,SHUT_RDWR);
                    _sql.close();
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }else{
        int status;
        std::cout << "[*] Server is running ..." << std::endl;
        waitpid(pid,&status,0);
    }

            /*
            if((_clientSock = accept(_serverSock,(struct sockaddr *)&_clientAddr,&_clientSize))!=-1){
                _clientPID = fork();
                if(_clientPID == 0){

                    close(_serverSock); // 关闭服务端Socket

                    _loadSubDomain();   // 加载字典

                    recv(_clientSock,_fromClientBUFF,500,0); // 接收数据

                    _clientData = (Pls * )_fromClientBUFF;   // 格式化

                    std::cout << "[*]Username :" << _clientData->username << std::endl;

                    _authUser();  // 验证权限
                    _createJob(); // 创建任务
                    _runDnsThread(); // 执行任务
                    //
                    for (auto it = _dnsResult.begin(); it != _dnsResult.end(); it++) {
                        std::cout <<"[" << std::get<0>(*it) << "] "<< std::get<1>(*it) << " => "<< std::get<2>(*it) << std::endl;
                    }

                    shutdown(_clientSock,SHUT_RDWR);
                    exit(0);
                }
            }
        }
    }else{
        int status;
        std::cout << "[*]Server is running ..." << std::endl;
        waitpid(pid,&status,0);
    }
 */
}

/**
 * 信号处理，用于收割僵尸进程
 * @param sig
 */
void ServerClass::_signalHandler(int sig) {
    pid_t pid;
    pid=wait(NULL);
}

/**
 * 多线程运行
 * @return
 */
bool ServerClass::_runDnsThread() {
    dns dnsRequest;
    dnsRequest.setDnsServer(_Config.dnsServer);
    dnsRequest.setHostname(_clientData->target);
    dnsRequest._subdomain.swap(_dnsSubDomains);
    dnsRequest.runThread(_Config.threads);
    _dnsResult = dnsRequest.getResult();
    std::vector<std::tuple<int,std::string,std::string>>::iterator it;

    for (it = _dnsResult.begin(); it != _dnsResult.end(); it++) {
        std::cout << "[*] DNS :" << std::get<1>(*it) << "  " << std::get<2>(*it) << std::endl;
        // auto DomainInfo = _getDomainInfo(std::get<1>(*it),std::get<2>(*it));
        auto DomainInfo = _copyThread(std::get<1>(*it),std::get<2>(*it));

        //CALL sp_add_domain ( "hao123.com", "80,55,44", "header", "title", "80.12.14.14", 0, 1 );--
        //std::cout << "[*] Host : " << std::get<1>(*it) << " " << std::endl;

        std::stringstream callQueryStrem;

        char callSpAddDomain[5000];
        callQueryStrem << "CALL sp_add_domain('"
                       << std::get<1>(*it)
                       <<"','"
                       << std::get<1>(DomainInfo)
                       <<"','"
                       <<(_sql.escapeString(std::get<2>(DomainInfo)))
                       <<"','"
                       <<std::get<3>(DomainInfo)
                       <<"','"
                       <<std::get<2>(*it)
                       <<"','"
                       <<0
                       <<"','"
                       <<_jobID
                       <<"');";
        sprintf(callSpAddDomain,"CALL sp_add_domain('%s','%s','%s','%s','%s','%d','%d');",
                std::get<1>(*it).c_str(),
                std::get<1>(DomainInfo).c_str(),
                (_sql.escapeString(std::get<2>(DomainInfo))).c_str(),
                std::get<3>(DomainInfo).c_str(),
                std::get<2>(*it).c_str(),
                0,
                _jobID
        );
        //std::cout << "Query : " << callQueryStrem.str() << std::endl;
        //printf("[*] CALL Query : %s \n",callSpAddDomain);
        if(mysql_query(_sql._mysqlPoint,callSpAddDomain)){
            mysql_commit(_sql._mysqlPoint);
        } else{
            perror(mysql_error(_sql._mysqlPoint));
        }

    }
}

/**
 * 加载配置文件
 * @param fileName
 * @return
 */
bool ServerClass::_loadConfig(std::string fileName) {
    _Config.threads = atoi(_readConfig(fileName,"threads").c_str());
    _Config.dbName  = _readConfig(fileName,"dbName");
    _Config.dbUser  = _readConfig(fileName,"dbUser");
    _Config.dbPass  = _readConfig(fileName,"dbPass");
    _Config.dnsServer  = _readConfig(fileName,"dnsServer");
    _Config.dbPort  = atoi(_readConfig(fileName,"dbPort").c_str());
    _Config.listenPort  = atoi(_readConfig(fileName,"listenPort").c_str());
    _Config.clientMaxNum  = atoi(_readConfig(fileName,"clientMaxNum").c_str());
    _Config.sqlDebug  = _readConfig(fileName,"sqlDebug");
    _Config.dbHost  = _readConfig(fileName,"dbHost");
}

/**
 * 读取配置文件
 * @param fileName
 * @param key
 * @return
 */
std::string ServerClass::_readConfig(std::string fileName,std::string key) {
    std::string tmp;
    std::fstream IO(fileName);
    if(!IO.is_open()){
        std::cout << "[!] Config File can't load .." << std::endl;
        exit(EXIT_FAILURE);
    }
    while(!IO.eof())
    {
        getline(IO,tmp);
        if(tmp[0]=='#'){
            continue;
        }
        std::string line(tmp);
        size_t pos = line.find('=');
        if(pos==-1){
            continue;
        }
        std::string tmpKey = line.substr(0,pos);
        if(key==tmpKey)
        {
            IO.close();
            return line.substr(pos+1);
        }
    }
    IO.close();
    return "NULL";
}

/**
 * 加载子域名字典
 * @return
 */
bool ServerClass::_loadSubDomain() {
    std::cout << "[+] Loading subdomain ... " << std::endl;
    _sql.select("SELECT pls_subdomain_name FROM pls_subdomain;");
    MYSQL_RES * Res = mysql_store_result(_sql._mysqlPoint);
    MYSQL_ROW  mysqlRow;
    while ((mysqlRow = mysql_fetch_row(Res)) != NULL)
    {
        for(int i=0;i < mysql_num_fields(Res);i++){
            std::cout << "[*] Subdomain >> " << mysqlRow[i] << std::endl;
            _dnsSubDomains.emplace_back(mysqlRow[i]);
        }
    }

    mysql_free_result(Res);
}

/**
 * 验证是否有派发任务权限
 * @return
 */
bool ServerClass::_authUser() {
    _getMySQL();
    // std::cout << strlen(_clientData->authcode) << std::endl;
    if(strlen(_clientData->authcode) != 32){
        _sendFail("AUTH CODE LENGTH MUST > 32");
    }
    Mystr CheckStr;
    if(!CheckStr._checkStr(_clientData->authcode, sizeof(_clientData->authcode))){
        _sendFail("AUTH CODE ERROR");
    }
    MYSQL_ROW mysqlRow;
    std::string sql = "SELECT if(pls_authcode='";
    sql.append(_clientData->authcode);
    sql.append("',1,2) as auth FROM pls_user");
    _sql.select(sql);
    MYSQL_RES * Res = mysql_store_result(_sql._mysqlPoint);
    mysqlRow = mysql_fetch_row(Res);
    if(mysqlRow[0] == "2"){
        _sendFail("AUTH CODE ERROR");
    }
    _sql.close();
    return true;
}

/**
 * 发送失败数据包
 * @param failStr
 * @return
 */
bool ServerClass::_sendFail(std::string failStr) {
    _clientData->rq = 0;
    strcpy(_clientData->comment,failStr.c_str());
    strcpy(_fromClientBUFF,(char *)_clientData);
    send(_clientSock,_fromClientBUFF, sizeof(_fromClientBUFF),0);
    shutdown(_clientSock,SHUT_RDWR);
    std::cout << "[*] "<<  inet_ntoa(_clientAddr.sin_addr) << ":"<<failStr << std::endl;
    exit(0);
}

// 创建扫描任务
int ServerClass::_createJob() {


    // 设置插入字段
    std::vector<std::string> columns{
             "pls_jobs_name",
             "pls_jobs_des",
             "pls_jobs_user",
             "pls_jobs_target",
    };

    // 设置插入数据
    std::vector<std::string> data{};
    data.emplace_back(_clientData->jobname);
    data.emplace_back(_clientData->comment);
    data.emplace_back(_clientData->username);
    data.emplace_back(_clientData->target);

    //插入任务
    _jobID = (int)_sql.insert("pls_jobs",columns,data);
    std::cout << "[*] CreateJob ID is " << _jobID << std::endl;
}

/* 链接数据库
 *
 * */
void ServerClass::_getMySQL() {
    if(_Config.sqlDebug == "true"){
        _sql.debug = true;
    }
    std::cout << "[+] Connect to MySQL ...." << std::endl;
    if(_sql.connect(_Config.dbHost,_Config.dbUser,_Config.dbPass,_Config.dbName,_Config.dbPort)){
        std::cout << "[*] Connect to MySQL Success ..." << std::endl;
    }else{
        std::cout << "[!] Connect to MySQL Fail ..." << std::endl;
        exit(EXIT_FAILURE);
    }
}


void ServerClass::_saveResult() {
    std::vector<std::string> columns{
            "pls_jobs_name",
            "pls_jobs_des",
            "pls_jobs_user",
            "pls_jobs_target",
    };
    std::vector<std::string> data{};

    for (auto it = _dnsResult.begin(); it != _dnsResult.end(); it++) {
        std::cout <<"[" << std::get<0>(*it) << "] "<< std::get<1>(*it) << " => "<< std::get<2>(*it) << std::endl;
        data.emplace_back(std::get<1>(*it));
        data.emplace_back(std::get<2>(*it));
    }

}

    /**
     * std::string IP地址
     * std::string 开启端口
     * std::string HTTP相应头
     * std::string HTTP标题
     */
std::tuple<std::string,std::string,std::string,std::string> ServerClass::_getDomainInfo(std::string host,std::string ip) {

    std::string _title,_responseHeader,_openPorts;

    MyportScan PortScan;

    PortScan.setHost(ip);

    PortScan.addPort(80);

    PortScan.addPort(8081);

    PortScan.addPort(8080);
    std::cout << "[+]Scan host :" << ip << std::endl;
    PortScan.runThread(_Config.threads);
    std::vector <uint16_t>_openList = PortScan.getOpenList();
    for (int i = 0; i < _openList.size(); ++i) {
        std::cout << "[*] " << host << ":" <<_openList[i] << " [Open] "<< std::endl;
        host.insert(0,"http://");
        host = host.replace(host.rfind('.'),1,"");
        std::cout << "[*]URL :" << host << std::endl;
        http HTTP_REQ(host,_openList[i]);
        HTTP_REQ.getRequest();
        std::cout <<"[*]Title : " <<  HTTP_REQ.getTitle() << std::endl;
        _title = HTTP_REQ.getTitle();
        _responseHeader = HTTP_REQ.getResponse();
        _openPorts.append(std::to_string(_openList[i]));
        _openPorts.append(",");
        std::cout << HTTP_REQ.getResponse() << std::endl;
        break;
    }
    auto _Result = std::make_tuple(host,_openPorts,_responseHeader,_title);
    return _Result;
}


std::tuple<std::string,std::string,std::string,std::string> ServerClass::_copyThread(std::string host,
                                                                                     std::string ip) {
    std::string _title,_responseHeader,_openPorts;

    MyportScan  PortScan;

    PortScan.setHost(ip);

    PortScan.addPort(80);

    PortScan.addPort(8081);

    PortScan.addPort(8080);
    std::cout << "[+] Scan host :" << ip << std::endl;
    PortScan.runThread(_Config.threads);
    std::vector <uint16_t>_openList = PortScan.getOpenList();
    for (int i = 0; i < _openList.size(); ++i) {
        std::cout << "[*] " << host << ":" <<_openList[i] << " [Open] "<< std::endl;
        host = host.replace(host.rfind('.'),1,"");
        std::cout << "[+] Host : " << host << std::endl;
        HttpClass Http;
        Http.setHost(host);
        Http.setPort(_openList[i]);
        Http.setIP(ip);
        Http.request();
        std::cout <<"[*] Title : " <<  Http.title << std::endl;
        _title = Http.title;
        _responseHeader = Http.responseHeader;
        _openPorts.append(std::to_string(_openList[i]));
        _openPorts.append(",");

        break;
    }
    auto _Result = std::make_tuple(host,_openPorts,_responseHeader,_title);
    return _Result;

}