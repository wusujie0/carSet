#include<iostream>
#include<cstdlib>
#include "db.h"

MyDB::MyDB()
{
    connection = mysql_init(NULL); // 初始化数据库连接变量
    if(connection == NULL)
    {
        std::cout << "Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }
}

MyDB::~MyDB()
{
    if(connection != NULL)  // 关闭数据库连接
    {
        mysql_close(connection);
    }
}

bool MyDB::initDB(std::string host, std::string user, std::string pwd, std::string db_name)
{
    connection = mysql_real_connect(connection, host.c_str(),
            user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
    if(connection == NULL)
    {
        std::cout << "initDB Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }
    return true;
}

void MyDB::closeDB(void)
{
    if(connection != NULL)  // 关闭数据库连接
    {
        mysql_close(connection);
    }

}


std::string MyDB::SelectData(std::string sql)
{
    if(mysql_query(connection, sql.c_str()))
    {
        std::cout << "SelectData Query Error:" << mysql_error(connection) << std::endl;
        return "";
    }
    result = mysql_store_result(connection);

    if(result==NULL)
    {
        std::cout << "SELECT error" << std::endl;
        return "";
    }
    unsigned int fieldcount = mysql_num_fields(result);

    std::string str("");

    while(row = mysql_fetch_row(result))
    {
        for(int i = 0;i < fieldcount; i++)
        {
            str += row[i];
            str += " ";
        }
        str += " ||| ";
    }

    mysql_free_result(result);

    //std::cout << "select ok" << std::endl;
    return str;

}

bool MyDB::InsertData(std::string sql)
{
    if(mysql_query(connection, sql.c_str()))
    {
        std::cout << "InsertData Query Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }
    else {
        //std::cout << "Insert ok" << std::endl;
        return true;
    }

}

bool MyDB::UpdateData(std::string sql)
{
    if(mysql_query(connection, sql.c_str()))
    {
        std::cout << "UpdateData Query Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }
    else {
        //std::cout << "Update ok" << std::endl;
        return true;
    }

}

bool MyDB::DeleteData(std::string sql)
{
    if(mysql_query(connection, sql.c_str()))
    {
        std::cout << "DeleteData Query Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }
    else //std::cout << "Delete ok" << std::endl;
    return true;

}
void MyDB::Create(std::string sql)
{
    if(mysql_query(connection, sql.c_str()))
    {
        std::cout << "CREATE Query Error:" << mysql_error(connection) << std::endl;
        exit(1);
    }

}
