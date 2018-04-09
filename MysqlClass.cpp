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
bool MysqlClass::connect(std::string host, std::string user, std::string pass, std::string database, short int port) {
    _mysqlPoint = NULL;
    if(!mysql_real_connect(_mysqlPoint,host.c_str(),user.c_str(),pass.c_str(),database.c_str(),port,NULL,1)){
        perror(mysql_error(_mysqlPoint));
        return false;
    }
    return true;
}


/**
 *
 * @param sql
 * @return
 */
MYSQL_RES* MysqlClass::select(std::string sql) {
    return exec(sql);
}


/**
 *
 * @param sql
 * @return
 */
MYSQL_RES* MysqlClass::exec(std::string sql) {
    if(!mysql_query(_mysqlPoint,sql.c_str())){
        if(debug)
            perror(mysql_error(_mysqlPoint));
        return NULL;
    }
    mysql_commit(_mysqlPoint);
    return mysql_store_result(_mysqlPoint);
}


my_ulonglong MysqlClass::insert(std::string tableName, std::vector<std::string> columns,
                                std::vector<std::string> data) {
    int code = 0;

    if(columns.size() != data.size() && mysql_debug){
        std::cout << "column count number not eq data number" << std::endl;
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
    if((code = mysql_query(_mysqlPoint,sql.c_str())) != 0 && debug){
        std::cout << "[Error] :" << mysql_error(_mysqlPoint) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return 0;
    }

    mysql_commit(_mysqlPoint);

    if(debug){
        std::cout << "Insert ID:" << mysql_insert_id(_mysqlPoint) << std::endl;
        std::cout <<"[+]SQL : " <<sql << std::endl;
    }
    return mysql_insert_id(_mysqlPoint);
}


std::string MysqlClass::escapeString(std::string str) {
    char resultSQL[(str.length()*2)+1];
    mysql_real_escape_string(_mysqlPoint,resultSQL,str.c_str(),str.length());
    std::string realsql = resultSQL;
    return realsql;
}

bool MysqlClass::update(std::string sql) {
    if(exec(sql) == NULL){
        perror("Update error ");
        return false;
    }
    return true;
}

void MysqlClass::close() {
    mysql_close(_mysqlPoint);
}

MysqlClass::~MysqlClass() {
    this->close();
}