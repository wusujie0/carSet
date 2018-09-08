#ifndef CARD2_H
#define CARD2_H

#include <QObject>

class Account;

class Card2 :public QObject
{
    friend class Account;
    Q_OBJECT
    Q_PROPERTY(QString UID READ UID WRITE setUID NOTIFY UIDChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString contains READ contains WRITE setContains NOTIFY containsChanged)
    Q_PROPERTY(QString createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged)
    Q_PROPERTY(QString pic READ pic WRITE setPic NOTIFY picChanged)


public:

    Card2(QObject *parent = 0);

    Q_INVOKABLE QString UID();
    Q_INVOKABLE void setUID(QString a);

    Q_INVOKABLE QString title();
    Q_INVOKABLE void setTitle(QString a);

    Q_INVOKABLE QString contains();
    Q_INVOKABLE void setContains(QString a);

    Q_INVOKABLE QString createTime();
    Q_INVOKABLE void setCreateTime(QString a);

    Q_INVOKABLE QString pic();
    Q_INVOKABLE void setPic(QString a);

signals:
    void UIDChanged();
    void titleChanged();
    void containsChanged();
    void createTimeChanged();
    void picChanged();

private:
    QString m_UID;
    QString m_title;
    QString m_contains;
    QString m_createTime;
    QString m_pic;

};

#endif // CARD2_H
