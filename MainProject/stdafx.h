#pragma once

#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include <vector>
#include <string>
#include <stack>
#include <array>
#include "InlineFrame.h"
#include "DataContainer.h"

struct SceneData
{
	int32 sceneNumber;
	DataContainer::DataContainer dataContainer;

};

using MyApp = SceneManager<String, SceneData>;

struct RoomInfo
{
	std::wstring roomName;
	std::wstring buttonName;
};

struct UserInfo
{
	std::wstring userName;
};