#ifndef __SOCKET_BASE_H__  
#define __SOCKET_BASE_H__  
#include "cocos2d.h"  
#include <list>  
#include <thread>  
USING_NS_CC;
// 对于windows平台  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#include <WinSock2.h>  
#pragma comment(lib, "WS2_32.lib")  
#define HSocket SOCKET  
// 对于android平台  
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <arpa/inet.h>    // for inet_**  
#include <netdb.h>    // for gethost**  
#include <netinet/in.h>   // for sockaddr_in  
#include <sys/types.h>    // for socket  
#include <sys/socket.h>   // for socket  
#include <unistd.h>  
#include <stdio.h>         // for printf  
#include <stdlib.h>   // for exit  
#include <string.h>   // for bzero  
#define HSocket int  
#endif   

class SocketBase : public Ref
{
public:
	SocketBase()
	{
		_bInitSuccess = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(2, 0);
		WSADATA wsaData;
		int nRet = WSAStartup(wVersionRequested, &wsaData);
		if (nRet != 0)
		{
			fprintf(stderr, "Initilize Error!\n");
			return;
		}
		_bInitSuccess = true;
#endif  
	}
	~SocketBase()
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		if (_bInitSuccess)
		{
			WSACleanup();
		}
#endif   
	}

protected:
	void closeConnect(HSocket socket)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		close(socket);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		closesocket(socket);
#endif  
	}
	bool error(HSocket socket)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		return socket == SOCKET_ERROR;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		return socket < 0;
#endif  
	}

protected:
	std::mutex _mutex;

private:
	bool _bInitSuccess;
};

#endif  