#include "chat_server.h"


//传入IO服务，然后创建一个UDP的SOCKET，IPV4版本，端号为6889
Chat_Server::Chat_Server(boost::asio::io_service& ioService)
    :m_sock(ioService,  udp::endpoint(udp::v4(), 6889))
{
    std::cout << "chat Server start" << std::endl;

    //进入接收服务中。
    RecvQuest();
}

//接收收客户端的请求。
void Chat_Server::RecvQuest(void)
{
    //异步接收数据
    m_sock.async_receive_from(
                boost::asio::buffer(m_rbuf), m_endpointRemote,
                boost::bind(&Chat_Server::handleRecvQuest, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

//当收到客户端数据时，就进入本函数响应处理
void Chat_Server::handleRecvQuest(const boost::system::error_code& error,
                                  std::size_t /*bytes_transferred*/)
{
    //如果没有出错，就处理请求。
    if (!error || error == boost::asio::error::message_size)
    {
        std::string s;
        int length = 0;
        for(int i = 0;i != 1024;i++) {
            if(m_rbuf[i] != '\0')
                length = i;
        }
        for(int i = 0;i != length + 1;i++) {
            s.push_back(m_rbuf[i]);
        }
        std::cout << "quest:" << s << std::endl;
        std::istringstream record(s);
        std::string work;
        record >> work;
        if(work == "login")  {
            std::string username;
            std::string passwd;
            record >> username;
            record >>passwd;
            if (!dao.user_login(username,passwd)) {
                std::string messages = "login faild";
                std::cout << messages << std::endl;
                m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote,            boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error,            boost::asio::placeholders::bytes_transferred));
            }
            else {
                std::cout << "receivefrom: " << m_endpointRemote.address().to_string() << ":" << m_endpointRemote.port() << std::endl;
                user u;
                u.username = username;
                u.IPaddress = m_endpointRemote.address().to_string();
                u.port = m_endpointRemote.port();
                users.push_back(u);
                std::string messages = "login suc ";
                messages += dao.handle_loginsuc(username);
                std::cout << messages << std::endl;

                m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote, boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }

        }
        else if(work == "getfriendsMes")  {
            std::string username;
            record >> username;

            std::string messages = "friendsMes ";
            messages += dao.handle_loginsuc(username);
            std::cout << messages << std::endl;

            m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote, boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


        }
        else if(work == "register") {
            std::string username;
            std::string passwd;
            record >> username >> passwd;
            if (dao.user_register(username,passwd)) {
                std::string messages = "register suc";
                std::cout << messages << std::endl;
                m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote,            boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error,            boost::asio::placeholders::bytes_transferred));
            }   else {
                std::string messages = "register faild";
                std::cout << messages << std::endl;
                m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote,            boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error,            boost::asio::placeholders::bytes_transferred));
            }
        }
        else if(work == "sendMessage") {
            std::string sender;
            std::string receiver;
            std::string message;
            record >> sender >> receiver >> message;
            if (message != "") {
                if (dao.handle_send_message(sender,receiver,message)) {
                    std::cout << "send ok" << std::endl;
                }
                for (int i = 0; i < users.size(); i++) {
                    if (users[i].username == receiver) {
                        udp::endpoint ep(boost::asio::ip::address::from_string(users[i].IPaddress.c_str()), users[i].port);
                        std::string messages = "NewMes "+sender+" "+receiver+" "+message;

                        std::cout << messages << std::endl;
                        m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), ep,            boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error,            boost::asio::placeholders::bytes_transferred));
                    }
                }
            }
        }
        else if (work == "receiveChatLog") {
            std::string username;
            std::string friendname;
            std::string way;
            record >> username >> friendname >> way;
            std::string messages = "ChatLog "+friendname+" +++ ";
            messages += dao.handle_receive_chatlog(username,friendname,way);
            m_sock.async_send_to(boost::asio::buffer(messages,messages.size()), m_endpointRemote,            boost::bind(&Chat_Server::handleSend, this, boost::asio::placeholders::error,            boost::asio::placeholders::bytes_transferred));
        }

        //接收下一次的信息。
        RecvQuest();
    }
}

//当发送时间字符串给客户端成功之后响应。
void Chat_Server::handleSend(const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/)
{

}
