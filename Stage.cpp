#include "Stage.h"
#include <string>
#include "vector"
#include "Engine/Model.h"
#include "resource.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include <cassert>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


namespace
{
	bool isBump = false;
}
Stage::Stage(GameObject* parent) : GameObject(parent, "Stage"), pConstantBuffer_(nullptr)
{
	hball_ = -1;
	hRoom_ = -1;
	hDonut_ = -1;
	hRoom_ = -1;
	hGround_ = -1;
}

Stage::~Stage()
{
}
void Stage::InitConstantBuffer()
{
	//Quadと一緒
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_STAGE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
	}
}

void Stage::Initialize()
{
	/*std::vector<string> ModelName
	{
		"default.fbx",
		"Lenga.fbx",
		"Glass.fbx",
		"Sand.fbx",
		"Donut.fbx",
	};
	for (int i = 0; i < ModelName.size(); i++)
	{
		hModel[i] = Model::Load(ModelName[i]);
		assert(hModel[i] >= 0);
	}*/

	InitConstantBuffer();
	hball_ = Model::Load("Ball.fbx");
	assert(hball_ >= 0);

	hRoom_ = Model::Load("Room.fbx");
	assert(hRoom_ >= 0); 
	
	hGround_ = Model::Load("Ground.fbx");
	assert(hGround_ >= 0);

	hDonut_ = Model::Load("Donut.fbx");
	assert(hDonut_ >= 0);
	
	Camera::SetPosition({ 0,0.8,-2.8 });
	Camera::SetTarget({ 0,0.8,0 });
}

void Stage::Draw()
{ 
	Transform ltr;
	ltr.position_ = { Direct3D::GetLightPos().x,Direct3D::GetLightPos().y,Direct3D::GetLightPos().z };
	ltr.scale_ = { 0.1,0.1,0.1 };
	Model::SetTransform(hball_, ltr);
	Model::Draw(hball_);
	
	Transform tr;
	tr.position_ = { 0,0,0 };
	tr.rotate_ = { 0,180.0f,0 };
	Model::SetTransform(hRoom_, tr);
	Model::Draw(hRoom_);
	/*for (int i = 0; i < ZSIZE; i++)
	{
		for (int j = 0; j < XSIZE; j++)
		{
			for(int k = 0; k < (int)(GetT(i,j).height); k++)
			{
			 int type = (int)(GetT(i,j)).type;
			 Transform t;
			 t.position_.x = i - 15 / 2.0;
			 t.position_.z = j;
			 t.position_.y = k - 15;
			 t.scale_ = { 0.95,0.95,0.95 };
			 Model::SetTransform(hModel[type],t);
			 Model::Draw(hModel[type]);
			}

		}
		
	}*/

	static Transform tDount;
	tDount.scale_ = { 0.25,0.25,0.25 };
	tDount.position_ = { 0,0.5,0 };
	//tDount.rotate_.y += 0.1f;
	Model::SetTransform(hDonut_, tDount);
	Model::Draw(hDonut_);
	//Model::DrawPseudoNormal(hDonut_);
	Transform tGround;
	tGround.scale_ = { 2.0f,2.0f,2.0f };
	tGround.position_ = { 0,0.01f,0 };
	Model::SetTransform(hGround_, tGround);
	Model::Draw(hGround_);
	
	/*static Transform trans;
	trans.scale_ = { 0.5f,0.5f,1.0f };
	trans.Calculation();
	XMMATRIX worldMatrix = XMMatrixIdentity();*/
	

	//ボックスを敷き詰める
	/*int type = BLOCK_TYPE::WATER;
	for (int i = 0; i < ZSIZE; i++)
	{
		for (int j = 0; j < XSIZE; j++)
		{
			for(int k = 0; k < (int)(GetT(i,j).height); k++)
			{
			Transform trans;
			trans.position_.x = 5 + i;
			trans.position_.y = 0;
			trans.position_.z = 5 + j;
			trans.scale_ = { 0.95,0.95,0.95 };
			
			Model::SetTransform(hModel[type], trans);
			Model::Draw(hModel[type]);
			}

		}

	}*/
	
//	ImGui::Text("Stage Class rot:%lf", tDount.rotate_.z);
	


//RayCastData rayData
	//{
	//	{0.0f,0.0f,5.0f,0.0f},
	//	{ 0.0f,-1.0f, 0.0f,0.0f},
	//	false,
	//	0.0f
	//};
	//
	//
	//Model::Raycast(hModel[type], rayData);

	//if (rayData.isHit)
	//{
	//	MessageBoxA(NULL,"hit","Info",MB_OK);

	//}
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
	if (Input::IsKey(DIK_A))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x - 0.01f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_D))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x + 0.01f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_W))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y,p.z + 0.01f,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_UP))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y + 0.01f,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKey(DIK_DOWN))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y - 0.01f,p.z,p.w };
		Direct3D::SetLightPos(p);
	}
	if (Input::IsKeyDown(DIK_B))
	{
		isBump = !isBump;
	}
	
	CONSTANT_BUFFER_STAGE cb;
	cb.lightPosition = Direct3D::GetLightPos();
	XMStoreFloat4(&cb.eyePosition,Camera::GetPosition());
	
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//ピクセルシェーダー用
}

void Stage::Release()
{
}

//BOOL Stage::localProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message) {
//	case WM_COMMAND: //コントロールの操作
//		switch (LOWORD(wParam))
//		{
//		case IDOK:
//			EndDialog(hWnd, IDOK);
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hWnd, IDCANCEL);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}
//
//BOOL Stage::ManuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{  
//	switch (message){
//	case WM_INITDIALOG:
//		SendMessage(GetDlgItem(hWnd, IDC_RADIO2), BM_SETCHECK,BST_CHECKED,0);
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"デフォルト");
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"レンガ");
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"草地");
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"砂地");
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"水場");//水場
//		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, 0, 0);
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam))
//		{
//		case IDC_RADIO2:
//			mode_ = 0;
//			return TRUE;
//		case IDC_RADIO3:
//			mode_ = 1;
//			return TRUE;
//		case IDC_RADIO4:
//			mode_ = 2;
//			return TRUE;
//		case IDC_COMBO1:
//			select_ = (int)SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);
//			return TRUE;
//		}
//		return FALSE;
//	}
//	return FALSE;
//}
//
