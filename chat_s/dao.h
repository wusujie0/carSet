#ifndef DAO_H
#define DAO_H

#include <iostream>
#include <string>
#include <sstream>
#include "db.h"

class DAO
{
public:
    DAO();

    void create_tables();

    bool user_register(std::string username , std::string passwd);
    bool user_login(std::string username , std::string passwd);
    bool handle_send_message(std::string sender, std::string receiver,std::string message);
    std::string handle_loginsuc(std::string username);
    std::string handle_receive_chatlog(std::string username, std::string friendname, std::string way);
private:
    MyDB db;
};

#endif // DAO_H
