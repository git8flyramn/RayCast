#include "PlayScene.h"
#include "Engine/Model.h"
#include "Engine/Texture.h"
#include "Engine/Fbx.h"
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
	transform_.position_ = { 0.0f,0.0f,0.0f };
}

void PlayScene::Draw()
{
	////int box[10][10] ={{1,1,1,1,1,1,1,1,1 },
	//			     { 1,1,1,1,1,1,1,1,1,1 },
	//			     { 1,1,1,1,1,1,1,1,1,1 },
	//			     { 1,1,1,1,1,1,1,1,1,1 },
	//			     { 1,1,1,1,1,1,1,1,1,1 },
	//	             { 1,1,1,1,1,1,1,1,1,1 },
	//				 { 1,1,1,1,1,1,1,1,1,1 },
	//				 { 1,1,1,1,1,1,1,1,1,1 },
	//				 { 1,1,1,1,1,1,1,1,1,1 },
	//	             { 1,1,1,1,1,1,1,1,1,1 },
	//};
	//
	//for (float i = 0.1f; i < 3.0f; i += 0.1f)
	//{
	//	for (float j = 0.1f; j < 3.0f; j+= 0.1f)
	//	{
	//		fbx->Draw(transform_);
	//		
	//		transform_.position_.x = j;
	//	}
	//	transform_.position_.z = i;
	//}
  	fbx->Draw(transform_);
}

void PlayScene::Update()
{
}

void PlayScene::Release()
{
}
