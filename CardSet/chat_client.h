#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>

using boost::asio::ip::udp;

class Chat_Client
{
public:
    Chat_Client(boost::asio::io_service& ioService);
    void doSend(std::string sendMes);
    void doReceive(std::string &receiveMes);
    void rbufClear();//清空read_str
    void sbufClear();//清空write_str
private:
    udp::socket m_sock; //SOCKET。
    udp::endpoint *Server_ep; //send数据的端点信息。
    char m_rbuf[1024]; //接收数据缓冲区。
    char m_sbuf[1024]; //发送数据缓冲区
    std::string username;
};


#endif // CHAT_CLIENT_H
