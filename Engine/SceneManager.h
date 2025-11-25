#pragma once
#include "GameObject.h"
//シーンIDの列挙型
enum SCENE_ID
{
	SCENE_ID_TEST,
	SCENE_ID_PLAY,
	SCENE_ID_MAX
};
//シーンの基底クラス
class SceneManager
	: public GameObject
{
private:
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
public:
	SceneManager(GameObject* parent);
	~SceneManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ChangeScene(SCENE_ID _nextScene);

};

