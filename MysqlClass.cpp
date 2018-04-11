//
// Created by payloads on 18-3-6.
//

#include "MysqlClass.h"

/*
  MYSQL *	STDCALL mysql_real_connect(
                       MYSQL *mysql,
                       const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag
					   );

 * Connect to MySQL
 * @param host
 * @param user
 * @param pass
 * @param database
 * @param port
 * @return
 */
bool MysqlClass::connect(std::string host, std::string user, std::string pass, std::string database, unsigned short int port) {
    _mysqlPoint= mysql_init(NULL);
    _mysqlPoint = mysql_real_connect(_mysqlPoint,host.c_str(),user.c_str(),pass.c_str(),database.c_str(),0,NULL,0);
    if(_mysqlPoint){
        mysql_set_character_set(_mysqlPoint,"utf8");
        return true;
    }
    std::cout << mysql_error(_mysqlPoint) << std::endl;
    return false;
}


/**
 *
 * @param sql
 * @return
 */
int MysqlClass::select(std::string sql) {
    printf("[*] SQL :  %s \n",sql.c_str());
    return mysql_query(_mysqlPoint,sql.c_str());
}

MYSQL_RES * MysqlClass::exec(std::string sql) {
    if(mysql_query(_mysqlPoint,sql.c_str())){
        //mysql_commit(_mysqlPoint);
        return mysql_store_result(_mysqlPoint);
    }else{
        if(debug)
            std::cout << mysql_error(_mysqlPoint) << std::endl;
        return NULL;
    }
}


my_ulonglong MysqlClass::insert(std::string tableName, std::vector<std::string> columns,
                                std::vector<std::string> data) {
    if(columns.size() != data.size() && mysql_debug){
        std::cout << "[!] Column count number not eq data number" << std::endl;
        return 0;
    }
    std::string sql = "INSERT INTO "+tableName;
    for(int j = 0; j < columns.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (columns.size()-1)){
            sql += escapeString(columns[j])+")";
            break;
        }
        sql+=escapeString(columns[j])+",";
    }
    sql+="VALUES";

    for(int j = 0; j < data.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (data.size()-1)){
            sql += "'"+escapeString(data[j])+"')";
            break;
        }
        sql+= "'"+escapeString(data[j])+"',";
    }
    if(mysql_query(_mysqlPoint,sql.c_str()) != 0 && debug){
        std::cout << "[Error] :" << mysql_error(_mysqlPoint) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return 0;
    }

    //mysql_commit(_mysqlPoint);
    std::cout << "[+] Insert ID:" << mysql_insert_id(_mysqlPoint) << std::endl;
    std::cout <<"[+] SQL : " <<sql << std::endl;
    return mysql_insert_id(_mysqlPoint);
}


std::string MysqlClass::escapeString(std::string str) {
    char resultSQL[(str.length()*2)+1];
    mysql_real_escape_string(_mysqlPoint,resultSQL,str.c_str(),str.length());
    std::string realsql = resultSQL;
    return realsql;
}


void MysqlClass::close() {
    mysql_close(_mysqlPoint);
}

MysqlClass::~MysqlClass() {
    this->close();
}