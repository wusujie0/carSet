#include "client.h"
#include <iostream>
#include <json/json.h>
#include <json/writer.h>
#include <QDebug>
using namespace boost::asio;
using namespace std;


#define Max_Len 1024*1024


void Client::connect()
{

}

void Client::do_write(const string &msg)
{
    m_sock.write_some(buffer(msg,msg.size()));
}

void Client::do_read()
{
    m_sock.read_some(buffer(read_buffer));
}

size_t Client::read_complete(char *buf, const error_code &err, size_t bytes)
{
    if(err) return 0;
}

void Client::loop()
{

    //    while(started){
    //        cout << "choose the information" << endl;
    //        string a;
    //        cin >> a;
    //        if(a.find("Create_Card") == 0){
    //            do_write("Create_Card");
    //            m_sock.read_some(buffer(read_buffer));
    //            createCard();
    //        }
    //        else if(a.find("Create_Card_Bag") == 0){
    //            do_write("Create_Card_Bag");
    //            createCardBag();
    //        }
    //        else if(a.find("Search_Card") == 0){
//                do_write("Search_Card");
    //            searchCard();
    //        }
    //        else if(a.find("Collect_Card") == 0){
    //            do_write("Collect_Card");
    //            collectCard(" ");
    //        }
    //        else if(a.find("Fetch_User_Card") == 0){
    //            do_write("Fetch_User_Card");
    //            fetchUserCard();
    //        }
    //        else if(a.find("Fetch_User_Collect_Card") == 0){
    //            do_write("Fetch_User_Collect_Card");
    //            fetchUserCollectCard();
    //        }
    //        else if(a.find("Delete_Card") == 0){
    //            do_write("Delete_Card");
    //            deleteCard();
    //        }
    //        else if(a.find("Delete_Collect_Card") == 0){
    //            do_write("Delete_Collect_Card");
    //            deleteCollectCard();
    //        }
    //    }
}

void Client::stop()
{

}


void Client::createCard()
{
//    Card* new_card = new Card();

//    cout << "enter the ID" << endl;
//    string ID;
//    cin >> ID;
//    new_card->UID = ID;

//    cout << "enter the title" << endl;
//    string title;
//    cin >> title;
//    new_card->title = title;

//    cout << "enter the contains" << endl;
//    string contains;
//    cin >> contains;
//    new_card->contains = contains;

//    sendPicture(*new_card);

}

void Client::sendCard(Card &card)
{
    memset(write_buffer,0,sizeof(write_buffer));
    //   memcpy(write_buffer,&card,sizeof(card));

    Json::Value j = card.toJson();
    std::string t = j.toStyledString();

    t.copy(write_buffer,t.length(),0);
    m_sock.write_some(buffer(write_buffer));

}

void Client::sendPicture(Card &card)
{
    memset(write_buffer,0,sizeof(write_buffer));
//    char ImgName[100] = "../imagetest1/images/timg2.jpg";
    char ImgName[100];

    memset(ImgName,0,sizeof(ImgName));

    do_write("Create_Card");
    m_sock.read_some(buffer(read_buffer));

    cout << card.pic << endl;
    int i;
    for (i = 7; i < card.pic.size();i++){

        ImgName[i-7] = card.pic[i];
        cout << ImgName[i-7];
    }
    ImgName[i] ='\0';


    //图像数据长度
    int length;
    //文件指针
    FILE *fp;
    //以二进制方式打开图像
    if ( (fp = fopen(ImgName, "rb" )) == NULL )
    {
        cout << "Open image failed!" << endl;
        exit(0);
    }
    //获取图像数据总长度
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    card.pictureLength = length;
    //    m_sock.read_some(buffer(read_buffer));
    sendCard(card);
    m_sock.read_some(buffer(read_buffer));
    memset(read_buffer,0,sizeof(read_buffer));
    rewind(fp);
    //根据图像数据长度分配内存buffer
    char* ImgBuffer=(char*)malloc( length* sizeof(char) );
    //将图像数据读入buffer
    fread(ImgBuffer, length, 1, fp);
    //std::cout << ImgBuffer << std::endl;
    fclose(fp);
    string r;
    for (int i = 0; i< length ; i++) {
        r.push_back(*(ImgBuffer+i));
    }
    //    std::cout << sizeof(ImgBuffer) << endl;
    free(ImgBuffer);
    m_sock.write_some(buffer(r,r.size()));

}

