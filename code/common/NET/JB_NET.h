#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>     //ipv4
#include <arpa/inet.h>      //address translation
#include <netdb.h>          //Name resolution
#endif
