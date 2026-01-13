#include "SceneManager.h"
//#include "../PlayScene.h"
#include "../TestScene.h"
#include "Direct3D.h"
#include "Model.h"
SceneManager::SceneManager(GameObject* parent)
	:GameObject(parent, "SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate<TestScene>(this);

}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_)
	{
		//シーン切り替え処理
		auto scene = childList_.front();
		scene->ReleaseSub();
		SAFE_DELETE(scene);
		childList_.clear();
		//Model::Release();
		switch (nextSceneID_)
		{
		case SCENE_ID_PLAY:
			//Instantiate<PlayScene>(this);
			//Instantiate<TestScene>(this);
			break;
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this);
			break;
		}
		//今のシーンを次のシーンに更新する
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID _nextScene)
{
	nextSceneID_ = _nextScene;
}