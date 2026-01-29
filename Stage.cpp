#include "Stage.h"
#include <string>
#include "vector"
#include "Engine/Model.h"
#include "resource.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

Stage::Stage(GameObject* parent) : GameObject(parent, "Stage"), pConstantBuffer_(nullptr),pImage(nullptr)
{
	
}

Stage::~Stage()
{
}
void Stage::InitConstantBuffer()
{
	//Quadと一緒
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANTBUFFER_STAGE);
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
	pImage = new Sprite("Aseets//texture.png");
}

void Stage::Draw()
{

	
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
	//コンスタントバッファ  1番から１スロット使う
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	static Transform trans;
	trans.scale_ = { 0.5f,0.5f,1.0f };
	trans.Calculation();
	XMMATRIX worldMatrix = XMMatrixIdentity();
	pImage->Draw(worldMatrix);
	//
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
	
	ImGui::Text("Stage Class rot:%lf", trans.rotate_.z);
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
	
}

void Stage::Release()
{
}

//BOOL Stage::localProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	//switch (message) {
//	//case WM_COMMAND: //コントロールの操作
//	//	switch (LOWORD(wParam))
//	//	{
//	//	case IDOK:
//	//		EndDialog(hWnd, IDOK);
//	//		return TRUE;
//	//	case IDCANCEL:
//	//		EndDialog(hWnd, IDCANCEL);
//	//		return TRUE;
//	//	}
//	//	break;
//	//}
//	//return FALSE;
//}

//BOOL Stage::ManuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{  
//	//switch (message){
//	//case WM_INITDIALOG:
//	//	SendMessage(GetDlgItem(hWnd, IDC_RADIO2), BM_SETCHECK,BST_CHECKED,0);
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"デフォルト");
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"レンガ");
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"草地");
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"砂地");
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"水場");//水場
//	//	SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, 0, 0);
//	//	return TRUE;
//	//case WM_COMMAND:
//	//	switch (LOWORD(wParam))
//	//	{
//	//	case IDC_RADIO2:
//	//		mode_ = 0;
//	//		return TRUE;
//	//	case IDC_RADIO3:
//	//		mode_ = 1;
//	//		return TRUE;
//	//	case IDC_RADIO4:
//	//		mode_ = 2;
//	//		return TRUE;
//	//	case IDC_COMBO1:
//	//		select_ = (int)SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);
//	//		return TRUE;
//	//	}
//	//	return FALSE;
//	//}
//	//return FALSE;
//}
//
