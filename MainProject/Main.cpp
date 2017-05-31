//#include <Siv3D.hpp>
//#include <HamFramework.hpp>

/*
지금 하려고 하는게 무엇인가? 아이디 입력창을 클릭하면 아이디 입력창으로 입력을 할 수 있게 만드는 것이다.
어떻게? 입력했을 때 그 공간에 입력할 수 있어야겠지? 그 공간이 선택되었는지 알아야겠지? 
*/

/*void Main()
{
	const Font BasicFont(30);
	const Font ButtonFont(15);

	const Rect IdRect(50, 100, 300, 50);
	const Rect PwRect(50, 175, 300, 50);
	const Rect LoginRect(375, 100, 100, 125);

	String IDText;
	String PWText;

	while (System::Update())
	{
		//font(L"너무 재미있다！").draw();

		//Circle(Mouse::Pos(), 50).draw({ 255, 0, 0, 127 });//원 그려라? 어떻게? 마우스의 위치에서, 반지름 50으로, rgba(255, 0, 0, 127)색으로

		const bool IdIn = IdRect.leftPressed;
		const bool PwIn = PwRect.leftPressed;
		const bool LoginIn = IdRect.leftPressed;


		//IdRect.draw(IdIn? BasicFont(IDText).draw(50, 100, Palette::Lightpink) : );//ID입력창

		Input::GetCharsHelper(IDText);
		
		

		PwRect.draw();//PW입력창

		BasicFont(L"PW 입력창！").draw(50, 175, Palette::Lightpink);

		LoginRect.draw();//LOGIN입력창

		ButtonFont(L"LOGIN 입력창！").draw(375, 100, Palette::Lightpink);
		
	}
}*/

/*enum class SceneName
{
	Front,
	Region,
	Country
};*/

/*class Title: public SceneManager <String>::Scene
{
public:

	GUI gui(GUIStyle::Default);

	void init() override
	{
		Graphics::SetBackground(Color(160, 200, 100));

		gui.add(L"IDF", GUITextField::Create(none));
		gui.add(L"PWF", GUITextField::Create(none));
		gui.add(L"LOGIN", GUIButton::Create(L"login"));
	}

	void update() override
	{
		if (Input::MouseL.clicked)
			changeScene(L"Game");
	}

	void draw() const override
	{
		//Window::ClientRect().draw(Palette::Blue);
		//font(L"TITLE").drawCenter(Window::Center());
		
	}

	Font  font{ 50 };

};

class  Game : public  SceneManager < String > ::Scene
{
public:

	void  update()  override
	{
		if (Input::MouseL.clicked)
			changeScene(L"Result");
	}

	void  draw()  const  override
	{
		Window::ClientRect().draw(Palette::Red);
		font(L"GAME").drawCenter(Window::Center());
	}

	Font  font{ 50 };
};

class  Result : public  SceneManager < String > ::Scene
{
public:

	void  update()  override
	{
		if (Input::MouseL.clicked)
			changeScene(L"Title");
	}

	void  draw()  const  override
	{
		Window::ClientRect().draw(Palette::Green);
		font(L"RESULT").drawCenter(Window::Center());
	}

	Font  font{ 50 };
};

void Main()
{
	SceneManager < String >  manager;

	manager.add < Title >(L"Title");
	manager.add < Game >(L"Game");
	manager.add < Result >(L"Result");

	while (System::Update())
	{
		manager.updateAndDraw();
	}
}*/

//장면전환어떻게하는가? 전환하는 장면에 원하는 요소 반영 어떻게 하는가?

/*
void Main()
{
	
	Graphics::SetBackground(Color(160, 200, 100));

	//front
	GUI fgui(GUIStyle::Default);

	fgui.add(L"IDF",GUITextField::Create(none));
	fgui.add(L"PWF",GUITextField::Create(none));//이거 위치 어떻게 바꿈?
	fgui.add(L"LOGIN", GUIButton::Create(L"login"));
	


	//region
	GUI rgui(GUIStyle::Default);

	rgui.add(L"Channel1", GUIButton::Create(L"채널1(1/100)"));

	rgui.add(L"Channel2", GUIButton::Create(L"채널2(100/100)"));

	rgui.add(L"Channel3", GUIButton::Create(L"채널3(100/100)"));

	rgui.add(L"Channel4", GUIButton::Create(L"채널4(100/100)"));

	rgui.add(L"Channel5", GUIButton::Create(L"채널5(100/100)"));
	

	//country
	GUI cgui(GUIStyle::Default);
	
	rgui.add(L"Channel1", GUIButton::Create(L"방1"));

	rgui.add(L"Channel1", GUIButton::Create(L"방2");

	rgui.add(L"Channel1", GUIButton::Create(L"방3"));

	rgui.add(L"Channel1", GUIButton::Create(L"방4"));

	rgui.add(L"Channel1", GUIButton::Create(L"방5"));
	

	while (System::Update())
	{
		//front
		if (gui.button(L"login").pushed)
		{
			Graphics::SetBackground(Color(160, 200, 100));

		}
		


	
	}

}
*/
//존재 성질, 기능 성질

#include "stdafx.h"
#include "Main.h"
#include "LoginScene.h"
#include "LobbyScene.h"
#include "RoomList.h"
#include "Room.h"


void Main()
{
	Window::SetTitle(L"Chatting Client Program");

	MyApp sceneManager(SceneManagerOption::ShowSceneName);

	sceneManager.add<Login>(L"Login");
	sceneManager.add<Lobby>(L"Lobby");
	sceneManager.add<RoomList>(L"RoomList");
	sceneManager.add<Room>(L"Room");

	while (System::Update())
	{
		if (!sceneManager.updateAndDraw())
		{
			break;
		}
	}
}