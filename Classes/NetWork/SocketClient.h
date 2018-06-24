#ifndef __SOCKET_CLIENT_H__  
#define __SOCKET_CLIENT_H__  
#include "NetWork\SocketBase.h"  
#include "Model\character.h"
class SocketClient : public SocketBase
{
public:
	SocketClient(void);
	~SocketClient(void);
	int totalNum = 0;
	bool connectServer(const char* serverIP, unsigned short port);
	void sendMessage(const char* data, int count);
	
	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;
	bool heartiseaten[36] = { false };
	bool dotiseaten[500] = { false };
	Vec2 EnemyPos=Vec2(3000, 3000);
	float EnemyDir = 0;
	Vec2 FrontDirection;
	Vec2 FrontLeftDirection;
	Vec2 FrontRightDirection;
	Vec2 BackDirection;
	Vec2 BackLeftDirection;
	Vec2 BackRightDirection;
	Vec2 BesideDirection;
	Vec2 BesideFrontDirection;
	Vec2 BesideBackDirection;
	int enemyfrontnum;
	int enemybesidenum;
	int enemybacknum;
	float rotateDegrees;
	Sprite* enemyarrow[3][4];
	PhysicsBody* enemyarrowbody[3][4];
	Character* enemy;
	int enemyShootTime=0;
	int beAttackenTime = 0;
	int iMode = 0;
	bool canbegin=false;
	int ID=0;
	int change=0;
	bool boardchange = false;
	bool enemybuilt=false;
	char playname[20] = {0};
	bool addkill;
private:
	bool initClient();
	void recvMessage();

private:
	HSocket _socketServer;
	HSocket _socektClient;
};






#endif  