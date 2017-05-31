#pragma once
//네트워크 관련 기능 connect,thread만들고 while돌면서 recv() 받으면 저장소에 넣는다,그리고 send()! (서버가 이런다고?)
//네트워크 상태나타내는 상태변수 enum 필요하다. connect, disconnect, connecting

/*#define BUFFSIZE 128

namespace LogicLib
{
	enum class NetworkStatus
	{
		Disconnect = 0,
		Connect,
		Connecting
	};

	class ClientNetwork
	{
	public:

		ClientNetwork() = default;
		~ClientNetwork() = default;

		bool Init();//조종기 재료, 조종기 가지기

		void Close();//조종기 버리기

		void Release();//조종기 재료 버리기

		bool Connect();//서버로 연결하게 조종

		void Send();//서버로 정보 보내게 조종
		
		void Update();//서버로 부터 정보 받아들이게 조종
		
		char Buff[BUFFSIZE + 1];

		std::thread m_SplitThread;

	private:

		SOCKET m_Clientsock = INVALID_SOCKET;

		NetworkStatus m_Status = NetworkStatus::Disconnect;
	};
}*/

//해야할것이? 받는다. 어떻게? recv()로 쪼개서. 어떻게?
//과제 프로그램 실행하면 ui: id, pw (입력받으면 그냥 ok) 접속누르면 connect호출, 로비 리스트

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <deque>
#include <string>
#include <mutex>

#include "Definition.h"
#include "ErrorCode.h"
#include "Packet.h"
#include "PacketID.h"

using namespace NCommon;

namespace ClientLogic
{
	enum class CONNECT_STATE
	{
		DISCONNECT, CONNECTING, CONNECTED
	};

	class ClientNetwork
	{
	public:

		ClientNetwork() = default;
		~ClientNetwork() = default;

		CONNECT_STATE GetConnectState() { return m_ConnectState; };
		RecvPacketInfo* GetPacketFromDeque()
		{
			std::lock_guard<std::mutex> lockDeque(m_Mutex);
			if (m_PacketDeque.empty())
			{
				return nullptr;
			}
			auto returnPacket = m_PacketDeque.front();
			m_PacketDeque.pop_front();
			return returnPacket;
		};

		void Init();
		void Release();
		bool Connect();

		bool SendLogin(std::wstring, std::wstring);

	private:


		void Update();

		void PacketHeaderDivide(char* buf, const int size);

		void PushPacketToDeque(const PktHeader*, char*);

		void Send(char* buf, const int size);
		void Send(const short, const short, char*);

		void Disconnect();

		void ErrDisplay(char * msg);
		void ErrQuit(char * msg);

		CONNECT_STATE  m_ConnectState = CONNECT_STATE::DISCONNECT;
		SOCKET m_ClientSock = INVALID_SOCKET;
		SOCKADDR_IN m_ServerAddr;
		char m_RecvBuffer[COMMON_INFO::recvSize] = { 0, };
		std::thread m_Thread;
		std::deque<RecvPacketInfo*> m_PacketDeque;
		std::mutex m_Mutex;

	};

	class PacketProcess
	{
	public:
		PacketProcess() = default;
		~PacketProcess() = default;

		void Update();
		void RegisterClientNetwork(ClientNetwork*);
		bool GetIsLoginSuccessed() { return m_IsLoginSuccessed; };
		void SetIsLoginSuccedded(const bool isSuccessed) { m_IsLoginSuccessed = isSuccessed; };

	private:

		ClientNetwork* m_ClientNetwork;
		bool m_IsClientRegisterd = false;
		bool m_IsLoginSuccessed = false;

	};
}