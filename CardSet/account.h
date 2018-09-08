#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QObject>

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <algorithm>
#include <QQmlListProperty>

#include "chat_client.h"
#include "friend.h"
#include "client.h"
#include "card.h"
#include "card2.h"

using namespace boost::asio;

class Account:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString sendMessage READ sendMessage WRITE setSendMessage NOTIFY sendMessageChanged)



    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString contains READ contains WRITE setContains NOTIFY containsChanged)
    Q_PROPERTY(QString  pic READ pic WRITE setPic NOTIFY picChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)


    Q_PROPERTY(QQmlListProperty<Card2> userCard READ userCard  NOTIFY userCardChanged)
    Q_PROPERTY(QQmlListProperty<Card2> searchCard READ searchCard NOTIFY searchCardChanged)


public:
    Account(QObject *parent = 0);



    Q_INVOKABLE void appendCard(Card2 *c) ;
    Q_INVOKABLE int countCard() ;
    Q_INVOKABLE Card2 * card(int index) ;
    Q_INVOKABLE void clearCard() ;

    Q_INVOKABLE Card2 * sCard(int index);
    Q_INVOKABLE int countsCard() ;

    Q_INVOKABLE QString title();
    Q_INVOKABLE void setTitle(QString title);

    Q_INVOKABLE QString contains();
    Q_INVOKABLE void setContains(QString contains);

    Q_INVOKABLE QString pic();
    Q_INVOKABLE void setPic(QString pic);

    Q_INVOKABLE QString message();
    Q_INVOKABLE void setMessage(QString message);

    Q_INVOKABLE QQmlListProperty<Card2> userCard();
//    Q_INVOKABLE void setUserCard(QQmlListProperty<Card2> a);

    Q_INVOKABLE QQmlListProperty<Card2> searchCard();
//    Q_INVOKABLE void setSearchCard(QQmlListProperty<Card2> a);

    void getUserCard();
    void getSearchCard();


    Q_INVOKABLE QString userName() const;
    Q_INVOKABLE void setUserName(const QString &userName);

    Q_INVOKABLE QString password() const;
    Q_INVOKABLE void setPassword(const QString &password);

    Q_INVOKABLE void run();
    ~Account();

    QString sendMessage() const;
    Q_INVOKABLE void setSendMessage(const QString &sendMessage);

    void getReceiveMessage();
    void receiveMessageLoop();
    void UIshowFriends();
    void UIshowChatLog(std::string friendname);


    static void appendCard(QQmlListProperty<Card2>* list, Card2* p);
    static Card2* card(QQmlListProperty<Card2>* list, int i);
    static void clearCard(QQmlListProperty<Card2>* list);
    static int countCard(QQmlListProperty<Card2>* list);

signals:
    void userNameChanged();
    void passwordChanged();
    void sendMessageChanged();
    void registerSuccess();
    void registerFailed();
    void loginSuccess();
    void loginFailed();
    void showFriends(QList<QString> vec, QList<int> vec_num);
    void showChatLog(QList<QString> vec1,QList<QString> vec2);

    void titleChanged();
    void containsChanged();
    void picChanged();
    void messageChanged();
    void userCardChanged(QList<Card2*> m_userCard);
    void searchCardChanged();
private:
    QString m_userName;
    QString m_password;
    QString m_sendMessage;//从qml端组成发送的字符串
    std::string m_receiveMessage;

    std::vector<chat_friend*> friends;
    io_service m_udp_io;
    io_service m_card;
    Chat_Client m_chat_cli;


    QString m_title;
    QString m_contains;
    QString m_pic;
    QString m_message;
    Client m_client;

    QList<Card2*> m_userCard;
    QList<Card2*> m_searchCard;
};

#endif // ACCOUNT_H
