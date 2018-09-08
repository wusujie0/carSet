#include "friend.h"

chat_friend::chat_friend(std::string name, int num)
    :friendname(name), unread_mes_num(num)
{

}

void chat_friend::getFriendname(std::string &name)
{
    name = friendname;
}
void chat_friend::setFriendUnreadMesNum(int num)
{
    unread_mes_num = num;

}

int chat_friend::getFriendUnreadMesNum()
{
    return unread_mes_num;
}

void chat_friend::setFriendChatLog(std::string chatlog)
{
    chat_log = chatlog;
}

void chat_friend::getFriendChatLog(std::string &chatlog)
{
    chatlog = chat_log;
}

void chat_friend::addNewMes(std::string sender, std::string receiver, std::string message)
{
    unread_mes_num += 1;
    chat_log += sender+" "+receiver+" "+message+" ||| ";

}
