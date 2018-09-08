#include "account.h"

#include <string>
#include <vector>
#include <QDebug>

Account::Account(QObject *parent):QObject(parent),m_chat_cli(m_udp_io),m_client(m_card)
{
    boost::thread t1(boost::bind(&Account::receiveMessageLoop,this));
    //    t1.join();

    m_client.searchCard();
    m_client.fetchUserCard("1");

    getUserCard();
    getSearchCard();
}

QString Account::userName() const
{
    return m_userName;
}

void Account::setUserName(const QString &userName)
{
    m_userName = userName;
    userNameChanged();

}

QString Account::password() const
{
    return m_password;
}

void Account::setPassword(const QString &password)
{
    m_password = password;
}

void Account::run()
{
    m_udp_io.run();
    std::cout << "run" << std::endl;
}

Account::~Account()
{
    //    run();
}

QString Account::sendMessage() const
{
    return m_sendMessage;
}

void Account::setSendMessage(const QString &sendMessage)
{
    m_sendMessage = sendMessage;
    std::cout << "send:    " << m_sendMessage.toStdString() << std::endl;
    m_chat_cli.doSend(m_sendMessage.toStdString());

}

void Account::getReceiveMessage()
{
    m_chat_cli.doReceive(m_receiveMessage);

    std::cout << "receive: " << m_receiveMessage << std::endl;
    std::istringstream record(m_receiveMessage);
    std::string ope;
    record >> ope;
    if(ope == "register") {
        std::string r;
        record >> r;
        if(r == "suc")
            registerSuccess();
        else registerFailed();
    }
    else if(ope == "login") {
        std::string  r;
        record >> r;
        if(r == "suc") {
            loginSuccess();
            std::string friendname;
            int unread_log_num;
            std::string tmp;
            while (record >> friendname >> unread_log_num >> tmp) {
                if (tmp == "|||") {
                    friends.push_back(new chat_friend(friendname, unread_log_num));
                    //                    std::string r_chatlog = "receiveChatLog " +m_userName.toStdString()+ " "+friendname;
                    //                    m_chat_cli.doSend(r_chatlog);
                }
            }

            UIshowFriends();

            //            for (int i = 0; i < friends.size(); i++) {

            //                std::cout << "friends" << i + 1 << std::endl;
            //                std::cout << friends[i]->getFriendname() << std::endl;
            //                std::cout << friends[i]->getFriendUnreadMesNum() << std::endl;

            //            }
        }
        else loginFailed();
    }
    else if(ope == "friendsMes") {
            std::string friendname;
            int unread_log_num;
            std::string tmp;
            while (record >> friendname >> unread_log_num >> tmp) {
                if (tmp == "|||") {
                    for (int i = 0; i < friends.size(); i++) {
                        std::string realname;
                        friends[i]->getFriendname(realname);
                        if (friendname == realname) {
                            friends[i]->setFriendUnreadMesNum(unread_log_num);
                        }
                    }
                }
            }

            UIshowFriends();


    }
    else if(ope == "ChatLog") {
        std::string friendname;
        record >> friendname;
        for (int i = 0; i < friends.size(); i++) {
            std::string realname;
            friends[i]->getFriendname(realname);
            if (friendname == realname) {
                std::cout << m_receiveMessage << std::endl;
                friends[i]->setFriendChatLog(m_receiveMessage);
            }
        }
        UIshowChatLog(friendname);
    }
    else if(ope == "NewMes") {
        std::string friendname;
        std::string sender;
        std::string receiver;
        std::string message;
        record >> sender >> receiver >> message;
        if (sender == m_userName.toStdString())
            friendname = receiver;
        else
            friendname = sender;
        for (int i = 0; i < friends.size(); i++) {
            std::string realname;
            friends[i]->getFriendname(realname);

            if (friendname == realname) {
                std::cout << m_receiveMessage << std::endl;
                friends[i]->addNewMes(sender,receiver,message);
            }
        }
        std::string receivechatlog = "receiveChatLog "+receiver+ " "+sender+" off";
        m_chat_cli.doSend(receivechatlog);
        std::string receiveFriendMes = "getfriendsMes "+receiver;
        m_chat_cli.doSend(receiveFriendMes);
    }
}

void Account::UIshowFriends()
{
    QList<QString> vec;
    QList<int> vec_num;
    for (int i = 0; i < friends.size(); i++) {
        std::string friendname;
        friends[i]->getFriendname(friendname);
        QString q = friendname.c_str();
        vec.push_back(q);
        int num = friends[i]->getFriendUnreadMesNum();
        vec_num.push_back(num);
    }
    showFriends(vec,vec_num);
}

