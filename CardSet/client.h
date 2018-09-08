#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <boost/thread.hpp>
#include <QObject>
#include "card.h"

class Account;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
using namespace boost::asio;

class Client
{
public:

    friend class Account;
    typedef boost::system::error_code error_code;
    Client(boost::asio::io_service &service):m_sock(service),started(true){
        searchCards = new std::vector<Card>;
        userCards = new std::vector<Card>;
        collectCards = new std::vector<Card>;

        sSearchCards = new std::vector<std::string>;
        sUserCards = new std::vector<std::string>;
        sCollectCards = new std::vector<std::string>;

                ip::tcp::endpoint ep(ip::address::from_string("10.253.112.169"),2001);
        try{
            m_sock.connect(ep);
            //            loop();

        }
        catch(boost::system::system_error &err){
            std::cerr << "error!" << std::endl;
            return;
        }

    }

    void connect();
    void do_write(const std::string &msg);
    void do_read();
    size_t read_complete(char *buf,const error_code &err,size_t bytes);
    void loop();
    void stop();

    //以下为用户操作
    //卡片操作
    void cardSearch();                     //从服务器上搜索10张卡片
    void createCard();                     //创建卡片
    void sendCard(Card &card);                        //send card to server
    void sendPicture(Card &card);           //发送卡片背景图
    void on_receive_picture(const Card &card); //接受卡片图
    //    void on_receive_picture_2(const std::string name);

    void deleteCard();                     //删除卡片
    void cardOperations();                 //卡片操作，包含评论、收藏等

    void searchCard();                      //搜索卡片
    void collectCard(const std::string &cardName);                     //收藏卡片
    void deleteCollectCard();             //删除收藏的卡片

    //卡包
    void createCardBag();                  //创建卡包
    void deleteCardBag();                  //删除卡包

    void fetchUserCard(const QString ID);                  //从服务器数据库获取用户的卡片

    void fetchUserCollectCard();           //从服务器数据库获取用户收藏的卡片

    //聊天
    void chat();                           //聊天
    void addFriends();                     //添加好友
    void deleteFrineds();                  //删除好友

    //账户
    void loginIn();                        //登录
    void registerAccount();                //注册
    void informationChange();              //修改个人信息
private:
    enum {max_msg = 1024};
    bool started;
    char read_buffer[max_msg];
    char write_buffer[max_msg];
    boost::asio::ip::tcp::socket m_sock;
    std::vector<Card> *searchCards;
    std::vector<Card> *userCards;
    std::vector<Card> *collectCards;

    std::vector<std::string> *sSearchCards;
    std::vector<std::string> *sUserCards;
    std::vector<std::string> *sCollectCards;
    //    boost::asio::io_service m_ser;
};

#endif // CLIENT_H


