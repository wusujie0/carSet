#include "database.h"
#include <QDebug>


using namespace std;

DataBase::DataBase()
{
    char cs1[] = "card";
    QString cs = cs1;
    QSqlDatabase db = connect(cs);
    m_query = QSqlQuery(db);
    m_query.exec("USE card");
    //    createTable("1");
}

QSqlDatabase DataBase::connect(const QString &DataBaseName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("chakra");
    db.setDatabaseName(DataBaseName);
    bool ok = db.open();
    if(!ok){
        qDebug() << "could't open the Database";
    }
    return db;
}

void DataBase::insertCard(const QString &ID, const QString &createTime,const QString &card,const QString &picturePosition)
{
    QString a = "INSERT INTO " + ID +"_card VALUES(\' " + ID + "\',\'" + createTime + "\',\'" + card + "\',\'" + picturePosition +"\')";
    if(!m_query.exec(a)){
        createTable(ID);
        m_query.exec(a);
    }

}

void DataBase::insertCollectCard(const QString &ID, const QString &createTime, const QString &card, const QString &picturePosition)
{
    QString a = "INSERT INTO " + ID +"_collect_card VALUES(\' " + ID + "\',\'" + createTime + "\',\'" + card + "\',\'" + picturePosition +"\')";
    if(!m_query.exec(a)){
        createTable(ID+"_collect");
        m_query.exec(a);
    }
}

void DataBase::deleteCard(const QString picturePosition, const QString ID)
{
    QString a = "DELETE FROM all_card WHERE picture = \'" + picturePosition +"\'";
//    QString ID = picturePosition[0];
    QString b = "DELETE FROM " + ID + "_card WHERE picture = \'" + picturePosition +"\'";
    m_query.exec(a);
    m_query.exec(b);

}

void DataBase::deleteCollectCard(const QString picturePosition, const QString ID)
{
    QString b = "DELETE FROM " + ID + "_collect_card WHERE picture = \'" + picturePosition +"\'";
    m_query.exec(b);
}

std::vector<std::string>DataBase::searchCard()
{

    m_query.exec("Select * FROM all_card limit 10");

    int i = 2;
    std::vector<std::string> result;
    while (m_query.next()) {
        result.push_back(m_query.value(i).toString().toStdString());
    }
    return result;
}

std::vector<string> DataBase::searchUserCard(const QString ID)
{
    QString a ="Select * FROM " + ID + "_card";
    m_query.exec(a);

    int i = 2;
    std::vector<std::string> result;
    while (m_query.next()) {
        result.push_back(m_query.value(i).toString().toStdString());
    }
    return result;
}

void DataBase::collectCard(const QString cardName, const QString ID)
{
    QString a = "Select * FROM all_card WHERE picture = \'" + cardName +"\'";
    m_query.exec(a);
    while (m_query.next()) {
        string card = m_query.value(2).toString().toStdString();
        Json::Value val;
        Json::Reader read;
        if(read.parse(card,val)){
            string UID = val["UID"].asString();
            string title = val["title"].asString();
            string contains = val["contains"].asString();
            string createTime = val["createTime"].asString();
            int pictureLength = val["pictureLength"].asInt();

            insertCollectCard(QString::fromStdString(UID),QString::fromStdString(createTime),QString::fromStdString(card),QString::fromStdString(UID+createTime));
        }

    }

}


void DataBase::createTable(const QString &ID)
{
    QString a = "CREATE TABLE "  + ID +  "_card("
                                         "id VARCHAR(30)  NOT NULL,"
                                         "createTime VARCHAR(30) PRIMARY KEY NOT NULL,"
                                         "card TEXT NOT NULL,"
                                         "picture VARCHAR(40) NOT NULL)";

    m_query.exec(a);

}
