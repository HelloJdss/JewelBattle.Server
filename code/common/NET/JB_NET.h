#pragma once
#ifndef _JB_NET_H__
#define _JB_NET_H__

#include <sys/socket.h>
#include <netinet/in.h>     //ipv4
#include <arpa/inet.h>      //address translation
#include <netdb.h>          //Name resolution
#include <cerrno>
#include <unistd.h>

#include <ctime>
#include "JB_define.h"

#ifndef NO_ERROR
#define NO_ERROR 0
#endif

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

using std::string;
class SocketAddress;
class SocketAddressFactory;
class UDPSocket;
class TCPSocket;
class SocketUtil;


class SocketAddress
{
public:
    SocketAddress();

    SocketAddress(uint32_t inAddress, uint16_t inPort);
    explicit SocketAddress(const sockaddr& inSockAddr);
    size_t GetSize() const;
private:
    friend class UDPSocket;
    friend class TCPSocket;
    sockaddr m_SockAddr;
    sockaddr_in* GetAsSockAddrIn();
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory
{
public:
    static SocketAddressPtr CreateIPV4FromString(const string& inString);
};

class UDPSocket
{
public:
    ~UDPSocket();
    int Bind(const SocketAddress& inToAddress);
    int SendTo(const void *inData, int inLen, const SocketAddress& inTo);
    int ReceiveFrom(void *inBuffer, int inLen, SocketAddress& outFrom);
    int SetNonBlockingMode(bool isShouldBeNonBlocking); //设为非阻塞模式
private:
    friend class SocketUtil;
    UDPSocket(SOCKET inSocket) : m_Socket(inSocket) { m_isBlockingMode = false; }
    SOCKET m_Socket;
    bool m_isBlockingMode;
};

typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

class TCPSocket
{
public:
    ~TCPSocket();
    int Connect(const SocketAddress& inAddress);
    int Bind(const SocketAddress& inToAddress);
    int Listen(int inBackLog = TCPCONNECTS);
    std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
    int Send(const void* inData, int inLen);
    int Receive(void* inBuffer, int inLen);

private:
    friend class SocketUtil;
    TCPSocket(SOCKET inSocket) : m_Socket(inSocket) {}
    SOCKET m_Socket;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

enum SocketAddressFamily
{
    INET    = AF_INET,
    INET6   = AF_INET6
};

class SocketUtil
{
public:
    static void ReportError(std::string inError);
    static void ReportLog(std::string inLog);
    static int GetLastError();
    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
    static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
private:
    static tm* getTimePtr();
    static std::string TimeStamp();
};

#endif