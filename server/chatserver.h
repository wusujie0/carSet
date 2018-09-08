#ifndef CHATSERVER_H
#define CHATSERVER_H

#include<boost/asio.hpp>

typedef boost::system::error_code error_code;

class ChatServer
{
public:
    ChatServer();
    void start();
    void stop();
    void do_read();
    void on_read(const error_code &err,size_t bytes);
    void do_write();
    void on_write(const error_code &err,size_t bytes);
private:
    boost::asio::ip::tcp::socket m_sock;
    enum {max_msg = 1024};
    char read_buffer[max_msg];
    char write_buffer[max_msg];
};

#endif // CHATSERVER_H
