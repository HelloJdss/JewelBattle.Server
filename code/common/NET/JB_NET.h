#pragma once
#ifndef _JB_NET_H__
#define _JB_NET_H__

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>     //ipv4
#include <arpa/inet.h>      //address translation
#include <netdb.h>          //Name resolution
#endif

#include "JB_define.h"

using std::string;

class SocketAddress
{
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
#ifdef  _MAC
        GetAsSockAddrIn()->sin_len = sizeof(sockaddr_in);     
#endif
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }
    SocketAddress(const sockaddr& inSockAddr)
    {
        memcpy(&m_SockAddr, &inSockAddr, sizeof(sockaddr));
    }

    size_t GetSize() const { return sizeof(sockaddr); }
private:
    sockaddr m_SockAddr;
    sockaddr_in* GetAsSockAddrIn() 
    {
        return reinterpret_cast<sockaddr_in*>( &m_SockAddr );
    }
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory
{
public:
    static SocketAddressPtr CreateIPV4FromString(const string& inString)
    {
        auto pos = inString.find_last_of(":");
        string host, service;
        if(pos != string::npos)
        {
            host = inString.substr(0, pos);
            service = inString.substr(pos + 1);
        }
        else
        {
            host = inString;
            service = "0";  //use default
        }
        addrinfo hint;
        memset(&hint, 0 ,sizeof(hint));
        hint.ai_family = AF_INET;
        
        addrinfo* result;
        int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

        if(error != 0 && result != nullptr)
        {
            freeaddrinfo(result);
            return nullptr;
        }

        while(!result->ai_addr && result->ai_next)
        {
            result = result->ai_next;
        }

        if(!result->ai_addr)
        {
            freeaddrinfo(result);
            return nullptr;
        }

        auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

        freeaddrinfo(result);

        return toRet;
    }
};
#endif