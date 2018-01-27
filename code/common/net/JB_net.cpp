
#include <cstdint>
#include <fcntl.h>
#include "JB_net.h"

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
    GetAsSockAddrIn()->sin_len = sizeof(sockaddr_in);
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr &inSockAddr)
{
    memcpy(&m_SockAddr, &inSockAddr, sizeof(sockaddr));
}

sockaddr_in *SocketAddress::GetAsSockAddrIn()
{
    return reinterpret_cast<sockaddr_in*>( &m_SockAddr );
}

size_t SocketAddress::GetSize() const
{
    return sizeof(sockaddr);
}

SocketAddress::SocketAddress()
{

}

SocketAddressPtr SocketAddressFactory::CreateIPV4FromString(const string &inString)
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

void SocketUtil::ReportError(const std::string inError)
{
    std::cerr << "\033[31m" << TimeStamp() << "[ERROR]: " << inError << std::endl << "\033[0m";
}

tm *SocketUtil::getTimePtr() {
    static time_t the_time;
    time(&the_time);
    static struct tm *tm_ptr = localtime(&the_time);
    return tm_ptr;
}

std::string SocketUtil::TimeStamp() {
    static std::string str;
    str.clear();
    tm * t = getTimePtr();
    char c[50] = {0};
    sprintf(c, "[%04d-%02d-%02d %02d:%02d:%02d]",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    str.append(c);
    return str;
}

void SocketUtil::ReportLog(const std::string inLog)
{
    std::clog << TimeStamp() << "[LOG]: " << inLog << std::endl;
}

int SocketUtil::GetLastError()
{
    ReportError(strerror(errno));
    return errno;
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
    SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (s != INVALID_SOCKET)
    {
        return UDPSocketPtr(new UDPSocket(s));
    }
    else
    {
        ReportError("SocketUtil::CreateUDPSocket");
        return nullptr;
    }
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily) {
    SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_TCP);
    if (s != INVALID_SOCKET)
    {
        return TCPSocketPtr(new TCPSocket(s));
    }
    else
    {
        ReportError("SocketUtil::CreateTCPSocket");
        return nullptr;
    }
}

UDPSocket::~UDPSocket()
{
    close(m_Socket);
}

int UDPSocket::Bind(const SocketAddress &inToAddress)
{
    int err = bind(m_Socket, &inToAddress.m_SockAddr, inToAddress.GetSize());
    if (err != 0)
    {
        SocketUtil::ReportError("UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int UDPSocket::SendTo(const void *inData, int inLen, const SocketAddress &inTo)
{
    int byteSentCount = sendto(m_Socket, static_cast<const char *>(inData),
                               inLen, 0, &inTo.m_SockAddr, inTo.GetSize());
    if (byteSentCount >= 0)
    {
        return byteSentCount;
    }
    else
    {
        SocketUtil::ReportError("UDPSocket::SendTo");
        return -SocketUtil::GetLastError();
    }
}

int UDPSocket::ReceiveFrom(void *inBuffer, int inLen, SocketAddress &outFrom)
{
    socklen_t fromLength = outFrom.GetSize();
    int readByteCount = recvfrom(m_Socket, static_cast<char *>(inBuffer),
                                                  inLen, 0, &outFrom.m_SockAddr, &fromLength);
    if (readByteCount >= 0)
    {
        return readByteCount;
    }
    else {
        if (!(m_isBlockingMode && errno == EAGAIN))    //忽略非阻塞调用下的警告
        {
            SocketUtil::ReportError("UDPSocket::ReceiveFrom");
            return -SocketUtil::GetLastError();
        }
        return 0;
    }
}

int UDPSocket::SetNonBlockingMode(bool isShouldBeNonBlocking)
{
#if _WIN32
    auto arg = isShouldBeNonBlocking ? 1 : 0;
    int result = ioctlsocket(m_Socket, FIONBIO, &arg);
#else
    int flags = fcntl(m_Socket, F_GETFL, 0);
    flags = isShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    int result = fcntl(m_Socket, F_SETFL, flags);
#endif

    if (result == SOCKET_ERROR)
    {
        SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
        return SocketUtil::GetLastError();
    }
    else
    {
        m_isBlockingMode = true;
        return NO_ERROR;
    }
}

TCPSocket::~TCPSocket()
{
    close(m_Socket);
}

int TCPSocket::Connect(const SocketAddress &inAddress)
{
    int err = connect(m_Socket, &inAddress.m_SockAddr, inAddress.GetSize());
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Connect");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress &inToAddress)
{
    int err = bind(m_Socket, &inToAddress.m_SockAddr, inToAddress.GetSize());
    if (err != 0)
    {
        SocketUtil::ReportError("TCPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog)
{
    int err = listen(m_Socket, inBackLog);
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Listen");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress &inFromAddress)
{
    int length = static_cast<int>(inFromAddress.GetSize());
    SOCKET newSocket = accept(m_Socket, &inFromAddress.m_SockAddr, reinterpret_cast<socklen_t *>(&length));
    if (newSocket != INVALID_SOCKET)
    {
        return TCPSocketPtr(new TCPSocket(newSocket));
    }
    else
    {
        SocketUtil::ReportError("TCPSocket::Accept");
        return nullptr;
    }
}

int TCPSocket::Send(const void *inData, int inLen)
{
    int byteSentCount = send(m_Socket, static_cast<const char*>(inData), inLen, 0);
    if (byteSentCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Send");
        return -SocketUtil::GetLastError();
    }
    return byteSentCount;
}

int TCPSocket::Receive(void *inBuffer, int inLen)
{
    int ByteReceivedCount = recv(m_Socket, static_cast<char*>(inBuffer), inLen, 0);
    if (ByteReceivedCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Receive");
        return -SocketUtil::GetLastError();
    }
    return ByteReceivedCount;
}

