/*
 * 这是测试用的client
 */

#include <iostream>
#include "JB_net.h"


bool g_IsGameRunning = true;

int main()
{
    std::cout << "Client Start!\n";
    UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
    mySock->SetNonBlockingMode(true);

    while (g_IsGameRunning)
    {
        std::cout << "Client$: ~ ";
        std::string command;
        std::cin >> command;
        if (command == "exit")
        {
            g_IsGameRunning = false;
        }
        else if (command == "send")
        {
            std::cout << "input ip/address:port : ";
            std::cin >> command;
            SocketAddressPtr address = SocketAddressFactory::CreateIPV4FromString(command);
            while (address != nullptr )
            {
                command.clear();
                std::cout << "msg: ";
                std::cin >> command;
                if (command == "q")
                {
                    break;
                }
                mySock->SendTo(command.c_str(), command.size(), *address);
            }
        }

    }
    std::cout << "Client Stop!\n";
    return 0;
}