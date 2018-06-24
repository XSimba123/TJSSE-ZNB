#include "NetWork\SocketClient.h"  
#include "Scene\OrdinaryScene.h"

SocketClient::SocketClient(void) :
	onRecv(nullptr),
	_socektClient(0)
{

}


SocketClient::~SocketClient(void)
{
	if (_socektClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}
}

bool SocketClient::initClient()
{
	if (_socektClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}

	_socektClient = socket(AF_INET, SOCK_STREAM, 0);
	iMode = 1;
	int ret = ioctlsocket(_socektClient, FIONBIO, (u_long FAR*)&iMode);
	if (ret == SOCKET_ERROR)
	{
		log("ioctlsocket");
	}
	if (error(_socketServer))
	{
		log("init client error!");
		_socektClient = 0;
		return false;
	}
	return true;
}

bool SocketClient::connectServer(const char* serverIP, unsigned short port)
{
	if (!this->initClient())
	{
		return false;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);
	while (true)
	{
		int ret = 0;
		ret = connect(_socektClient, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
		if (SOCKET_ERROR == ret)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL)
			{
				continue;
			}
			else if(err==WSAEISCONN)
			{
				log("succeed");
				break;
			}
			/*else
			{
				log("connection failed");
				this->closeConnect(_socektClient);
				_socektClient = 0;
				return false;
			}*/
		}
	}
	std::thread recvThread(&SocketClient::recvMessage, this);
	recvThread.detach();
	return true;
}

