#include "stdafx.h"
#include "LobbyScene.h"
#include <random>
#include <ctime>

const int lobbyMaxUserNumber = 300;
const int lobbyMaxNumber = 5;
const int roomNameWidth = 200;
const int roomUserWidth = 95;
const int infoWidth = 150;

void Lobby::init()
{
	m_LobbyGui = GUI(GUIStyle::Default);
	m_LobbyGui.setTitle(L"Lobby");
	m_InfoGui = GUI(GUIStyle::Default);
	m_InfoGui.setTitle(L"My Info");

	lobbyInfoSetting();

	Point infoGuiPos = Point(roomNameWidth + roomUserWidth + 160, 0);
	m_InfoGui.setPos(infoGuiPos);
	Graphics::SetBackground(Color(100, 160, 200));

	makeLobbys();

	m_InfoGui.addln(L"Info", GUIText::Create(L"Y Z, X", infoWidth));
}

void Lobby::update()
{
	checkButtonClicked();

}

void Lobby::draw() const
{

}

void Lobby::lobbyInfoSetting()
{

	std::mt19937 rng((unsigned int)time(NULL));
	std::uniform_int_distribution<int> lobbyNumberRange(1, lobbyMaxNumber);
	std::uniform_int_distribution<int> userNumberRange(0, lobbyMaxUserNumber);

	int lobbyNumber = lobbyNumberRange(rng);
	for (int i = 0; i < lobbyNumber; ++i)
	{
		LobbyInfo* newLobbyInfo = new LobbyInfo;
		newLobbyInfo->LobbyName = L"Chatting Server " + std::to_wstring(i + 1);
		newLobbyInfo->LobbyButtonName = L"Button" + std::to_wstring(i + 1);
		newLobbyInfo->LobbyId = L"Lobby" + std::to_wstring(i + 1);
		newLobbyInfo->MaxUserNumber = lobbyMaxUserNumber;
		newLobbyInfo->UserNumber = userNumberRange(rng);
		m_LobbyVector.push_back(newLobbyInfo);
	}
}

void Lobby::makeLobbys()
{
	int idx = 1;

	for (auto i : m_LobbyVector)
	{
		std::wstring userNumberInfo = std::to_wstring(i->UserNumber) + L" / " + std::to_wstring(i->MaxUserNumber);
		m_LobbyGui.add(i->LobbyId, GUIText::Create(i->LobbyName, roomNameWidth));
		m_LobbyGui.add(L"userNumberInfo", GUIText::Create(userNumberInfo, roomUserWidth));
		m_LobbyGui.addln(i->LobbyButtonName, GUIButton::Create(L"ENTER"));

		++idx;
	}
}

void Lobby::exitScene()
{
	m_LobbyVector.clear();
}

void Lobby::checkButtonClicked()
{
	for (const auto i : m_LobbyVector)
	{
		if (m_LobbyGui.button(i->LobbyButtonName).pushed)
		{
			changeScene(L"RoomList");
		}
	}
}