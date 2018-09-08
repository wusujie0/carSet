#ifndef CARDSERVER_H
#define CARDSERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "database.h"
#include "./json/json.h"
#include "card.h"


class CardServer
{
public:

    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    typedef boost::shared_ptr<CardServer> cardServer_ptr;
    typedef std::vector<cardServer_ptr> arrays;

    CardServer();

    void start();
    void handle_start(const boost::system::error_code &err,socket_ptr sock);
    void run();
    void stop();

    void do_read(socket_ptr sock);
    void on_read(const boost::system::error_code &err,socket_ptr sock);
    void read_handle(const boost::system::error_code &err);
    size_t read_complete(char *buf,const error_code &err);

    void do_write(socket_ptr sock,std::string &msg);
    void on_write(const boost::system::error_code &err,socket_ptr sock);
    void write_handle(const boost::system::error_code &err);


    //    boost::asio::ip::tcp::socket &sock(){return m_sock;}
    boost::asio::ip::tcp::acceptor &acc(){return m_acc;}

    void on_create_card(const std::string &msg,socket_ptr sock);
    void on_receive_picture(const boost::system::error_code &err,CardServer::socket_ptr sock,const Card &card);

    void on_search_card(CardServer::socket_ptr sock);                               //搜索所有用户的卡片
    void on_search_card_2(const boost::system::error_code &err,CardServer::socket_ptr sock,const std::string card);
    void on_search_card_handler(const boost::system::error_code &err);

    void on_delete_card(const boost::system::error_code &err,socket_ptr sock);
    void on_delete_card_handle(const boost::system::error_code &err,socket_ptr sock);
    void on_delete_card_handle_2(const boost::system::error_code &err,socket_ptr sock,const std::string pic);

    void on_delete_collect_card(const boost::system::error_code &err,socket_ptr sock);
    void on_delete_collect_card_handle(const boost::system::error_code &err,socket_ptr sock);
    void on_delete_collect_card_handle_2(const boost::system::error_code &err,const std::string pic);

    void on_collect_card(const boost::system::error_code &err,socket_ptr sock);
    void on_collect_card_handle(const boost::system::error_code &err,const std::string cardName);

    void on_create_cardBag(socket_ptr sock);

    void send_picture(const boost::system::error_code &err,const std::string pictureName,CardServer::socket_ptr sock);
    void send_picture_handler(const boost::system::error_code &err);

    void on_fetch_user_card(const boost::system::error_code &err,socket_ptr sock);
    void on_fetch_user_card_handle(const boost::system::error_code &err,socket_ptr sock);

    void on_fetch_user_collect_card(const boost::system::error_code &err,socket_ptr sock);
    void on_fetch_user_collect_card_handle(const boost::system::error_code &err,socket_ptr sock);

    void send_picture_fetch(const boost::system::error_code &err,const std::string pictureName,CardServer::socket_ptr sock);
    void send_card_fetch(const boost::system::error_code &err, const std::string card, CardServer::socket_ptr sock);

private:
    //    boost::asio::ip::tcp::socket m_sock;
    boost::asio::io_service m_ser;
    boost::asio::ip::tcp::acceptor m_acc;
    enum {max_msg = 1024};
    enum {max_pic = 1024 * 1024};
    char read_buffer[max_msg];
    char write_buffer[max_msg];
    char pic_buf[max_pic];
    bool started;
    arrays m_clients;
    DataBase m_db;
};

#endif // CARDSERVER_H
