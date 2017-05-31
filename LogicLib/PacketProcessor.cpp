#include <Windows.h>
#include <map>
#include <memory>
#include <vector>
#include <deque>
#include "Observer.h"
#include "PacketMessenger.h"
#include "PacketProcessor.h"

namespace DataContainer
{
	void PacketProcessor::Update()
	{
		// �޽��� ����� �ȵǾ� ���� ��� Update�� �������� ����. 
		if (m_IsMessengerRegisterd == false)
		{
			return;
		}

		auto packet = m_pMessengerAddress->GetPacketFromDeque();

		/* ť�� ��Ŷ�� ������ Update�� ��� ����. */
		if (packet == nullptr)
		{
			Sleep(0);
			return;
		}

		/* ��Ŷ�� �ִٸ�, ������ ���� �������鿡�� ��ε�ĳ��Ʈ. */
		BroadCast(packet);
	}

	void PacketProcessor::RegisterMessenger(std::shared_ptr<PacketMessenger> messengerAddress)
	{
		if (m_IsMessengerRegisterd == false)
		{
			m_pMessengerAddress = messengerAddress;
			m_IsMessengerRegisterd = true;
		}
	}

	void PacketProcessor::BroadCast(RecvPacketInfo* packet)
	{
		auto subscribers = m_ObserverMap.find(packet->PacketId);
		if (subscribers == m_ObserverMap.end())
		{
			OutputDebugString(L"INVALID BroadCast");
			return;
		}
		else
		{
			for (auto& i : subscribers->second)
			{
				i->push_back(packet);
			}
		}
	}

	void PacketProcessor::Subscribe(short interestPacketId, std::deque<RecvPacketInfo*>* observerPacketQueue)
	{
		// �ش� ��Ŷ ���̵�� ���� ����� ������ ť�� ������ ���ο� ����Ʈ ����.
		if (m_ObserverMap.find(interestPacketId) == m_ObserverMap.end())
		{
			ObserverQueueVector newObserverVector;
			m_ObserverMap.emplace(interestPacketId, std::move(newObserverVector));
		}

		// ����Ʈ�� ������ ť �߰�.
		m_ObserverMap.at(interestPacketId).emplace_back(observerPacketQueue);
	}

}//�ٸ� ������ ���� ī��Ʈ�� �÷���� �Ұ� ������?