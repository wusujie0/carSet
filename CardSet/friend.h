#ifndef FRIEND_H
#define FRIEND_H


#include <iostream>


class chat_friend
{
public:
    chat_friend(std::string name, int num);
    void getFriendname(std::string &name);
    void setFriendUnreadMesNum(int num);
    int getFriendUnreadMesNum();
    void setFriendChatLog(std::string chatlog);
    void getFriendChatLog(std::string &chatlog);
    void addNewMes(std::string sender, std::string receiver, std::string message);
private:
    std::string friendname;
    int unread_mes_num;
    std::string chat_log;
};

#endif // FRIEND_H
