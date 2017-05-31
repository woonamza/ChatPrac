#pragma once

class TextScorllBox;


struct RoomList : MyApp::Scene
{
	GUI m_RoomListGui;
	GUI m_UserListGui;
	GUI m_ChattingGui;
	TextScorllBox* m_pChatTextBox;
	std::vector<RoomInfo*> m_RoomInfoVector;
	std::vector<UserInfo*> m_UserInfoVector;
	std::stack<std::wstring*> m_ChatStack;
	String m_ChatString;

	void init() override;

	void update() override;

	void draw() const override;

	void refresh();

	void roomInfoSetting();
	void userInfoSetting();

	void makeRooms();
	void makeUsers();
	void makeChattingGui();

	void checkRoomClicked();

	void exitScene();
};