#include "Sprite.h"
#include "Camera.h"
#include "Texture.h"
#include <codecvt>

//Sprite::Sprite() : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr),
//pConstantBuffer_(nullptr), pTexture_(nullptr)
//{
//}

namespace {
	// string (UTF-8) -> wstring
	std::wstring ToWString(const std::string& str) {
		if (str.empty()) return L"";
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

	// wstring -> string (UTF-8)
	std::string ToString(const std::wstring& wstr) {
		if (wstr.empty()) return "";
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}
}

Sprite::Sprite(const char* filename)
	: pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	// バッファ初期化
	HRESULT hr = InitializeBuffers();
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sprite 初期化（バッファ生成）に失敗しました", L"エラー", MB_OK);
		return;
	}

	// テクスチャ読み込み
	LoadTexture(filename);
}
Sprite::Sprite(const std::wstring& filename)
	: pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
	// バッファ初期化
	HRESULT hr = InitializeBuffers();
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sprite 初期化（バッファ生成）に失敗しました", L"エラー", MB_OK);
		return;
	}

	// テクスチャ読み込み
	LoadTexture(filename);
}

Sprite::~Sprite()
{
}

//HRESULT Sprite::Initialize()
//{
//	
//}

void Sprite::Draw(XMMATRIX& worldMatrix)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	//cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matWorld = XMMatrixTranspose(worldMatrix);


	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->DrawIndexed(6, 0, 0);
}

void Sprite::Release()
{
	//終了処理
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::LoadTexture(const char* filename)
{
	if (pTexture_) { delete pTexture_; pTexture_ = nullptr; }
	pTexture_ = new Texture();

	// Texture::Load(const char*) を想定
	// 戻り値がある場合はチェックして MessageBox などで通知すると良いです
	pTexture_->Load(filename);
}

void Sprite::LoadTexture(const std::wstring& filename)
{
	if (pTexture_) { delete pTexture_; pTexture_ = nullptr; }
	pTexture_ = new Texture();

	std::string str = ToString(filename);
	pTexture_->Load(str);
}

HRESULT Sprite::InitializeBuffers()
{
	HRESULT hr;
	//縦横2の乗数
	VERTEX vertices[] =
	{
		//{{position,{uv}}
		{{-1.0f,1.0f,0.0f,0.0f},  {0.0f,0.0f}},//四角形の頂点(左上)
		{{1.0f, 1.0f,0.0f,0.0f},  {1.0f,0.0f}},//四角形の頂点(右上)
		{{1.0f,-1.0f,0.0f,0.0f},  {1.0f,1.0f}}, //四角形の頂点(右下)
		{{-1.0f,-1.0f,0.0f,0.0f}, {0.0f,1.0f}},//四角形の頂点(左下)


	};
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	//インデックス情報
	int index[] = { 0,2,3, 0,1,2 }; //CW
	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	//pTexture_ = new Texture();
	//pTexture_->Load("Dice1.png");
	return S_OK;
}
