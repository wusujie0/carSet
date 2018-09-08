#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>

#include "dao.h"

//使用UDP命名空间
using boost::asio::ip::udp;

struct user
{
    std::string username;
    std::string IPaddress;
    int port;
};

//
//创建一个异步UDP的服务器。
//
class Chat_Server
{
public:
    Chat_Server(boost::asio::io_service& ioService);
private:
    void RecvQuest(void);
    void handleRecvQuest(const boost::system::error_code& error, std::size_t /*bytes_transferred*/);
    void handleSend(const boost::system::error_code& /*error*/,
            std::size_t /*bytes_transferred*/);

private:
    udp::socket m_sock; //服务器的SOCKET。
    udp::endpoint m_endpointRemote; //收到数据时的端点信息。
    std::vector<user> users;
    char m_rbuf[1024]; //接收数据缓冲区。
    char m_sbuf[1024]; //发送数据缓冲区
    DAO dao;

};


#endif // CHAT_SERVER_H
