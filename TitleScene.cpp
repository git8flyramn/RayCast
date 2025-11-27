#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine//SceneManager.h"
#include "Engine/Texture.h"
#include "Engine/Model.h"
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent,"TitleScene"),fbx(nullptr)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{

}

void TitleScene::Draw()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Release()
{
}
