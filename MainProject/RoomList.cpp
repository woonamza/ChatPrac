#include "stdafx.h"
#include "RoomList.h"
#include "TextScrollBox.h"

const int roomWidth = 250;
const int buttonWidth = 50;
const int userInfoWidth = 200;
const int chattingInfoHeight = 275;

void RoomList::init()
{
	m_RoomListGui = GUI(GUIStyle::Default);
	m_RoomListGui.setTitle(L"Room List");

	m_UserListGui = GUI(GUIStyle::Default);
	m_UserListGui.setTitle(L"User List");
	Point userGuiPos = Point(roomWidth + buttonWidth + 110, 0);
	m_UserListGui.setPos(userGuiPos);

	m_ChattingGui = GUI(GUIStyle::Default);
	Point chattingGuiPos = Point(0, chattingInfoHeight);
	m_ChattingGui.setPos(chattingGuiPos);

	Graphics::SetBackground(Color(200, 160, 100));

	roomInfoSetting();
	userInfoSetting();

	makeRooms();
	makeUsers();
	makeChattingGui();
}

void RoomList::update()
{
	if (m_ChattingGui.button(L"InputButton").pressed || Input::KeyEnter.clicked)
	{
		if (m_ChattingGui.textArea(L"InputField").text == L"")
		{
			return;
		}

		if (m_ChatString == L"")
		{
			m_ChatString = m_ChatString + m_ChattingGui.textArea(L"InputField").text;
		}
		else
		{
			m_ChatString = m_ChatString + L"\n" + m_ChattingGui.textArea(L"InputField").text;
		}

		m_ChattingGui.textArea(L"ChattingWindow").setText(m_ChatString);
		m_ChattingGui.textArea(L"InputField").setText(L"");
	}

	checkRoomClicked();
}

void RoomList::draw() const
{
}

void RoomList::refresh()
{

}

void RoomList::roomInfoSetting()
{
	const int roomInfoNum = 6;
	const std::wstring roomName = L"Room ";
	for (int i = 0; i < roomInfoNum; ++i)
	{
		RoomInfo* newRoomInfo = new RoomInfo;
		newRoomInfo->roomName = roomName + std::to_wstring(i + 1);
		std::wstring buttonName = L"Button" + std::to_wstring(i + 1);
		newRoomInfo->buttonName = buttonName;

		m_RoomInfoVector.push_back(newRoomInfo);
	}
}

void RoomList::userInfoSetting()
{
	const int userInfoNum = 5;
	const std::wstring userName = L"User ";
	for (int i = 0; i < userInfoNum; ++i)
	{
		UserInfo* newUserInfo = new UserInfo;
		newUserInfo->userName = userName + std::to_wstring(i + 1);
		m_UserInfoVector.push_back(newUserInfo);
	}
}

void RoomList::makeRooms()
{
	for (const auto& i : m_RoomInfoVector)
	{
		m_RoomListGui.add(i->roomName, GUIText::Create(i->roomName, roomWidth));

		m_RoomListGui.addln(i->buttonName, GUIButton::Create(L"ENTER", buttonWidth));
	}
}

void RoomList::makeUsers()
{
	for (const auto& i : m_UserInfoVector)
	{
		m_UserListGui.addln(i->userName, GUIText::Create(i->userName, userInfoWidth));
	}
}

void RoomList::makeChattingGui()
{
	m_ChattingGui.addln(L"ChattingWindow", GUITextArea::Create(5, 33));
	//m_pChatTextBox = new TextScorllBox(Rect(30, chattingInfoHeight, 585, 130));

	m_ChattingGui.add(L"Divider", GUIHorizontalLine::Create(1));
	m_ChattingGui.horizontalLine(L"Divider").style.color = Color(127);

	//m_ChattingGui.setPos(15, chattingInfoHeight + 140);
	m_ChattingGui.add(L"InputField", GUITextArea::Create(1, 26));
	m_ChattingGui.add(L"InputButton", GUIButton::Create(L"Send"));
}

void RoomList::checkRoomClicked()
{
	for (const auto i : m_RoomInfoVector)
	{
		if (m_RoomListGui.button(i->buttonName).pushed)
		{
			changeScene(L"Room");
		}
	}
}

void RoomList::exitScene()
{
	m_RoomInfoVector.clear();
	m_UserInfoVector.clear();
}