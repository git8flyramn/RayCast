#include "TestScene.h"
#include "Engine/Input.h"
#include "Engine//SceneManager.h"
#include "Stage.h"
#include "Controller.h"
TestScene::TestScene(GameObject* parent)
	: GameObject(parent,"TestScene"), fbx(nullptr)
{
	
}

TestScene::~TestScene()
{
	
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Controller>(this);
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
