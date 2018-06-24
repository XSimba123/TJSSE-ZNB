#ifndef __SOCKET_SERVER_H__  
#define __SOCKET_SERVER_H__  

#include "NetWork\SocketBase.h"  

class SocketServer : public SocketBase
{
public:
	static SocketServer* create();

	SocketServer();
	~SocketServer();
	int totalNum = 0;
	bool startServer();
	void sendMessage(HSocket socket, const char* data, int count);
	void sendMessage(const char* data, int count);

	std::function<void(const char* ip)> onStart;
	std::function<void(HSocket socket)> onNewConnection;
	std::function<void(const char* data, int count)> onRecv;
	std::function<void(HSocket socket)> onDisconnect;
	int iMode = 1;
	void SocketServer::close();
private:
	bool initServer();
	void acceptClient();
	void acceptFunc();
	void newClientConnected(HSocket socket);
	void recvMessage(HSocket socket);

private:
	HSocket _socketServer;

private:
	std::list<HSocket> _clientSockets;

};

#endif  