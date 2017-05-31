#include "network.h"
#include "ErrorCode.h"
#include "Packet.h"
#include "PacketID.h"
using namespace NCommon;

namespace ClientLogic
{
	void PacketProcess::Update()
	{
		/* Ŭ���̾�Ʈ�� ����� �ȵǾ������� Update �ǽ����� ����. */
		if (m_IsClientRegisterd == false)
		{
			return;
		}

		auto packet = m_ClientNetwork->GetPacketFromDeque();

		/* ť�� ��Ŷ�� ������ Update �ǽ����� ����. */
		if (packet == nullptr)
		{
			return;
		}

		/* ��Ŷ ���̵� �м��Ͽ� �ش� ���� ����. */
		switch (packet->PacketId)
		{
		case (short)PACKET_ID::LOGIN_IN_RES:

			auto i = (PktLogInRes*)packet->pData;

			if (i->ErrorCode != (short)ERROR_CODE::NONE)
			{
				// ERROR
				OutputDebugString(L"[PacketProcess] �α��� ����!");
			}
			else
			{
				// ����
				OutputDebugString(L"[PacketProcess] �α��� ����");
				m_IsLoginSuccessed = true;
			}
			delete i;

			break;
		}

	}

	void PacketProcess::RegisterClientNetwork(ClientNetwork* clientNetwork)
	{
		if (m_IsClientRegisterd == false)
		{
			m_ClientNetwork = clientNetwork;
			m_IsClientRegisterd = true;
		}
		return;
	}

}