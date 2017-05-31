#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

namespace NCommon
{
#pragma pack(push, 1)
	struct PktHeader
	{
		short Id;
		short BodySize;
	};

	const int PACKET_HEADER_SIZE = sizeof(PktHeader);

	struct PktBase
	{
		short ErrorCode = (short)ERROR_CODE::NONE;
		void SetError(ERROR_CODE error) { ErrorCode = (short)error; }
	};

	//- �α��� ��û
	const int MAX_USER_ID_SIZE = 16;
	const int MAX_USER_PASSWORD_SIZE = 16;
	struct PktLogInReq
	{
		char szID[MAX_USER_ID_SIZE + 1] = { 0, };
		char szPW[MAX_USER_PASSWORD_SIZE + 1] = { 0, };
	};

	struct PktLogInRes : PktBase
	{
	};

	struct RecvPacketInfo
	{
		RecvPacketInfo() {};

		short PacketId = 0;
		short PacketBodySize = 0;
		char* pData = nullptr;
	};

	//- ä�� ����Ʈ ��û
	const int MAX_LOBBY_LIST_COUNT = 20;
	struct LobbyListInfo
	{
		short LobbyId;
		short LobbyUserCount;
	};
	struct PktLobbyListRes : PktBase
	{
		short LobbyCount = 0;
		LobbyListInfo LobbyList[MAX_LOBBY_LIST_COUNT];
	};


	//- �κ� ���� ��û
	struct PktLobbyEnterReq
	{
		short LobbyId;
	};

	struct PktLobbyEnterRes : PktBase
	{
		short MaxUserCount;
		short MaxRoomCount;
	};


	//- �κ� �ִ� �������� �κ� ���� ���� �뺸
	struct PktLobbyNewUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};


	//- �κ��� �� ����Ʈ ��û
	struct PktLobbyRoomListReq
	{
		short StartRoomIndex; // �κ񿡼� ó�� ��û�� ���� 0, �ι�°���ʹ� �տ� ���� �������� ������ ��ȣ + 1
	};

	const int MAX_ROOM_TITLE_SIZE = 16;
	struct RoomSmallInfo
	{
		short RoomIndex;
		short RoomUserCount;
		wchar_t RoomTitle[MAX_ROOM_TITLE_SIZE + 1] = { 0, };
	};

	const int MAX_NTF_LOBBY_ROOM_LIST_COUNT = 12;
	struct PktLobbyRoomListRes : PktBase
	{
		bool IsEnd = false; // true �̸� �� �̻� �� ����Ʈ ��û�� ���� �ʴ´�
		short Count = 0;
		RoomSmallInfo RoomInfo[MAX_NTF_LOBBY_ROOM_LIST_COUNT];
	};


	//- �κ��� ���� ����Ʈ ��û
	struct PktLobbyUserListReq
	{
		short StartUserIndex;
	};

	struct UserSmallInfo
	{
		short LobbyUserIndex;
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};

	const int MAX_SEND_LOBBY_USER_LIST_COUNT = 32;
	struct PktLobbyUserListRes : PktBase
	{
		bool IsEnd = false; // true �̸� �� �̻� �� ����Ʈ ��û�� ���� �ʴ´�
		short Count = 0;
		UserSmallInfo UserInfo[MAX_SEND_LOBBY_USER_LIST_COUNT];
	};


	//- �κ񿡼� ������ ��û
	struct PktLobbyLeaveReq {};

	struct PktLobbyLeaveRes : PktBase
	{
	};

	//- �κ񿡼� ������ ���� �뺸(�κ� �ִ� ��������)
	struct PktLobbyLeaveUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};


	//- �뿡 ���� ��û
	struct PktRoomEnterReq
	{
		bool IsCreate;
		short RoomIndex;
		wchar_t RoomTitle[MAX_ROOM_TITLE_SIZE + 1];
	};

	struct PktRoomEnterRes : PktBase
	{
	};


	//- ����� �� ���� �뺸
	struct PktChangedRoomInfoNtf
	{
		RoomSmallInfo RoomInfo;
	};

	//- �뿡 �ִ� �������� ���� ���� ���� ���� �뺸
	struct PktRoomEnterUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};


	//- �� ������ ��û
	struct PktRoomLeaveReq {};

	struct PktRoomLeaveRes : PktBase
	{
	};

	//- �뿡�� ������ ���� �뺸(�κ� �ִ� ��������)
	struct PktRoomLeaveUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};


	//- �� ä��
	const int MAX_ROOM_CHAT_MSG_SIZE = 256;
	struct PktRoomChatReq
	{
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	struct PktRoomChatRes : PktBase
	{
	};

	struct PktRoomChatNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	//- �κ� ä��
	const int MAX_LOBBY_CHAT_MSG_SIZE = 256;
	struct PktLobbyChatReq
	{
		wchar_t Msg[MAX_LOBBY_CHAT_MSG_SIZE + 1] = { 0, };
	};

	struct PktLobbyChatRes : PktBase
	{
	};

	struct PktLobbyChatNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
		wchar_t Msg[MAX_LOBBY_CHAT_MSG_SIZE + 1] = { 0, };
	};


	// ������ ���� ���� ��û
	struct PktRoomMaterGameStartReq
	{};

	struct PktRoomMaterGameStartRes : PktBase
	{};

	struct PktRoomMaterGameStartNtf
	{};


	// ������ �ƴ� ����� ���� ���� ��û
	struct PktRoomGameStartReq
	{};

	struct PktRoomGameStartRes : PktBase
	{};

	struct PktRoomGameStartNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};




	const int DEV_ECHO_DATA_MAX_SIZE = 1024;

	struct PktDevEchoReq
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

	struct PktDevEchoRes : PktBase
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

#pragma pack(pop)



}