#include <iostream>
#include "chat_server.h"

using namespace std;

void TestUdp(void);

int main()
{
    TestUdp();
    cin.get();
    return 0;
}


void TestUdp(void)
{
    boost::asio::io_service ioService;
    Chat_Server chatServer(ioService);
    ioService.run();
}
