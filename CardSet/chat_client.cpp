#include "chat_client.h"



Chat_Client::Chat_Client(boost::asio::io_service& ioService)
    :m_sock(ioService, udp::endpoint(udp::v4(), 0))
{
    Server_ep = new udp::endpoint(boost::asio::ip::address::from_string("10.253.112.169"),6889);
}

void Chat_Client::doSend(std::string sendMes)
{
    sbufClear();
    for(unsigned int i = 0;i != 1024;i++) {
        if(i < sendMes.size()) m_sbuf[i] = sendMes[i];
        else m_sbuf[i] = '\0';
    }

    m_sock.send_to(boost::asio::buffer(m_sbuf), *Server_ep);

}

void Chat_Client::doReceive(std::string &receiveMes)
{
    rbufClear();
    size_t readSize = 0;
    while(readSize == 0) {
        readSize = m_sock.receive_from(boost::asio::buffer(m_rbuf), *Server_ep);
    }
    std::string s(m_rbuf);
    receiveMes = s;
}

void Chat_Client::sbufClear()
{
    for(int i = 0;i != 1024;i++) {
        m_sbuf[i] = '\0';
    }
}
void Chat_Client::rbufClear()
{
    for(int i = 0;i != 1024;i++) {
        m_rbuf[i] = '\0';
    }
}
