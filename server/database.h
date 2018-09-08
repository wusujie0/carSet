#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <vector>
#include "./json/json.h"

class DataBase
{
public:
    DataBase();

    QSqlDatabase connect(const QString &DataBaseName);

    void createTable(const QString &ID);
    void insertCard(const QString &ID, const QString &createTime,const QString &card,const QString &picturePosition);
    void insertCollectCard(const QString &ID, const QString &createTime,const QString &card,const QString &picturePosition);
    void deleteCard(const QString picturePosition,const QString ID);
    void deleteCollectCard(const QString picturePosition,const QString ID);

    std::vector<std::string> searchCard();      //搜索所有用户的卡片
    std::vector<std::string> searchUserCard(const QString ID);  //搜索用户自己的卡片

    void changeCard();    //修改卡片信息，如评论等

    void insertCardBag();

    void collectCard(const QString cardName,const QString ID);     //收藏卡片

    void insertUser();
    void deleteUser();
    void changeUserInformation();      //修改用户信息,性别，昵称等

private:
    QSqlQuery m_query;

};

#endif // DATABASE_H
