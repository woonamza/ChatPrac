#include <winsock2.h>
#include <thread>
#include <deque>
#include <mutex>
#include "PacketMessenger.h"
#include "Definition.h"

using namespace NCommon;

namespace DataContainer
{
	void PacketMessenger::Init()
	{
		if (!SetSocket())
		{
			OutputDebugString(L"Socket Setting Failed");
			return;
		}
	}

	void PacketMessenger::Release()
	{
		/* WSA Release */
		WSACleanup();
		closesocket(m_ClientSock);
	}

	RecvPacketInfo* PacketMessenger::GetPacketFromDeque()
	{
		/* Make a lockguard */
		std::lock_guard<std::mutex> lockDeque(m_Mutex);

		/* If there is no message in queue, return nullptr */
		if (m_PacketDeque.empty())
		{
			return nullptr;
		}

		/* Return front packet of the queue */
		auto frontPacket = m_PacketDeque.front();
		m_PacketDeque.pop_front();
		return frontPacket;
	}

	bool PacketMessenger::SetSocket()
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			/* WSADATA make failed */
			return false;
		}

		m_ClientSock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_ClientSock == INVALID_SOCKET)
		{
			/* Client Socket make failed */
			return false;
		}

		ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
		return true;
	}

	bool PacketMessenger::Connect()
	{
		m_ConnectState = CONNECT_STATE::CONNECTING;
		int retval = S_OK;

		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_port = htons(SERVER_INFO::serverPort);
		m_ServerAddr.sin_addr.s_addr = inet_addr(SERVER_INFO::loopBackIp.c_str());

		retval = connect(m_ClientSock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
		if (retval == SOCKET_ERROR)
		{
			/* Connect Failed */
			int err = WSAGetLastError();
			m_ConnectState = CONNECT_STATE::DISCONNECT;
			return false;
		}

		m_ConnectState = CONNECT_STATE::CONNECTED;

		/* Make Thread */
		m_RecvThread = std::thread([this]() { Update(); });
		return true;
	}

	void PacketMessenger::Update()
	{
		int len = 0;
		int retval = 0;
		char buf[COMMON_INFO::recvSize + 1];

		/* ��Ʈ��ũ ���°� CONNECTED�� ���� ���� ��ȯ */
		while (m_ConnectState == CONNECT_STATE::CONNECTED)
		{
			retval = recv(m_ClientSock, buf, COMMON_INFO::recvSize, 0);
			if (retval == SOCKET_ERROR)
			{
				/* Recv Error */
				int err = WSAGetLastError();
				break;
			}
			else if (retval == 0)
			{
				Disconnect();
			}
			LoadUpPacketToDeque(buf, retval);
		}
	}

	void PacketMessenger::LoadUpPacketToDeque(char* buf, const int bufSize)
	{
		int readPos = 0;

		/* ��� ������� �۰� ���Ҵٸ� �ǹ� ���� �����Ͷ�� �����ϰ� ���� ����. */
		while ((bufSize - readPos) > PACKET_HEADER_SIZE)
		{
			/* ��� �и� �۾� */
			PktHeader* pPacketHeader = (PktHeader*)&buf[readPos];

			readPos += PACKET_HEADER_SIZE;

			auto pPacketInfo = new RecvPacketInfo;
			pPacketInfo->PacketId = pPacketHeader->Id;
			pPacketInfo->PacketBodySize = pPacketHeader->BodySize;
			pPacketInfo->pData = new char[pPacketHeader->BodySize];
			memcpy(pPacketInfo->pData, &buf[readPos], pPacketHeader->BodySize);

			readPos += pPacketHeader->BodySize;

			// ������ ��.
			std::lock_guard<std::mutex> lockDeque(m_Mutex);
			m_PacketDeque.push_back(pPacketInfo);
		}
	}

	bool PacketMessenger::Disconnect()
	{
		if (m_ConnectState != CONNECT_STATE::DISCONNECT)
		{
			m_RecvThread.join();
			m_PacketDeque.clear();
			m_ConnectState = CONNECT_STATE::DISCONNECT;
		}
		return true;
	}

	/* ��Ŷ �޽����� Send�� ��û�� �� pData�� �˸��� ���·� �־��־�� �Ѵ�. */
	bool PacketMessenger::Send(const short packetId, const short packetSize, char* pData)
	{
		/* �α��� ��û ��Ŷ ó�� */
		if (packetId == (short)PACKET_ID::LOGIN_IN_REQ)
		{
			PktLogInReq* pNewLoginReq = (PktLogInReq*)&pData;

			char data[COMMON_INFO::MAX_PACKET_SIZE] = { 0, };

			PktHeader pktHeader{ packetId, sizeof(PktLogInReq) };
			memcpy(&data[0], (char*)&pktHeader, PACKET_HEADER_SIZE);

			if (pktHeader.BodySize > 0)
			{
				memcpy(&data[PACKET_HEADER_SIZE], (char*)&pNewLoginReq, pktHeader.BodySize);
			}

			int hr = send(m_ClientSock, data, pktHeader.BodySize + PACKET_HEADER_SIZE, 0);
			if (hr == SOCKET_ERROR)
			{
				/* Send Error */
				int err = WSAGetLastError();
				return false;
			}
		}
		return true;
	}


}