//
// Created by payloads on 18-3-6.
//

#ifndef NEWPRO_MYSQL_H
#define NEWPRO_MYSQL_H

#include <mysql/mysql.h>
#include <iostream>
#include <vector>
class MysqlClass {
public:
    bool debug=false;
    bool connect(std::string host,std::string user,std::string pass,std::string database,short int port);
    MYSQL_RES * select(std::string sql);
    bool update(std::string sql);
    std::string escapeString(std::string str);
    my_ulonglong insert(std::string tableName,std::vector<std::string>columns,std::vector<std::string>data);
    MYSQL_RES * exec(std::string sql);
    void close();
private:
    MYSQL * _mysqlPoint;
    ~MysqlClass();
};


#endif //NEWPRO_MYSQL_H