void Client::on_receive_picture(const Card &card)
{
    string name = "/root/pic/" + card.UID + card.createTime;

    FILE *fp;
    //输入要保存的文件名
    char OutImgName[100];

    char buf[Max_Len];
    for (int i = 0; i < Max_Len; i++) {
        buf[i] = '\0';
    }
    cout << "yes " << endl;
    size_t length = m_sock.read_some(buffer(buf));
    cout << "The picture length is:";
    cout << length << endl;

    int i;
    for( i = 0; i < name.length();i++){
        OutImgName[i] = name[i];
    }
    OutImgName[i] = '\0';

    //以二进制写入方式
    if ( (fp = fopen(OutImgName, "wb")) == NULL)
    {
        cout<<"Open File failed!"<<endl;
        exit(0);
    }
    //从buffer中写数据到fp指向的文件中
    //fwrite(buf, Max_Len, 1, fp);
    fwrite(buf, length, 1, fp);
    cout<<"Done!"<<endl;
    //关闭文件指针，释放buffer内存
    fclose(fp);

}


void Client::deleteCard()
{
    do_read();                                        //读取服务器发送的读取成功消息
    cout << "enther the Card picPositon" << endl;
    string picPosition;
    cin >> picPosition;
    string temp;
    cin >> temp;
    string a = picPosition + " " + temp;
    cout << a << endl;
    do_write(a);                                    //发送被删除卡片的唯一标识符

    do_read();                                        //读取服务器发送的读取成功消息

    cout << "enther the UID" << endl;
    string UID;
    cin >> UID;
    cout << UID << endl;

    do_write(UID);                                    //发送用户的ID

}

void Client::searchCard()
{
    do_write("Search_Card");
    Card *card = new Card();
    for(int n = 0;n < 10;n++){
        memset(read_buffer,0,sizeof(read_buffer));
        m_sock.read_some(buffer(read_buffer));
        string cardIn = read_buffer;
        Json::Value val;
        Json::Reader read;
        if(read.parse(cardIn,val)){
            card->UID = val["UID"].asString();
            card->title = val["title"].asString();
            card->contains = val["contains"].asString();
            card->createTime = val["createTime"].asString();
            card->pictureLength = val["pictureLength"].asInt();
            cout << val << endl;
            searchCards->push_back(*card);
            sSearchCards->push_back(cardIn);
        }

        memset(read_buffer,0,sizeof(read_buffer));
        m_sock.write_some(buffer("ok"));
        on_receive_picture(*card);
        do_write("ok");
    }
    delete(card);
}

void Client::collectCard(const std::string &cardName)
{
    do_read();
    do_write(" ");     //发送cardName;
    do_read();         //读取服务器发送的读取成功消息
    do_write(" ");     //发送收藏者的ID
}

void Client::deleteCollectCard()
{
    do_read();          //读取服务器发送的读取成功消息
    do_write(" ");     //发送cardName;
    do_read();         //读取服务器发送的读取成功消息
    do_write(" ");     //发送收藏者的ID
}

void Client::createCardBag()
{
    cout << "创建卡包" << endl;
    cout << "请输入卡包的名字" << endl;
    string cardBagName;
    cin >> cardBagName;

}

void Client::fetchUserCard(const QString ID)
{
    do_write("Fetch_User_Card");

    do_read();
    cout << ID.toStdString() << endl;
    do_write(ID.toStdString());                 //用户的帐号


    do_read();
    string cardNum = read_buffer;
    cout << cardNum << endl;

    do_write("ok");

    int length = atoi(cardNum.c_str());
    cout << length << endl;

    for(int i = 0; i < length;i++){
        do_read();
        do_write("ok");
        string cards = read_buffer;
        cout << cards << endl;

        Json::Value val;
        Json::Reader read;
        if(read.parse(cards,val)){
            auto new_card = new Card();
            new_card->UID = val["UID"].asString();
            new_card->title = val["title"].asString();
            new_card->contains = val["contains"].asString();
            new_card->createTime = val["createTime"].asString();
            new_card->pictureLength = val["pictureLength"].asInt();
            userCards->push_back(*new_card);
            sUserCards->push_back(cards);
            delete(new_card);
        }

        memset(read_buffer,0,sizeof(read_buffer));
    }

    for(auto &i:*userCards){
        on_receive_picture(i);
        do_write("ok");
    }

}

void Client::fetchUserCollectCard()
{
    do_read();
    do_write("1");                 //用户的帐号


    do_read();
    string cardNum = read_buffer;
    cout << cardNum << endl;

    do_write("ok");

    int length = atoi(cardNum.c_str());
    cout << length << endl;

    for(int i = 0; i < length;i++){
        do_read();
        do_write("ok");
        string cards = read_buffer;
        cout << cards << endl;

        Json::Value val;
        Json::Reader read;
        if(read.parse(cards,val)){
            auto new_card = new Card();
            new_card->UID = val["UID"].asString();
            new_card->title = val["title"].asString();
            new_card->contains = val["contains"].asString();
            new_card->createTime = val["createTime"].asString();
            new_card->pictureLength = val["pictureLength"].asInt();
            collectCards->push_back(*new_card);
            sCollectCards->push_back(cards);
            delete(new_card);
        }

        memset(read_buffer,0,sizeof(read_buffer));
    }

    for(auto &i:*userCards){
        on_receive_picture(i);
        do_write("ok");
    }
}

