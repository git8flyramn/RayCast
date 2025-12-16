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
	
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
