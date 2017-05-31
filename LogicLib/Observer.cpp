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
		// ���� ť�� ��������� ������ ����.
		if (m_RecvQueue.empty())
		{
			Sleep(0);
			return;
		}

		// ť�� �ִٸ� ����, LOGIN_IN_RES ��Ŷ�� �Դ��� Ȯ��.
		std::lock_guard<std::mutex> lockDeque(m_Mutex);
		auto packet = m_RecvQueue.front();

		if (packet->PacketId != (short)PACKET_ID::LOGIN_IN_RES)
		{
			// ���� �߸��� ��Ŷ�� ��.
			OutputDebugString(L"Invaild Packet Receive! (In LoginData Update)");
			return;
		}



		// �ùٸ� ��Ŷ�� �Դٸ�, �����ڵ� Ȯ��.
		auto i = (PktLogInRes*)packet->pData;
		if (i->ErrorCode != (short)ERROR_CODE::NONE)
		{
			// �α��� ����
			OutputDebugString(L"[LoginData] �α��� ����!\n");
		}
		else
		{
			// ����
			OutputDebugString(L"[LoginData] �α��� ����!\n");
			m_IsLoginSuccessed = true;
		}
		m_RecvQueue.pop_front();
	}
}