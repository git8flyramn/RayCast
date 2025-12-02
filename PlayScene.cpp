#include "PlayScene.h"
#include "Engine/Model.h"
#include "Engine/Texture.h"
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), fbx(nullptr)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	fbx = new Fbx;
	transform_.scale_.x = 0.5f;
	transform_.scale_.y = 0.5f;
	transform_.scale_.z = 0.5f;
    fbx->Load("Water.fbx");
	transform_.position_ = { 0.0f,1.0f,0.0f };
}

void PlayScene::Draw()
{
	int box[10][10] ={{1,1,1,1,1,1,1,1,1 },
				     { 1,1,1,1,1,1,1,1,1,1 },
				     { 1,1,1,1,1,1,1,1,1,1 },
				     { 1,1,1,1,1,1,1,1,1,1 },
				     { 1,1,1,1,1,1,1,1,1,1 },
		             { 1,1,1,1,1,1,1,1,1,1 },
					 { 1,1,1,1,1,1,1,1,1,1 },
					 { 1,1,1,1,1,1,1,1,1,1 },
					 { 1,1,1,1,1,1,1,1,1,1 },
		             { 1,1,1,1,1,1,1,1,1,1 },
	};
	
	for (int i = 0; i < 10; i++)
	{
		//transform_.position_.x = 0.1f * i;
		for (int j = 0; j < 10; j++)
		{
			fbx->Draw(transform_);
			
			transform_.position_.z = i;
		}
	}
		
}

void PlayScene::Update()
{

}

void PlayScene::Release()
{
}
