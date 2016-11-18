#pragma once   

#include "SMsg.h"
#include "Entity.h"
#include "ObjectMgr.h"
#include "GameLogicMgr.h"

static std::vector<std::string> s_vecObjTypeTable =
{
	{ "Player" },
	{ "OtherPlayer"},
	{ "Bat"},
	{ "Skeleton"},
	{ "Zombi"},
	{ "Ghost"},
	{ "Mushroom"},
	{ "Mage"},
	{ "Goblin"},
	{ "Minota"},
	{ "Dragon"},
	{ "PlayerAttackBox"},
	{ "PlayerEnemyBox"},
	{ "WarningBox"},
	{ "SpeedItem"},
	{ "HpItem"},
	{ "AttItem"},
	{ "TriCellItem"},
	{ "FourCellItem"},
	{ "RapierItem"},
	{ "ShotGunItem"},
	{ "RifleItem"},
	{ "Cursor"},
};

enum class PACKET_ID : unsigned short
{
	PACKET_IS_HOST,
	PACKET_IS_LOGIN_REQ,
	PACKET_NEW_PLAYER,
	PACKET_USER_INIT,
	PACKET_NEW_OBJ,
	PACKET_HOST_OBJ_INFO,
	PACKET_SEND_STAT_INFO,
	PACKET_SEND_POS_INFO,
	PACKET_MOVE,
	PACKET_ATT,
};

struct SPacketBase
{
	unsigned short uSize;
	unsigned short uID;
};

struct SPacketNewPlayer : public SPacketBase
{
	float x;
	float y;
};

struct SPacketMove : public SPacketBase
{
	float x;
	float y;
};

struct SPacketMoveTo : public SPacketBase
{
	float x;
	float y;
	float xTo;
	float yTo;
};

struct SPacketAttack : public SPacketBase
{
	float x;
	float y;
	int attack_type;
};


class Session
{
public:
	Session()
		:m_socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	{
		m_hEvent = WSACreateEvent();
	}
	~Session()
	{
		WSACloseEvent(m_hEvent);

		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
	}

public:
	bool Connect(const char* strAddress, unsigned short pPort)
	{
		SOCKADDR_IN ServerAddress;
		ServerAddress.sin_family = AF_INET;
		ServerAddress.sin_port = ::htons(pPort);
		ServerAddress.sin_addr.S_un.S_addr = inet_addr(strAddress);

		int iResult = connect(m_socket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
		iResult = WSAEventSelect(m_socket, m_hEvent, FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE );

		return true;
	}

	bool Send(const void* pBuf, size_t szBuf) const
	{
		auto bytesSend = send(m_socket, reinterpret_cast<const char*>(pBuf), szBuf, 0);
		if (bytesSend == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			return false;
		}
		return bytesSend == szBuf;
	}

	bool Send( const SMsgSend& msg ) const
	{
		return Send(msg.GetData(), msg.GetSize());
	}
	
	void Update()
	{
		int iResult = WSAWaitForMultipleEvents(1, &m_hEvent, FALSE, 1, FALSE);
		if (iResult == WSA_WAIT_EVENT_0)
		{
			WSANETWORKEVENTS event;
			event.lNetworkEvents = 0;

			auto ret = WSAEnumNetworkEvents(m_socket, m_hEvent, &event);

			if (event.lNetworkEvents && FD_CLOSE == FD_CLOSE)
				OnClose();
			if (event.lNetworkEvents && FD_CONNECT)
				OnConnect();
			if (event.lNetworkEvents && FD_READ)
				OnRecieve();
			if (event.lNetworkEvents && FD_WRITE)
				OnSend();
		}
	}
	void OnConnect()
	{
		OutputDebugStringA("OnConnect\n");
	}
	void OnClose()
	{
		OutputDebugStringA("OnClose\n");
	}
	void OnRecieve()
	{
		auto bufs = m_netBuf.get_empty_buf();

		auto received = recv(m_socket, reinterpret_cast<char *>(bufs[0].first), bufs[0].second, 0 );
		if (received == bufs[0].second && bufs.size() > 1)
		{
			auto additional_received = recv(m_socket, reinterpret_cast<char *>(bufs[1].first), bufs[1].second, 0 );
			received += additional_received;
		}
		if (received > 0)
		{
			m_netBuf.commit_recv(received);

			while (m_netBuf.available_data() >= 4)
			{
				SMsgRecv msg(&m_netBuf);

				if (!msg.is_valid())
					break;

				switch (static_cast<PACKET_ID>(msg.GetID()))
				{
				case PACKET_ID::PACKET_IS_HOST:
				{
					bool bIsHost;
					msg >> bIsHost;
					if (bIsHost)
						CGameLogicMgr::getSingleton()->SetHost();
					break;
				}
				case PACKET_ID::PACKET_IS_LOGIN_REQ:
				{
					SMsgSend msg(PACKET_ID::PACKET_IS_LOGIN_REQ);
					Send(msg);
					break;
				}
				case PACKET_ID::PACKET_NEW_PLAYER:
				{
					ENTITY_TYPE eType;
					float fX;
					float fY;

					msg >> eType >> fX >> fY;

					CGameLogicMgr::getSingleton()->CreateObject(s_vecObjTypeTable[static_cast<int>(eType)], D2D1::Point2F(fX, fY));

					break;
				}
				case PACKET_ID::PACKET_USER_INIT:
				{
					unsigned iID;
					ENTITY_TYPE eType;
					float fX;
					float fY;


					msg >> iID >> eType >> fX >> fY;
					CGameLogicMgr::getSingleton()->CreateObject(s_vecObjTypeTable[static_cast<int>(eType)], D2D1::Point2F(fX, fY));

					auto IsEntity = [=](EntityPtr pEntity)
					{
						if (pEntity->GetEntityID() == iID)
						{
							pEntity->Do("SendStatInfoToSever");
							pEntity->Do("SendPosObjInfo");
						}
					};
					CObjectMgr::getSingleton()->for_each_Addentities(IsEntity);

					break;
				}
				case PACKET_ID::PACKET_NEW_OBJ:
				{
					ENTITY_TYPE eType;
					float fX;
					float fY;

					msg >> eType >> fX >> fY;

					CGameLogicMgr::getSingleton()->CreateObject(s_vecObjTypeTable[static_cast<int>(eType)], D2D1::Point2F(fX, fY));
					break;
				}
				case PACKET_ID::PACKET_MOVE:
				{
					unsigned iID;
					float fX;
					float fY;
					float xTo;
					float yTo;

					msg >> iID >> fX >> fY >> xTo >> yTo;

					auto IsEntity = [=](EntityPtr pEntity)
					{
						if (pEntity->GetEntityID() == iID)
						{
							pEntity->SetAttr("Pos", D2D1::Point2F(fX, fY));
							pEntity->Do("MovePacket", D2D1::Point2F(xTo, yTo));
						}
					};

					CObjectMgr::getSingleton()->for_each_entities(IsEntity);

					break;
				}
				}
				msg.done_msg();
			}
		}
		OutputDebugStringA("OnRecieve\n");
	}
	void OnSend()
	{
		OutputDebugStringA("OnSend\n");
	}

private:
	SOCKET m_socket;
	WSAEVENT m_hEvent;
	SNetBuf m_netBuf;
};