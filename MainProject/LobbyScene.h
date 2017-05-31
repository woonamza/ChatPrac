#pragma once

struct LobbyInfo
{
	std::wstring LobbyName;
	std::wstring LobbyId;
	std::wstring LobbyButtonName;
	int UserNumber = 0;
	int MaxUserNumber = 0;
};


struct Lobby : MyApp::Scene
{
	GUI m_LobbyGui;
	GUI m_InfoGui;
	std::vector<LobbyInfo*> m_LobbyVector;


	void init() override;

	void update() override;

	void draw() const override;

	void lobbyInfoSetting();

	void makeLobbys();

	void exitScene();

	void checkButtonClicked();
};