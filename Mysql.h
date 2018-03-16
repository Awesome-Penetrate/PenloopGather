//
// Created by payloads on 18-3-6.
//

#ifndef NEWPRO_MYSQL_H
#define NEWPRO_MYSQL_H

#include <mysql/mysql.h>
#include <iostream>
#include <vector>
enum show_type{
    fields,
    data
};
class Mysql {
public:
    bool mysql_debug=false;
    enum show_type Type;
    bool connect(std::string host,std::string user,std::string pass,std::string database,short int port);
    MYSQL_RES * Select(std::string sql);
    int Update(std::string sql);
    void ShowData();
    std::string Escape(std::string str);
    void close();
    my_ulonglong Insert(std::string tableName,std::vector<std::string>columns,std::vector<std::string>data);
    int real_Insert(std::string tableName,std::vector<std::string>columns,std::vector<std::string>data);
    void printVersionTest();
    int exec(std::string sql);
private:
    MYSQL_RES * Res;
    MYSQL_RES * Query(std::string sql);
    MYSQL * sqler;
    MYSQL_FIELD * mysql_field;
    MYSQL_ROW mysqlRow;
};


#endif //NEWPRO_MYSQL_H
