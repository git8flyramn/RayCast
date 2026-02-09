#include "PlayScene.h"
#include "Engine/Model.h"
#include "Engine/Texture.h"
#include "Engine/Fbx.h"
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{

}

void PlayScene::Draw()
{
	
	/*int box[10][10] ={{1,1,1,1,1,1,1,1,1 },
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
	
	for(float i = 0.1f; i < 3.0f; i += 0.1f)
	{
		for (float j = 0.1f; j < 3.0f; j+= 0.1f)
		{
			fbx->Draw(transform_);
			
			transform_.position_.x = j;
		}
		transform_.position_.z = i;
	}
  	fbx->Draw(transform_);*/
}

void PlayScene::Update()
{
}

void PlayScene::Release()
{
}
