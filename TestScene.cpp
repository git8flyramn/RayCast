#include "TestScene.h"
#include "Engine/Input.h"
#include "Engine//SceneManager.h"
#include "Engine/Texture.h"
TestScene::TestScene(GameObject* parent)
	: GameObject(parent,"TestScene")
{
	
}

TestScene::~TestScene()
{
	
}

void TestScene::Initialize()
{
}

void TestScene::Update()
{
	//スペースキーを押したら,SceneManager::ChangeScene(SCENE_ID_PLAY);を呼び出す
	if (Input::IsKeyDown(DIK_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	 
}

void TestScene::Draw()
{
	
}

void TestScene::Release()
{
}
