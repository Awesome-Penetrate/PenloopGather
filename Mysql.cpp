//
// Created by payloads on 18-3-6.
//

#include "Mysql.h"
bool Mysql::connect(std::string host, std::string user, std::string pass, std::string database, short int port) {
    sqler = mysql_init(NULL);
    if(!mysql_real_connect(sqler,host.c_str(),user.c_str(),pass.c_str(),database.c_str(),port,NULL,0)){
        std::cout << mysql_error(sqler)<< std::endl;
        mysql_close(sqler);
        exit(-1);
    }
    Type = fields;
    return true;
}

void Mysql::printVersionTest() {

}

int Mysql::real_Insert(std::string tableName, std::vector<std::string> columns, std::vector<std::string> data) {
    int code = 0;
    if(columns.size() != data.size() && mysql_debug){
        std::cout << "column count number not eq data number" << std::endl;
        return -1;
    }
    std::string sql = "INSERT INTO "+tableName;
    for(int j = 0; j < columns.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (columns.size()-1)){
            sql += columns[j]+")";
            break;
        }
        sql+=columns[j]+",";
    }
    sql+="VALUES";
    for(int j = 0; j < data.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (data.size()-1)){
            sql += data[j]+")";
            break;
        }
        sql+= data[j]+",";
    }
    if((code =mysql_query(sqler,sql.c_str())) != 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    if(mysql_debug){
        std::cout <<"[+]SQL : " <<sql << std::endl;
        std::cout << "Insert ID:" << mysql_insert_id(sqler) << std::endl;
    }
    return code;
}
my_ulonglong Mysql::Insert(std::string tableName, std::vector<std::string> columns, std::vector<std::string> data) {
    int code = 0;
    if(columns.size() != data.size() && mysql_debug){
        std::cout << "column count number not eq data number" << std::endl;
        return -1;
    }
    std::string sql = "INSERT INTO "+tableName;
    for(int j = 0; j < columns.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (columns.size()-1)){
            sql += Escape(columns[j])+")";
            break;
        }
        sql+=Escape(columns[j])+",";
    }
    sql+="VALUES";

    for(int j = 0; j < data.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (data.size()-1)){
            sql += "'"+Escape(data[j])+"')";
            break;
        }
        sql+= "'"+Escape(data[j])+"',";
    }
    if((code =mysql_query(sqler,sql.c_str())) != 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    if(mysql_debug){
        std::cout << "Insert ID:" << mysql_insert_id(sqler) << std::endl;
        std::cout <<"[+]SQL : " <<sql << std::endl;
    }
    return mysql_insert_id(sqler);
}

int Mysql::exec(std::string sql) {
    int code = 0;
    if((code =mysql_query(sqler,sql.c_str())) != 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    return code;
}

MYSQL_RES* Mysql::Select(std::string sql) {
    return Query(sql);
}

MYSQL_RES* Mysql::Query(std::string sql) {
    if(mysql_query(sqler,sql.c_str()) != 0 && mysql_debug){
        std::cout << "QUERY : " << sql  << std::endl;
        std::cout << mysql_error(sqler)<< std::endl;
        return NULL;
    }
    Res = mysql_use_result(sqler);
    return Res;
}

int Mysql::Update(std::string sql) {
    return exec(sql);
}

void Mysql::ShowData() {
    switch (Type){
        case fields:
            for(int i=0;mysql_field=mysql_fetch_field(Res);i++)//获取列名
            {
                std::cout << mysql_field->name << " \t ";
            }
            std::cout << "\n";
        default:
            while ((mysqlRow = mysql_fetch_row(Res)) != NULL)
            {
                for(int i=0;i<mysql_num_fields(Res);i++){
                    std::cout << mysqlRow[i] << " \t";
                }
                std::cout << "\n";
            }
    }
}

std::string Mysql::Escape(std::string str) {
    char resultSQL[(str.length()*2)+1];
    mysql_real_escape_string(sqler,resultSQL,str.c_str(),str.length());
    std::string realsql = resultSQL;
    return realsql;
}

void Mysql::close(){
    mysql_free_result(Res);
    mysql_close(sqler);
}