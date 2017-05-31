#include "stdafx.h"
#include "LoginScene.h"
#include "DataContainer.h"
#include "Util.h"

void Login::init()
{
	using namespace DataContainer;
	m_LoginGui = GUI(GUIStyle::Default);
	Graphics::SetBackground(Color(160, 200, 100));

	m_LoginGui.setTitle(L"Login!");

	m_LoginGui.add(L"idLabel", GUIText::Create(L"ID :   "));
	m_LoginGui.addln(L"idField", GUITextField::Create(none));
	m_LoginGui.add(L"passwordLabel", GUIText::Create(L"PW : "));
	m_LoginGui.addln(L"passwordField", GUITextField::Create(none));

	m_LoginGui.add(L"loginBtn", GUIButton::Create(L"LOGIN"));
	m_LoginGui.addln(L"exitBtn", GUIButton::Create(L"EXIT"));

	m_LoginGui.setCenter(Window::Center());

	m_data->dataContainer.Init();
	m_data->dataContainer.ConnectRequest();
}

void Login::update()
{
	if (m_LoginGui.button(L"exitBtn").pushed)
	{
		System::Exit();
	}
	if (m_LoginGui.button(L"loginBtn").pushed)
	{
		tryLogin();
	}
	if (Input::KeyEnter.clicked)
	{
		tryLogin();
	}
	m_data->dataContainer.Update();
	checkLoginSuccessed();
}

void Login::checkLoginSuccessed()
{
	if (m_data->dataContainer.GetLoginData()->GetLoginSuccessed())
	{
		OutputDebugString(L"로그인 성공.");
		changeScene(L"Lobby");
	}
}

void Login::draw() const
{
}

bool Login::tryLogin()
{
	m_IdStr = m_LoginGui.textField(L"idField").text;
	m_PasswordStr = m_LoginGui.textField(L"passwordField").text;

	PktLogInReq newLoginReq;

	char szID[MAX_USER_ID_SIZE] = { 0, };
	char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };

	DataContainer::Util::UnicodeToAnsi(m_IdStr.c_str(), MAX_USER_ID_SIZE, szID);
	DataContainer::Util::UnicodeToAnsi(m_PasswordStr.c_str(), MAX_USER_PASSWORD_SIZE, szPW);

	strncpy_s(newLoginReq.szID, MAX_USER_ID_SIZE + 1, szID, MAX_USER_ID_SIZE);
	strncpy_s(newLoginReq.szPW, MAX_USER_PASSWORD_SIZE + 1, szPW, MAX_USER_PASSWORD_SIZE);

	m_data->dataContainer.SendRequest((short)PACKET_ID::LOGIN_IN_REQ, sizeof(newLoginReq), (char*)&newLoginReq);
	return true;
}
//클라이언트 두 개 띄어서 로비 들어가서 유저리스트에 나와야한다.