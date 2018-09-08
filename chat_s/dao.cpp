#include "dao.h"

DAO::DAO()
{
    db.initDB("localhost", "root", "chakra", "cardset");
    create_tables();
}

void DAO::create_tables()
{
    std::string t1 = "CREATE TABLE IF NOT EXISTS account(username VARCHAR(20),password VARCHAR(20))";
    db.Create(t1);
}

bool DAO::user_register(std::string username , std::string passwd)
{
    std::string sql = "INSERT INTO account(username,password) VALUES('"+username+"','"+passwd+"')";
    std::cout << sql << std::endl;
    if  (db.InsertData(sql)) {
        std::string t1 = "CREATE TABLE IF NOT EXISTS "+username+"_friends(friendname VARCHAR(20),chatlognum INTEGER )";
        std::string t2 = "CREATE TABLE IF NOT EXISTS "+username+"_chat_log(sendername VARCHAR(20),receivername VARCHAR(20),message TEXT)";
        db.Create(t1);
        db.Create(t2);
        return true;
    } else {
        std::cout << "register error" << std::endl;
        return false;
    }
}


bool DAO::user_login(std::string username , std::string passwd)
{
    std::string sql = "SELECT username FROM account WHERE username = '"+username+"'";
    std::string sql1 = "SELECT password FROM account WHERE username = '"+username+"'";



    std::string s_name = db.SelectData(sql);
    std::string s_word = db.SelectData(sql1);
    std::string s_username;
    std::string s_passwd;
    std::istringstream r(s_name);
    r >> s_username;
    std::istringstream r1(s_word);
    r1 >> s_passwd;

    if (username == s_username) {
        std::cout << "username suc" << std::endl;

        if (passwd == s_passwd) {
            std::cout << "passwd suc" << std::endl;
            return true;
        }
        else {
            std::cout << "passwd not exist" << std::endl;
            return false;
        }
    }
    else {
        std::cout << "username not exist" << std::endl;
        return false;
    }

}

bool DAO::handle_send_message(std::string sender, std::string receiver,std::string message)
{
    std::string i1 = "INSERT INTO "+sender+"_chat_log(sendername,receivername,message) VALUES('"+sender+"','"+receiver+"','"+message+"')";
    std::string i2 = "INSERT INTO "+receiver+"_chat_log(sendername,receivername,message) VALUES('"+sender+"','"+receiver+"','"+message+"')";
    std::string u1 = "UPDATE "+receiver+"_friends SET chatlognum = chatlognum + 1 WHERE friendname = '"+sender+"'";
    if (db.InsertData(i1) && db.InsertData(i2) && db.UpdateData(u1))
        return true;
    else {
        return false;
    }


}
std::string DAO::handle_loginsuc(std::string username)
{
    std::string s1 = "SELECT * FROM "+username+"_friends";
    return db.SelectData(s1);

}


std::string DAO::handle_receive_chatlog(std::string username, std::string friendname, std::string way)
{
    std::string s1 = "SELECT * FROM "+username+"_chat_log WHERE sendername = '"+friendname+"' OR receivername = '"+friendname+"'";
    if (way == "on") {
        std::string u1 = "UPDATE "+username+"_friends SET chatlognum = 0 WHERE friendname = '"+friendname+"'";
        if (db.UpdateData(u1))
        {

        }
    }
    return db.SelectData(s1);

}

