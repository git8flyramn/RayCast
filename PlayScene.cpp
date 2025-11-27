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
	fbx->Load("Lenga.fbx");
	transform_.position_ = { 0.0f,0.0f,0.0f };
}

void PlayScene::Draw()
{
	fbx->Draw(transform_);
}

void PlayScene::Update()
{
}

void PlayScene::Release()
{
}
