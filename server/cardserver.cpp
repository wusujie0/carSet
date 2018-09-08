#include "cardserver.h"

using namespace boost::asio;
using namespace std;

#define Max_Len 1024*1024

CardServer::CardServer():m_acc(m_ser, ip::tcp::endpoint (ip::tcp::endpoint(ip::tcp::v4(),2001)))
{

}



void CardServer::start()
{
    started = true;
    socket_ptr sock(new ip::tcp::socket(m_ser));
    m_acc.async_accept(*sock,boost::bind(&CardServer::handle_start,this,_1,sock));
    m_ser.run();
}

void CardServer::stop()
{

}

void CardServer::handle_start(const boost::system::error_code &err,socket_ptr sock)
{

    std::cout << sock->remote_endpoint().address() << std::endl;
    if(err) return;
    //    async_read(*sock,buffer(read_buffer),boost::bind(&CardServer::on_read,this,_1,_2,sock));
    do_read(sock);
    start();

}



void CardServer::do_read(socket_ptr sock)
{
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_read,this,_1,sock));
    //     async_read(*sock,buffer(read_buffer),boost::bind(&CardServer::on_read,this,_1,_2,sock));
}

void CardServer::on_read(const error_code &err,socket_ptr sock)
{
    if(err) stop();
    if(!started) return;

    std::string msg(read_buffer);
    cout << msg << endl;
    if(msg.find("Create_Card") == 0){
        sock->write_some(buffer("ok"));
        on_create_card(msg,sock);
    }
    else if(msg.find("Search_Card") == 0){
        on_search_card(sock);
        memset(read_buffer,0,sizeof(read_buffer));
    }
    else if(msg.find("Delete_Card") == 0){
        sock->async_write_some(buffer("ok"),boost::bind(&CardServer::on_delete_card,this,_1,sock));
    }
    else if(msg.find("Create_Card_Bag") == 0){
        //        on_create_cardBag(sock);
    }
    else if(msg.find("Fetch_User_Card") == 0){
        sock->async_write_some(buffer("ok"),boost::bind(&CardServer::on_fetch_user_card,this,_1,sock));
    }
    else if(msg.find("Fetch_User_Collect_Card") == 0){
        sock->async_write_some(buffer("ok"),boost::bind(&CardServer::on_fetch_user_collect_card,this,_1,sock));
    }
    else if(msg.find(("Collect_Card")) == 0){
        sock->async_write_some(buffer("ok"),boost::bind(&CardServer::on_collect_card,this,_1,sock));
    }
    else if(msg.find(("Delete_Collect_Card")) == 0){
        sock->async_write_some(buffer("ok"),boost::bind(&CardServer::on_delete_collect_card,this,_1,sock));
    }
}

void CardServer::read_handle(const boost::system::error_code &err)
{
    if(err){
        cout << "read failed!" << endl;
    }
    cout << "read successfully!" <<endl;
}



void CardServer::do_write(CardServer::socket_ptr sock, string &msg)
{
    memset(write_buffer,0,sizeof(write_buffer));
    for(int i = 0;i < msg.length();i++){
        write_buffer[i] = msg[i];
    }
    async_write(*sock,buffer(write_buffer),boost::bind(&CardServer::on_write,this,_1,sock));
}

void CardServer::on_write(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    do_read(sock);
}

void CardServer::write_handle(const boost::system::error_code &err)
{
    if(err){
        cout << "write failed!" << endl;
    }
    cout <<"write successfully!" << endl;
}

void CardServer::on_create_card(const std::string &msg,socket_ptr sock)
{
    Card* card = new Card();
    //    char temp[max_msg];
    //    memset(temp,0,sizeof(temp));
    //    for(int i = 0;i < max_msg;i++){
    //        temp[i] = msg[i+11];
    //    }

    read(*sock,buffer(read_buffer));
    string receive = read_buffer;
    cout <<receive << endl;
    Json::Value val;
    Json::Reader rea;
    if(rea.parse(receive,val)){
        card->UID = val["UID"].asString();
        card->title = val["title"].asString();
        card->contains = val["contains"].asString();
        card->createTime = val["createTime"].asString();
        card->pictureLength = val["pictureLength"].asInt();
    }

    string cardInformation = val.toStyledString();
    string cardName = card->UID + card->createTime;

    m_db.insertCard(QString::fromStdString(card->UID),QString::fromStdString(card->createTime),QString::fromStdString(cardInformation),QString::fromStdString(cardName));  //将卡片信息插入进用户专属的数据表

    m_db.insertCard(QString::fromStdString("all"),QString::fromStdString(card->createTime),QString::fromStdString(cardInformation),QString::fromStdString(cardName));  //将卡片插入进所有卡片的数据表

    memset(pic_buf,0,sizeof(pic_buf));
    sock->write_some(buffer("ok"));
    async_read(*sock,buffer(pic_buf,card->pictureLength),boost::bind(&CardServer::on_receive_picture,this,_1,sock,*card)); //由于read_some在读取Json字符串时会在读取中途返回,所以此处用read函数，读取完整数据


}

