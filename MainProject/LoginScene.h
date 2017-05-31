#pragma once

struct Login : MyApp::Scene
{
	GUI m_LoginGui;
	String m_IdStr;
	String m_PasswordStr;

	void init() override;

	void update() override;
	void checkLoginSuccessed();

	void draw() const override;

	bool tryLogin();
};