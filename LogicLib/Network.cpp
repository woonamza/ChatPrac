 
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

bool ClientNetwork::Init()//조종기 재료 가지기, 조종기 가지기
{
	//조종기 재료(윈속) 가지기(초기화)
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		return false;
	}

	//조종기(소켓) 만들기,가지기(초기화)
	m_Clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Clientsock == INVALID_SOCKET)
	{
		return false;
	}

	return true;
};

void ClientNetwork::Close()//조종기(소켓) 없애기,버리기
{
	if (m_Status == NetworkStatus::Connect)
	{
		closesocket(m_Clientsock);
		m_Status = NetworkStatus::Disconnect;
	}

}

void ClientNetwork::Release()//조종기 재료(소켓) 버리기
{
	WSACleanup();
};

bool ClientNetwork::Connect()//서버로 연결하게 조종
{
	struct sockaddr_in sockinfo;

	if (connect(m_Clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
	{
		return false;
	}
	
	//m_SplitThread =, 정보를 받아야 할 때 사용하는 수단(스레드)이므로 업데이트 기능에서 만들어야 하는 것 아닐까?
	
	return true;
};

void ClientNetwork::Send()//서버로 정보 보내게 조종
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

//서버로 부터 정보를 계속 받아들여야 한다, 다른 행위를 하더라도 서버가 정보를 보낼 때 마다 받아들여야 한다
//그러니까 (thread()로) 수단 만들고 (while문으로) 계속 (recv()로) 받는다?
//정보는 형식정보와 내용정보로 이루어져있는데 내용정보를 얻어야한다, 정보를 형식정보와 내용정보로 나누어 내용정보를 얻어야 한다

//패킷 추출하려면, 헤더 봐야하고, 그러려면(얻으려는게(추출하려는게) 바디이므로 '바디 봐야하고 그러려면 헤더를 봐야한다' 아닐까?) 바디 봐야한다
void ClientNetwork::Update()//서버로 부터 정보 받아 들이게 조종
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
			//recv의 반환값은 읽은 바이트크기이다.
			Retval = recv(m_Clientsock, Buff, sizeof(short), 0);//헤더id 읽는다. 수신소켓으로 수신버퍼에 도착(하위단에서 처리하겠지?)한 데이터를 지정한 공간에 저장한다
			Retval = recv(m_Clientsock, Buff+1, sizeof(short), 0);//헤더bodysize 읽는다
			bodysize = atoi(Buff[Retval]);//얼만큼 읽어야 하는가?
			Retval = recv(m_Clientsock, Buff+2, bodysize, 0);//bodysize만큼 읽는다
			Buff[retval] = '\0';//받은 정보만 출력하고싶다면, 받은 정보의 뒤에 null문자를 넣어야한다
			printf("%s\n", Buff);
			
		}
	});//스레드로 계속 받는다! 헤더를 보고 바디를 얻는다?!



	//Retval = ;

};*/
//코드를 읽을수 있어야 한다!!
//로비리스트까지 들어간다

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

		// 쓰레드 생성.
		m_Thread = std::thread([this]() { Update(); });
		return true;
	}

	void ClientNetwork::Update()
	{
		int len = 0;
		int retval = 0;
		char buf[COMMON_INFO::recvSize + 1];

		// 네트워크 상태가 CONNECTED일 때만 루프 순환.
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

			// 디큐에 패킷 추가.
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

		// 메인쓰레드에서 패킷 디큐를 사용하기 때문에 락을 걸어준다.
		std::lock_guard<std::mutex> lockDeque(m_Mutex);
		m_PacketDeque.push_back(packetInfo);
	}

	bool ClientNetwork::SendLogin(std::wstring idStr, std::wstring pwStr)
	{
		PktLogInReq newLoginReq;

		/* 변환 과정. */
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

}//데이터 흐름을 잘알아야 한다!