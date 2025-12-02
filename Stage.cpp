#include "Stage.h"
#include <iostream>
#include "vector"
void Stage::SetBlockHeight(int x, int z, int height)
{
}

Stage::Stage(GameObject* parent) : fbx(nullptr)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	fbx = new Fbx();
	std::vector<string> ModelName
	{
		"default.fbx",
		"Glass.fbx",
		"Sand.fbx",
		"Water.fbx",
		"Lenga.fbx"
	};
}

void Stage::Draw()
{
	for (int i = 0; i < ModelName.size(); i++)
	{
		for (int j = 0; j < ModelName.size(); j++)
		{
			fbx->Draw(transform_);

		}
		
	}
}

void Stage::Update()
{
}

void Stage::Release()
{
}