void CardServer::on_receive_picture(const boost::system::error_code &err,CardServer::socket_ptr sock,const Card &card)
{
    int length = card.pictureLength;

    FILE *fp;
    string name = card.UID + card.createTime;

    char OutImgName[100];
    //    cout << "Enter the name you wanna to save:";
    //    cin >> OutImgName;
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
    fwrite(pic_buf, length, 1, fp);
    cout<<"Done!"<<endl;
    //关闭文件指针，释放buffer内存
    fclose(fp);

    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_read,this,_1,sock));

}

void CardServer::on_search_card(CardServer::socket_ptr sock)
{

    auto result = m_db.searchCard();
    int n = 0;
    for(auto &i:result){
        string temp = i;
        Json::Value val;
        Json::Reader read;
        if(n != 0){
            sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_search_card_2,this,_1,sock,temp));
        }

        if (n == 0) sock->async_write_some(buffer(temp),boost::bind(&CardServer::on_search_card_handler,this,_1));

        Card *card = new Card();
        if(read.parse(temp,val)){
            card->UID = val["UID"].asString();
            card->createTime= val["createTime"].asString();
            card->pictureLength = val["pictureLength"].asInt();
            cout << val << endl;
        }
        string pictureName = card->UID + card->createTime;

        sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::send_picture,this,_1,pictureName,sock));
        n++;
        if( n == result.size()) sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::read_handle,this,_1));
    }
    do_read(sock);

}

void CardServer::on_search_card_2(const boost::system::error_code &err,CardServer::socket_ptr sock,const string card)
{
    if(err){
        cout << "search card error" << endl;
        return;
    }
    sock->async_write_some(buffer(card),boost::bind(&CardServer::on_search_card_handler,this,_1));
}

void CardServer::on_search_card_handler(const boost::system::error_code &err)
{
    if(err){
        cout << "search card error" << endl;
        return;
    }


}

void CardServer::on_delete_collect_card(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    if(err){
        cout << "delete collect card fail!" << endl;
    }
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_delete_collect_card_handle,this,_1,sock));
}

void CardServer::on_delete_card(const boost::system::error_code &err,CardServer::socket_ptr sock)
{
    if(err){
        cout << "delete card fail!" << endl;
        return;
    }
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_delete_card_handle,this,_1,sock));
}

void CardServer::on_delete_card_handle(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    if(err){
        cout << "delete card failed"<<endl;
        return;
    }
    string picPosition = read_buffer;
    cout << picPosition << endl;

    sock->async_write_some(buffer("ok"),boost::bind(&CardServer::write_handle,this,_1));

    memset(read_buffer,0,sizeof(read_buffer));

    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_delete_card_handle_2,this,_1,sock,picPosition));

}

void CardServer::on_delete_card_handle_2(const boost::system::error_code &err, CardServer::socket_ptr sock, const string pic)
{
    string ID = read_buffer;
    cout << ID << endl;
    m_db.deleteCard(QString::fromStdString(pic),QString::fromStdString(ID));
    do_read(sock);
}

void CardServer::on_delete_collect_card_handle(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    if(err){
        cout << "delete collect card failed"<<endl;
        return;
    }

    sock->async_write_some(buffer("ok"),boost::bind(&CardServer::write_handle,this,_1));
    string picPosition = read_buffer;
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_delete_collect_card_handle_2,this,_1,picPosition));


}

void CardServer::on_delete_collect_card_handle_2(const boost::system::error_code &err, const string pic)
{
    string ID = read_buffer;
    m_db.deleteCollectCard(QString::fromStdString(pic),QString::fromStdString(ID));
}



void CardServer::on_collect_card(const boost::system::error_code &err,CardServer::socket_ptr sock)
{
    if(err){
        cout << "collect failed"<<endl;
        return;
    }
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::read_handle,this,_1));   //读取cardName
    string cardName = read_buffer;

    sock->async_write_some(buffer("ok"),boost::bind(&CardServer::write_handle,this,_1));
    memset(read_buffer,0,sizeof(read_buffer));

    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_collect_card_handle,this,_1,cardName));  //读取收藏者ID


}