void SocketClient::recvMessage()
{
	char recvBuf[2048];
	int ret = 0;
	while (true)
	{
		//ZeroMemory(recvBuf, 2048);
		ret = recv(_socektClient, recvBuf, sizeof(recvBuf)+1, 0);
		if (SOCKET_ERROR == ret)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				continue;
			}
			/*else
			{
				log("kwd recv error!");
				break;
			}*/
		}
		if (recvBuf[0] == 'b')
		{
			canbegin = true;
		}
		if (recvBuf[0] == 'O')
		{
			sscanf(recvBuf, "O %d", &totalNum);
		}
		if (recvBuf[0] == 'N')
		{
			if (ID == 0)sscanf(recvBuf, "N %d", &ID);
		}
		if (recvBuf[0] == '1')
		{
			int i;
			sscanf(recvBuf, "1 %d", &i);
			if (!heartiseaten[i])heartiseaten[i] = true;
		}
		/*else if (recvBuf[0] == 'X')
		{
			int a[500];
			int num;
			sscanf(recvBuf, "X %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
				,&num, &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15], &a[16], &a[17], &a[18], &a[19] ,&a[20],
				&a[21], &a[22], &a[23], &a[24], &a[25], &a[26], &a[27], &a[28], &a[29], &a[30], &a[31], &a[32], &a[33], &a[34], &a[35], &a[36], &a[37], &a[38], &a[39], &a[40],
				&a[41], &a[42], &a[43], &a[44], &a[45], &a[46], &a[47], &a[48], &a[49], &a[50], &a[51], &a[52], &a[53], &a[54], &a[55], &a[56], &a[57], &a[58], &a[59], &a[60],
				&a[61], &a[62], &a[63], &a[64], &a[65], &a[66], &a[67], &a[68], &a[69], &a[70], &a[71], &a[72], &a[73], &a[74], &a[75], &a[76], &a[77], &a[78], &a[79], &a[80],
				&a[81], &a[82], &a[83], &a[84], &a[85], &a[86], &a[87], &a[88], &a[89], &a[90], &a[91], &a[92], &a[93], &a[94], &a[95], &a[96], &a[97], &a[98], &a[99], &a[100],
				&a[101], &a[102], &a[103], &a[104], &a[105], &a[106], &a[107], &a[108], &a[109], &a[110], &a[111], &a[112], &a[113], &a[114], &a[115], &a[116], &a[117], &a[118], &a[119], &a[120],
				&a[121], &a[122], &a[123], &a[124], &a[125], &a[126], &a[127], &a[128], &a[129], &a[130], &a[131], &a[132], &a[133], &a[134], &a[135], &a[136], &a[137], &a[138], &a[139], &a[140],
				&a[141], &a[142], &a[143], &a[144], &a[145], &a[146], &a[147], &a[148], &a[149], &a[150], &a[151], &a[152], &a[153], &a[154], &a[155], &a[156], &a[157], &a[158], &a[159], &a[160],
				&a[161], &a[162], &a[163], &a[164], &a[165], &a[166], &a[167], &a[168], &a[169], &a[170], &a[171], &a[172], &a[173], &a[174], &a[175], &a[176], &a[177], &a[178], &a[179], &a[180],
				&a[181], &a[182], &a[183], &a[184], &a[185], &a[186], &a[187], &a[188], &a[189], &a[190], &a[191], &a[192], &a[193], &a[194], &a[195], &a[196], &a[197], &a[198], &a[199], &a[200],
				&a[201], &a[202], &a[203], &a[204], &a[205], &a[206], &a[207], &a[208], &a[209], &a[210], &a[211], &a[212], &a[213], &a[214], &a[215], &a[216], &a[217], &a[218], &a[219], &a[220],
				&a[221], &a[222], &a[223], &a[224], &a[225], &a[226], &a[227], &a[228], &a[229], &a[230], &a[231], &a[232], &a[233], &a[234], &a[235], &a[236], &a[237], &a[238], &a[239], &a[240],
				&a[241], &a[242], &a[243], &a[244], &a[245], &a[246], &a[247], &a[248], &a[249], &a[250], &a[251], &a[252], &a[253], &a[254], &a[255], &a[256], &a[257], &a[258], &a[259], &a[260],
				&a[261], &a[262], &a[263], &a[264], &a[265], &a[266], &a[267], &a[268], &a[269], &a[270], &a[271], &a[272], &a[273], &a[274], &a[275], &a[276], &a[277], &a[278], &a[279], &a[280],
				&a[281], &a[282], &a[283], &a[284], &a[285], &a[286], &a[287], &a[288], &a[289], &a[290], &a[291], &a[292], &a[293], &a[294], &a[295], &a[296], &a[297], &a[298], &a[299], &a[300], 
				&a[301], &a[302], &a[303], &a[304], &a[305], &a[306], &a[307], &a[308], &a[309], &a[310], &a[311], &a[312], &a[313], &a[314], &a[315], &a[316], &a[317], &a[318], &a[319], &a[320],
				&a[321], &a[322], &a[323], &a[324], &a[325], &a[326], &a[327], &a[328], &a[329], &a[330], &a[331], &a[332], &a[333], &a[334], &a[335], &a[336], &a[337], &a[338], &a[339], &a[340],
				&a[341], &a[342], &a[343], &a[344], &a[345], &a[346], &a[347], &a[348], &a[349], &a[350], &a[351], &a[352], &a[353], &a[354], &a[355], &a[356], &a[357], &a[358], &a[359], &a[360],
				&a[361], &a[362], &a[363], &a[364], &a[365], &a[366], &a[367], &a[368], &a[369], &a[370], &a[371], &a[372], &a[373], &a[374], &a[375], &a[376], &a[377], &a[378], &a[379], &a[380],
				&a[381], &a[382], &a[383], &a[384], &a[385], &a[386], &a[387], &a[388], &a[389], &a[390], &a[391], &a[392], &a[393], &a[394], &a[395], &a[396], &a[397], &a[398], &a[399], &a[400], 
			    &a[401], &a[402], &a[403], &a[404], &a[405], &a[406], &a[407], &a[408], &a[409], &a[410], &a[411], &a[412], &a[413], &a[414], &a[415], &a[416], &a[417], &a[418], &a[419], &a[420],
				&a[421], &a[422], &a[423], &a[424], &a[425], &a[426], &a[427], &a[428], &a[429], &a[430], &a[431], &a[432], &a[433], &a[434], &a[435], &a[436], &a[437], &a[438], &a[439], &a[440],
				&a[441], &a[442], &a[443], &a[444], &a[445], &a[446], &a[447], &a[448], &a[449], &a[450], &a[451], &a[452], &a[453], &a[454], &a[455], &a[456], &a[457], &a[458], &a[459], &a[460],
				&a[461], &a[462], &a[463], &a[464], &a[465], &a[466], &a[467], &a[468], &a[469], &a[470], &a[471], &a[472], &a[473], &a[474], &a[475], &a[476], &a[477], &a[478], &a[479], &a[480],
				&a[481], &a[482], &a[483], &a[484], &a[485], &a[486], &a[487], &a[488], &a[489], &a[490], &a[491], &a[492], &a[493], &a[494], &a[495], &a[496], &a[497], &a[498], &a[499]);
			for (int i = 0; i < num; i++)
			{
				if (!dotiseaten[a[i]])dotiseaten[a[i]] = true;
			}
		}*/
		/*else if (recvBuf[0] == 'H')
		{
			int a[36];
			int num;
			sscanf(recvBuf, "X %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
				, &num, &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15], &a[16], &a[17], &a[18], &a[19], &a[20],
				&a[21], &a[22], &a[23], &a[24], &a[25], &a[26], &a[27], &a[28], &a[29], &a[30], &a[31], &a[32], &a[33], &a[34], &a[35]);
			for (int i = 0; i < num; i++)
			{
				log("%d", a[i]);
				if (!heartiseaten[a[i]])heartiseaten[a[i]] = true;
			}
		}*/
		else if (recvBuf[0] == '2')
		{
			int i;
			sscanf(recvBuf, "2 %d", &i);
			if (!dotiseaten[i])dotiseaten[i] = true;
		}
		else if (recvBuf[0] == 'C'&&recvBuf[1] == 'P')
		{
			float HP;
			int level;
			sscanf(recvBuf, "CP %f,%f,%f,%d,%d", &HP ,&EnemyPos.x, &EnemyPos.y,&level,&enemy->killNum);
			if (this->enemy->get_level() < level)enemy->EnemyaddLevel();
			enemy->set_HP(HP);
		}
		else if (recvBuf[0] == 'C'&&recvBuf[1] == 'R')
		{
			sscanf(recvBuf, "CR %f", &EnemyDir);
		}
		else if (recvBuf[0] == 'A')
		{
			sscanf(recvBuf, "A %d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", &enemyfrontnum, &enemybesidenum, &enemybacknum,
				&FrontDirection.x, &FrontDirection.y, &FrontLeftDirection.x, &FrontLeftDirection.y, &FrontRightDirection.x, &FrontRightDirection.y,
				&BackDirection.x, &BackDirection.y, &BackLeftDirection.x, &BackLeftDirection.y, &BackRightDirection.x, &BackRightDirection.y,
				&BesideDirection.x, &BesideDirection.y, &BesideFrontDirection.x, &BesideFrontDirection.y, &BesideBackDirection.x, &BesideBackDirection.y, &rotateDegrees);
			enemyShootTime++;
		}
		else if (recvBuf[0] == 'E'&&recvBuf[1] == 'A')
		{
			beAttackenTime++;
		}
		else if (recvBuf[0] == 'E'&&recvBuf[1] == 'D')
		{
			enemy->setAlreadyDead();
		}
		else if (recvBuf[0] == 'E'&&recvBuf[1] == 'C')
		{
			enemy->setcomeback();
		}
		else if (recvBuf[0] == 'E'&&recvBuf[1] == 'G')
		{
			enemy->add_HP(1);
		}

		else if (recvBuf[0] == 'K')
		{
			skillID id;
			sscanf(recvBuf, "K %d", &id);
			if (id == useCrossBow)change = 1;
			else if (id == useGun)change = 2;
			else if (id == useFireBall)change = 3;
			enemy->getSkill(id);
		}
		else if (recvBuf[0] == 'C'&&recvBuf[1] == 'N')
		{
			sscanf(recvBuf, "CN %s", playname);
			log("%s\n%s", recvBuf,playname);
		}

	}
	this->closeConnect(_socektClient);
	if (onDisconnect != nullptr)
	{
		onDisconnect();
	}
	_socektClient = 0;
	_mutex.unlock();
}

void SocketClient::sendMessage(const char* data, int count)
{
	if (_socektClient != 0)
	{
		while (true)
		{
			int ret = send(_socektClient, data, count, 0);
			if (SOCKET_ERROR == ret)
			{
				int err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK)
				{
					continue;
				}
				/*else
				{
					log("kwq send error!");
					break;
				}*/
			}
			break;
		}
	}
}