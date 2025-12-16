#include "Stage.h"
#include <string>
#include "vector"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent) : GameObject(parent,"Stage")
{
	for (int j = 0; j < ZSIZE; j++)
	{
		for (int i = 0; i < XSIZE; i++)
		{
			SetBlock(BLOCK_TYPE::WATER, i, j);
			SetBlockHeight(i, j, 1 + rand() % 14);
		}
	}
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
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
	/*for (int i = 0; i < ZSIZE; i++)
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
		
	}*/

	RayCastData data;
	Transform trans;

	trans.position_.x = transform_.position_.x;
	trans.position_.y = transform_.position_.y;
	trans.position_.z = transform_.position_.z;
	int type = BLOCK_TYPE::WATER;
	Model::SetTransform(hModel[type], trans);
	Model::Draw(hModel[type]);
	RayCastData rayData
	{
		{0.0f,0.0f,5.0f,0.0f},
		{ 0.0f,-1.0f, 0.0f,0.0f},
		false,
		0.0f
	};

	Model::Raycast(hModel[type], data);

	if (data.isHit)
	{
		MessageBoxA(NULL,"hit","Info",MB_OK);

	}
}

void Stage::Update()
{
	
}

void Stage::Release()
{
}