void Account::UIshowChatLog(std::string friendname)
{
    std::string chatlog;
    for (int i = 0; i < friends.size(); i++) {
        std::string fname;
        friends[i]->getFriendname(fname);
        if (friendname == fname)
            friends[i]->getFriendChatLog(chatlog);
    }
    std::istringstream record(chatlog);
    QList<QString> vec1;
    QList<QString> vec2;
    std::string sender;
    std::string receiver;
    std::string message;
    std::string tmp;
    record >> tmp;
    record >> tmp;
    record >> tmp;
    std::cout << tmp << std::endl;
    if (tmp == "+++") {
        while (record >> sender >> receiver >> message >> tmp) {
            if (tmp == "|||") {
                if (receiver == friendname) {
                    QString me = "true";
                    vec1.push_back(me);
                }else {
                    QString me = "false";
                    vec1.push_back(me);
                }
                QString text = message.c_str();
                vec2.push_back(text);

            }
        }
    }
    showChatLog(vec1,vec2);

}

void Account::receiveMessageLoop()
{
    while (true)
        getReceiveMessage();
}



QString Account::title()
{
    return m_title;
}

void Account::setTitle(QString title)
{
    m_title = title;
}

QString Account::contains()
{
    return m_contains;
}

void Account::setContains(QString contains)
{
    m_contains = contains;
}

QString Account::pic()
{
    return m_pic;
}

void Account::setPic(QString pic)
{
    m_pic = pic;
}

QString Account::message()
{
    return m_message;
}

void Account::setMessage(QString message)
{

    auto new_card = new Card();
    new_card->title = title().toStdString();
    new_card->contains = contains().toStdString();
    new_card->pic = pic().toStdString();
    new_card->UID = m_userName.toStdString();

    std::cout << m_userName.toStdString() <<std::endl;

    m_client.sendPicture(*new_card);
}

void Account::appendCard(QQmlListProperty<Card2> *list, Card2 *p)
{
    reinterpret_cast< Account* >(list->data)->appendCard(p);
}

Card2 *Account::card(QQmlListProperty<Card2> *list, int i)
{
    return reinterpret_cast<Account* >(list->data)->card(i);
}

void Account::clearCard(QQmlListProperty<Card2> *list)
{
    reinterpret_cast<Account* >(list->data)->clearCard();
}

int Account::countCard(QQmlListProperty<Card2> *list)
{
    return reinterpret_cast< Account* >(list->data)->countCard();
}

QQmlListProperty<Card2> Account::userCard()
{
    return QQmlListProperty<Card2>(this,this,&Account::appendCard,&Account::countCard,&Account::card,&Account::clearCard);
}

QQmlListProperty<Card2> Account::searchCard()
{
    return QQmlListProperty<Card2>(this,this,&Account::appendCard,&Account::countCard,&Account::card,&Account::clearCard);
}


void Account::appendCard(Card2 *c)
{
    m_userCard.push_back(c);
}

int Account::countCard()
{
    return m_userCard.count();
}

Card2 *Account::card(int index)
{
    return m_userCard.at(index);
}

void Account::clearCard()
{
    m_userCard.clear();
}

Card2 *Account::sCard(int index)
{
    return m_searchCard.at(index);
}

int Account::countsCard()
{
    return m_searchCard.count();
}

void Account::getUserCard()
{
    for(auto i = m_client.sUserCards->begin();i != m_client.sUserCards->end();i++){
       auto card = new Card2();
        std::cout << *i << std::endl;
        Json::Value val;
        Json::Reader read;
        if(read.parse(*i,val)){
            card->setUID(QString::fromStdString(val["UID"].asString()));
            card->setTitle(QString::fromStdString(val["title"].asString()));
            card->setContains(QString::fromStdString(val["contains"].asString()));
            card->setCreateTime(QString::fromStdString(val["createTime"].asString()));
        }
//        m_userCard.push_back(card->UID());
//        m_userCard.push_back(card->title());
//        m_userCard.push_back(card->contains());
//        m_userCard.push_back(card->createTime());
        m_userCard.push_back(card);
    }

    std::cout << "this" << std::endl;
    int a = 0;
//    for(auto i : m_userCard){
//        if(a ==3){
//        std::cout << i.toStdString() ;
//        std::cout << " ";
//        }
//        a++;
//    }
    userCardChanged(m_userCard);
}

void Account::getSearchCard()
{
    for(auto i = m_client.sSearchCards->begin();i != m_client.sSearchCards->end();i++){
       auto card = new Card2();
        std::cout << *i << std::endl;
        Json::Value val;
        Json::Reader read;
        if(read.parse(*i,val)){
            card->setUID(QString::fromStdString(val["UID"].asString()));
            card->setTitle(QString::fromStdString(val["title"].asString()));
            card->setContains(QString::fromStdString(val["contains"].asString()));
            card->setCreateTime(QString::fromStdString(val["createTime"].asString()));
        }
        m_searchCard.push_back(card);
//        m_searchCard.push_back(card->UID());
//        m_searchCard.push_back(card->title());
//        m_searchCard.push_back(card->contains());
//        m_searchCard.push_back(card->createTime());
    }
   searchCardChanged();
}