void CardServer::on_collect_card_handle(const boost::system::error_code &err,const string cardName)
{
    if(err){
        return;
    }
    string ID = read_buffer;
    m_db.collectCard(QString::fromStdString(cardName),QString::fromStdString(ID));
}

void CardServer::on_create_cardBag(CardServer::socket_ptr sock)
{

}

void CardServer::send_picture(const boost::system::error_code &err,const string pictureName,CardServer::socket_ptr sock)
{
    if(err){
        cout << "send picture failed!" << endl;
        return;
    }
    memset(write_buffer,0,sizeof(write_buffer));

    char ImgName[100] = "../imagetest1/images/timg2.jpg";

    //    char ImgName[100];
    //    int i;
    //    for(i = 0;i < pictureName.length();i++){
    //        ImgName[i] = pictureName[i];
    //    }
    //    ImgName[i] = '\0';
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
    //    memset(read_buffer,0,sizeof(read_buffer));

    sock->async_write_some(buffer(r,r.size()),boost::bind(&CardServer::send_picture_handler,this,_1));
}

void CardServer::send_picture_handler(const boost::system::error_code &err)
{
    if(err){
        cout << "send picture failed!" << endl;
        return;
    }
    std::cout << "图片已发送" << std::endl;
}

void CardServer::on_fetch_user_card(const boost::system::error_code &err,CardServer::socket_ptr sock)
{
    memset(read_buffer,0,sizeof(read_buffer));
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_fetch_user_card_handle,this,_1,sock));

}

void CardServer::on_fetch_user_card_handle(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    string ID = read_buffer;
    cout << ID << endl;
    auto result = m_db.searchUserCard(QString::fromStdString(ID));


    int length = 0;
    for(auto &i:result){
        length++;
    }

    string l = std::to_string(length);
    sock->async_write_some(buffer(l),boost::bind(&CardServer::write_handle,this,_1));

    for(int i = 0; i < length;i++){
        string userCards = result[i].data();
        sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::send_card_fetch,this,_1,userCards,sock));
    }

    for(int i = 0;i < length;i++){
        Json::Value val;
        Json::Reader read;
        read.parse(result[i].data(),val);
        string picName = val["UID"].asString() + val["createTime"].asString();
        cout << picName << endl;
        sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::send_picture_fetch,this,_1,picName,sock));
        if(i == length-1) sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::read_handle,this,_1));
    }
    do_read(sock);

}

void CardServer::on_fetch_user_collect_card(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    memset(read_buffer,0,sizeof(read_buffer));
    sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::on_fetch_user_card_handle,this,_1,sock));
}

void CardServer::on_fetch_user_collect_card_handle(const boost::system::error_code &err, CardServer::socket_ptr sock)
{
    string ID = read_buffer;
    cout << ID << endl;
    auto result = m_db.searchUserCard(QString::fromStdString(ID + "_collect"));


    int length = 0;
    for(auto &i:result){
        length++;
    }

    string l = std::to_string(length);
    sock->async_write_some(buffer(l),boost::bind(&CardServer::write_handle,this,_1));

    for(int i = 0; i < length;i++){
        string userCards = result[i].data();
        sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::send_card_fetch,this,_1,userCards,sock));
    }

    for(int i = 0;i < length;i++){
        Json::Value val;
        Json::Reader read;
        read.parse(result[i].data(),val);
        string picName = val["UID"].asString() + val["createTime"].asString();
        cout << picName << endl;
        sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::send_picture_fetch,this,_1,picName,sock));
        if(i == length-1) sock->async_read_some(buffer(read_buffer),boost::bind(&CardServer::read_handle,this,_1));
//        if(i == length-1)  sock->async_write_some(buffer("ok"),boost::bind(&CardServer::write_handle,this,_1));
    }
    do_read(sock);
}

void CardServer::send_card_fetch(const boost::system::error_code &err, const string card, CardServer::socket_ptr sock)
{
    sock->async_write_some(buffer(card,card.size()),boost::bind(&CardServer::write_handle,this,_1));
}

void CardServer::send_picture_fetch(const boost::system::error_code &err, const string pictureName, CardServer::socket_ptr sock)
{
    if(err){
        cout << "err!" <<endl;
        return;
    }
    memset(write_buffer,0,sizeof(write_buffer));

    char ImgName[100] = "../imagetest1/images/timg2.jpg";

    //    char ImgName[100];
    //    int i;
    //    for(i = 0;i < pictureName.length();i++){
    //        ImgName[i] = pictureName[i];
    //    }
    //    ImgName[i] = '\0';
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
    //    memset(read_buffer,0,sizeof(read_buffer));
    sock->async_write_some(buffer(r,r.size()),boost::bind(&CardServer::send_picture_handler,this,_1));
}
