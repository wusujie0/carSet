#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool initDB(std::string host, std::string user, std::string pwd, std::string db_name);
    void closeDB(void);
    std::string SelectData(std::string sql);
    bool InsertData(std::string sql);
    bool UpdateData(std::string sql);
    bool DeleteData(std::string sql);
    void Create(std::string sql);
private:
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
};

#endif // DB_H
