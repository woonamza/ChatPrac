 
/*#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <string>
#include "Network.h"

//using namespace std;

using namespace LogicLib;

bool ClientNetwork::Init()//������ ��� ������, ������ ������
{
	//������ ���(����) ������(�ʱ�ȭ)
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		return false;
	}

	//������(����) �����,������(�ʱ�ȭ)
	m_Clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Clientsock == INVALID_SOCKET)
	{
		return false;
	}

	return true;
};

void ClientNetwork::Close()//������(����) ���ֱ�,������
{
	if (m_Status == NetworkStatus::Connect)
	{
		closesocket(m_Clientsock);
		m_Status = NetworkStatus::Disconnect;
	}

}

void ClientNetwork::Release()//������ ���(����) ������
{
	WSACleanup();
};

bool ClientNetwork::Connect()//������ �����ϰ� ����
{
	struct sockaddr_in sockinfo;

	if (connect(m_Clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
	{
		return false;
	}
	
	//m_SplitThread =, ������ �޾ƾ� �� �� ����ϴ� ����(������)�̹Ƿ� ������Ʈ ��ɿ��� ������ �ϴ� �� �ƴұ�?
	
	return true;
};

void ClientNetwork::Send()//������ ���� ������ ����
{
	int Retval;
	int len;

	while (1) {
		ZeroMemory(Buff, sizeof(Buff));
		if (fgets(Buff, BUFFSIZE + 1, stdin) == NULL)
			break;

		len = strlen(Buff);
		if (Buff[len - 1] == '\n')
			Buff[len - 1] == '\0';
		if (strlen(Buff) == 0)
			break;

		Retval = send(m_Clientsock, buf, strlen(Buff), 0);
		if (Retval == SOCKET_ERROR) {
			//err_display("send()");
			break;
		}
	}

};

//������ ���� ������ ��� �޾Ƶ鿩�� �Ѵ�, �ٸ� ������ �ϴ��� ������ ������ ���� �� ���� �޾Ƶ鿩�� �Ѵ�
//�׷��ϱ� (thread()��) ���� ����� (while������) ��� (recv()��) �޴´�?
//������ ���������� ���������� �̷�����ִµ� ���������� �����Ѵ�, ������ ���������� ���������� ������ ���������� ���� �Ѵ�

//��Ŷ �����Ϸ���, ��� �����ϰ�, �׷�����(�������°�(�����Ϸ��°�) �ٵ��̹Ƿ� '�ٵ� �����ϰ� �׷����� ����� �����Ѵ�' �ƴұ�?) �ٵ� �����Ѵ�
void ClientNetwork::Update()//������ ���� ���� �޾� ���̰� ����
{
	int Retval;
	int bodysize;
	//char Buff[BUFFSIZE + 1];

	m_SplitThread([m_Clientsock, Buff]()
	{
		while (1) {

			if (Retval == 0) {
				break;
			}
			//recv�� ��ȯ���� ���� ����Ʈũ���̴�.
			Retval = recv(m_Clientsock, Buff, sizeof(short), 0);//���id �д´�. ���ż������� ���Ź��ۿ� ����(�����ܿ��� ó���ϰ���?)�� �����͸� ������ ������ �����Ѵ�
			Retval = recv(m_Clientsock, Buff+1, sizeof(short), 0);//���bodysize �д´�
			bodysize = atoi(Buff[Retval]);//��ŭ �о�� �ϴ°�?
			Retval = recv(m_Clientsock, Buff+2, bodysize, 0);//bodysize��ŭ �д´�
			Buff[retval] = '\0';//���� ������ ����ϰ�ʹٸ�, ���� ������ �ڿ� null���ڸ� �־���Ѵ�
			printf("%s\n", Buff);
			
		}
	});//������� ��� �޴´�! ����� ���� �ٵ� ��´�?!



	//Retval = ;

};*/
//�ڵ带 ������ �־�� �Ѵ�!!
//�κ񸮽�Ʈ���� ����

#include "Network.h"
#include "Definition.h"
#include "Util.h"

