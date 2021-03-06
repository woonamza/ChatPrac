#include <Windows.h>
#include <memory>
#include <deque>
#include "Packet.h"
#include "PacketProcessor.h"
#include "Observer.h"

namespace DataContainer
{
	void LoginData::Subscribe(std::shared_ptr<PacketProcessor> processor)
	{
		processor->Subscribe((short)PACKET_ID::LOGIN_IN_RES, &m_RecvQueue);
	}

	void LoginData::Update()
	{
		// 받은 큐가 비어있으면 일하지 않음.
		if (m_RecvQueue.empty())
		{
			Sleep(0);
			return;
		}

		// 큐가 있다면 빼서, LOGIN_IN_RES 패킷이 왔는지 확인.
		std::lock_guard<std::mutex> lockDeque(m_Mutex);
		auto packet = m_RecvQueue.front();

		if (packet->PacketId != (short)PACKET_ID::LOGIN_IN_RES)
		{
			// 뭔가 잘못된 패킷이 옴.
			OutputDebugString(L"Invaild Packet Receive! (In LoginData Update)");
			return;
		}



		// 올바른 패킷이 왔다면, 에러코드 확인.
		auto i = (PktLogInRes*)packet->pData;
		if (i->ErrorCode != (short)ERROR_CODE::NONE)
		{
			// 로그인 실패
			OutputDebugString(L"[LoginData] 로그인 실패!\n");
		}
		else
		{
			// 성공
			OutputDebugString(L"[LoginData] 로그인 성공!\n");
			m_IsLoginSuccessed = true;
		}
		m_RecvQueue.pop_front();
	}
}