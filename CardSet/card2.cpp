#include "card2.h"

Card2::Card2(QObject *parent):QObject(parent)
{

}

QString Card2::UID()
{
    return m_UID;
}

void Card2::setUID(QString a)
{
   m_UID = a;
}

QString Card2::title()
{
   return m_title;
}

void Card2::setTitle(QString a)
{
    m_title = a;
}

QString Card2::contains()
{
    return m_contains;
}

void Card2::setContains(QString a)
{
    m_contains = a;
}

QString Card2::createTime()
{
    return m_createTime;
}

void Card2::setCreateTime(QString a)
{
    m_createTime = a;
}

QString Card2::pic()
{
    return m_pic;
}

void Card2::setPic(QString a)
{
    m_pic = a;
}
