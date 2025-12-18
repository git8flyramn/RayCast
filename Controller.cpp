#include "Controller.h"
#include "Engine/Camera.h";
#include "Engine/Input.h"
Controller::Controller(GameObject* parent) : GameObject(parent,"Controller")
{
	transform_.position_.x = 7.0f;
	transform_.position_.y = 7.0f;
	transform_.rotate_.x = 45.0f;
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
}

void Controller::Update()
{
	if(Input::IsKey(DIK_LEFT) || Input::IsKey(DIK_RIGHT))
	{
		transform_.rotate_.y += 0.3f;
	}

	if(Input::IsKey(DIK_UP) || Input::IsKey(DIK_DOWN))
	{
		transform_.rotate_.x += 1.0f;
	}

	if (Input::IsKey(DIK_A) || Input::IsKey(DIK_B))
	{
		transform_.position_.x += 1.0f;
	}

	if (Input::IsKey(DIK_W) || Input::IsKey(DIK_S))
	{
		transform_.position_.z += 1.0f;
	}
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
