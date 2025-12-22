#include "Camera.h"


//変数
XMVECTOR position_;	//カメラの位置（視点）
XMVECTOR target_;	//見る位置（焦点）
XMMATRIX viewMatrix_;	//ビュー行列
XMMATRIX projMatrix_;	//プロジェクション行列

void Camera::Initialize()
{
    //0, 3, -10, 0  0, 10, -20, 0
    position_ = XMVectorSet(0, 3, -10, 0); //カメラの位置
    target_ = XMVectorSet(0, 0, 0, 0); //カメラの焦点
   
    //プロジェクション行列
    projMatrix_ =
        XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
   
}


void Camera::Update()
{
    //ビュー行列の作成

      viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

void Camera::SetPosition(XMVECTOR position)
{
    position_ = position;
}

void Camera::SetTarget(XMVECTOR target)
{
    target_ = target;
}

XMMATRIX Camera::GetViewMatrix()
{
    return viewMatrix_;
}

XMMATRIX Camera::GetProjectionMatrix()
{
    return projMatrix_;
}
