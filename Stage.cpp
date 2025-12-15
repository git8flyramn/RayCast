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
	for (int i = 0; i < ModelName.size(); i++)
	{
		hModel[i] = Model::Load(ModelName[i]);
		assert(hModel[i] >= 0);
	}
}

void Stage::Draw()
{
	for (int i = 0; i < ZSIZE; i++)
	{
		for (int j = 0; j < XSIZE; j++)
		{
			for(int k = 0; k < (int)(GetT(i,j).height); k++)
			{
			 int type = (int)(GetT(i,j)).type;
			 Transform t;
			 t.position_.x = i - 15 / 2.0;
			 t.position_.z = j;
			 t.position_.y = k - 15;
			 t.scale_ = { 0.95,0.95,0.95 };
			 Model::SetTransform(hModel[type],t);
			 Model::Draw(hModel[type]);
			}

		}
		
	}
}

void Stage::Update()
{
}

void Stage::Release()
{
}