namespace ClientLogic
{
	void ClientNetwork::Init()
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			return;
		}

		m_ClientSock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_ClientSock == INVALID_SOCKET)
		{
			ErrQuit("Connect()");
		}

		ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));

	}

	void ClientNetwork::Release()
	{
		WSACleanup();
	}


	bool ClientNetwork::Connect()
	{
		m_ConnectState = CONNECT_STATE::CONNECTING;
		int retval;

		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_port = htons(SERVER_INFO::serverPort);
		m_ServerAddr.sin_addr.s_addr = inet_addr(SERVER_INFO::loopBackIp.c_str());
		//m_ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		retval = connect(m_ClientSock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
		if (retval == SOCKET_ERROR)
		{
			int err = WSAGetLastError();
			ErrDisplay("Receive Failed");
			m_ConnectState = CONNECT_STATE::DISCONNECT;
			return false;
		}

		m_ConnectState = CONNECT_STATE::CONNECTED;

		// ������ ����.
		m_Thread = std::thread([this]() { Update(); });
		return true;
	}

	void ClientNetwork::Update()
	{
		int len = 0;
		int retval = 0;
		char buf[COMMON_INFO::recvSize + 1];

		// ��Ʈ��ũ ���°� CONNECTED�� ���� ���� ��ȯ.
		while (m_ConnectState == CONNECT_STATE::CONNECTED)
		{
			retval = recv(m_ClientSock, buf, COMMON_INFO::recvSize, 0);
			if (retval == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				ErrDisplay("Receive Failed");
				break;
			}
			else if (retval == 0)
			{
				Disconnect();
			}

			PacketHeaderDivide(buf, retval);
		}
	}

	void ClientNetwork::PacketHeaderDivide(char * buf, const int size)
	{
		int readPos = 0;
		int dataSize = size;
		PktHeader* pPacketHeader;

		while ((dataSize - readPos) > PACKET_HEADER_SIZE)
		{
			pPacketHeader = (PktHeader*)&buf[readPos];

			readPos += PACKET_HEADER_SIZE;

			// ��ť�� ��Ŷ �߰�.
			PushPacketToDeque(pPacketHeader, &buf[readPos]);

			readPos += pPacketHeader->BodySize;
		}
	}

	void ClientNetwork::PushPacketToDeque(const PktHeader * pkt, char * pDataPos)
	{
		RecvPacketInfo* packetInfo = new RecvPacketInfo;
		packetInfo->PacketId = pkt->Id;
		packetInfo->PacketBodySize = pkt->BodySize;
		packetInfo->pData = new char[pkt->BodySize];
		memcpy(&packetInfo->pData, pDataPos, pkt->BodySize);

		// ���ξ����忡�� ��Ŷ ��ť�� ����ϱ� ������ ���� �ɾ��ش�.
		std::lock_guard<std::mutex> lockDeque(m_Mutex);
		m_PacketDeque.push_back(packetInfo);
	}

	bool ClientNetwork::SendLogin(std::wstring idStr, std::wstring pwStr)
	{
		PktLogInReq newLoginReq;

		/* ��ȯ ����. */
		char szID[MAX_USER_ID_SIZE] = { 0, };
		char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };

		//Util::UnicodeToAnsi(idStr.c_str(), MAX_USER_ID_SIZE, szID);
		//Util::UnicodeToAnsi(pwStr.c_str(), MAX_USER_PASSWORD_SIZE, szPW);

		strncpy_s(newLoginReq.szID, MAX_USER_ID_SIZE + 1, szID, MAX_USER_ID_SIZE);
		strncpy_s(newLoginReq.szPW, MAX_USER_PASSWORD_SIZE + 1, szPW, MAX_USER_PASSWORD_SIZE);

		Send((short)PACKET_ID::LOGIN_IN_REQ, sizeof(newLoginReq), (char*)&newLoginReq);
		return true;
	}

	void ClientNetwork::Send(const short packetId, const short dataSize, char* pData)
	{
		char data[COMMON_INFO::MAX_PACKET_SIZE] = { 0, };

		PktHeader pktHeader{ packetId, dataSize };
		memcpy(&data[0], (char*)&pktHeader, PACKET_HEADER_SIZE);

		if (dataSize > 0)
		{
			memcpy(&data[PACKET_HEADER_SIZE], pData, dataSize);
		}

		int hr = send(m_ClientSock, data, dataSize + PACKET_HEADER_SIZE, 0);
		if (hr == SOCKET_ERROR)
		{
			int err = WSAGetLastError();
			ErrDisplay("Send Failed");
		}
	}

	void ClientNetwork::Disconnect()
	{
		if (m_ConnectState == CONNECT_STATE::CONNECTED)
		{
			m_Thread.join();

			closesocket(m_ClientSock);

			m_PacketDeque.clear();
		}
		Release();
	}

	void ClientNetwork::ErrDisplay(char * msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL
		);
		printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return;
	}

	void ClientNetwork::ErrQuit(char * msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL
		);

		MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}

}//������ �帧�� �߾˾ƾ� �Ѵ�!