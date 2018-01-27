#include "JB_define.h"
#include "JB_net.h"

int main()
{
    std::cout << "GameServer Start!\n";

    UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
    mySock->SetNonBlockingMode(true);
    SocketAddressPtr address = SocketAddressFactory::CreateIPV4FromString("127.0.0.1:5277");
    mySock->Bind(*address);
    bool run = true;
    while (run)
    {
        char data[1000];
        SocketAddress socketAddress;
        int bytesReceived = mySock->ReceiveFrom(data, sizeof(data), socketAddress);
        if (bytesReceived > 0)
        {
            SocketUtil::ReportLog("recvmsg: " + std::string(data));
        }
        else if (bytesReceived < 0)
        {
            run = false;
        }
        //other
    }
    return 0;
}


