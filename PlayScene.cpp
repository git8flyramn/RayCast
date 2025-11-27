#include "PlayScene.h"
#include "Engine/Model.h"
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
	fbx->Load("default.fbx");
	transform_.position_ = { 0.0f,1.0f,-8.0f };
}

void PlayScene::Draw()
{
		for (int i = 0; i < 10; i++)	
		{
		
			for (int j = 0; j < 10; j++)
			{
				fbx->Draw(transform_);
				transform_.position_.x = 0.05f * i * j;
            }
			
			
		}
	
}

void PlayScene::Update()
{

}

void PlayScene::Release()
{
}
