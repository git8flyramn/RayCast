#include "Fbx.h"
#include "Direct3D.h"
#include "Camera.h"
#include <filesystem>
#include <string>
#include <DirectXCollision.h>
namespace fs = std::filesystem;

Fbx::Fbx()
	: pVertexBuffer_(nullptr)
	, pIndexBuffer_(nullptr)
	, pConstantBuffer_(nullptr)
	, vertexCount_(0)
	, polygonCount_(0)
	, materialCount_(0)
{
}

HRESULT Fbx::Load(std::string fileName)
{
	using std::string;
	string subDir("Assets");
	fs::path currPath, basePath;
	currPath = fs::current_path();
	basePath = currPath;
	currPath = currPath / subDir;
	//fs::path subPath(currPath.string() + "\\" + subDir);
	assert(fs::exists(currPath));//subPathはあります、という確認
	fs::current_path(currPath);

	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//meshの情報を取得
	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();
	//各情報の個数を取得

	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = mesh->GetPolygonCount();	//ポリゴンの数
	materialCount_ = pNode->GetMaterialCount(); //マテリアルの数



	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();
	InitMaterial(pNode);

	fs::current_path(basePath);

	//マネージャ解放
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
	//Quadをアレンジ
	Direct3D::SetShader(SHADER_3D);
	transform.Calculation();

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = transform.GetNormalMatrix();

	//for (int i = 0;i < materialCount_;i++)
	//{
	//	if (pMaterialList_[i].pTexture)
	//	{
	//		cb.materialFlag = { 1, 1, 1,1 };
	//		cb.diffuse = XMFLOAT4(1, 1, 1, 1);//保険
	//	}
	//	else
	//	{
	//		cb.materialFlag = { 0,0,0,0 };
	//		cb.diffuse = pMaterialList_[i].diffuse;
	//	}
	//}

	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	for (int i = 0; i < materialCount_; i++)
	{
		if (pMaterialList_[i].pTexture)
		{
			cb.materialFlag = TRUE;
			cb.diffuse = XMFLOAT4(1, 1, 1, 1);//保険
		}
		else
		{
			cb.materialFlag = FALSE;
			cb.diffuse = pMaterialList_[i].diffuse;
		}
		//コンスタントバッファにデータ転送
		cb.ambient = pMaterialList_[i].ambient;
		cb.specular= pMaterialList_[i].specular;
		cb.shininess = XMFLOAT4(pMaterialList_[i].shiniess, pMaterialList_[i].shiniess, pMaterialList_[i].shiniess, pMaterialList_[i].shiniess);
		cb.diffuseFactor = pMaterialList_[i].factor;
		cb.materialFlag = pMaterialList_[i].pTexture != nullptr;

		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

		// インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用


		if (pMaterialList_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}

		//描画
		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void Fbx::Release()
{
}

void Fbx::InitVertex(FbxMesh* mesh)
{
	//VERTEX* vertices = new VERTEX[vertexCount_];
	pVertices_.resize(vertexCount_);//修正
	//全ポリゴン
	for (long poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			//[index].position
			pVertices_[index].position
				= XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//頂点のUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			//vertices[index].uv
			pVertices_[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 1.0f);
			//頂点の法線
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(poly, vertex, normal);
			//vertices[index].normal
			pVertices_[index].normal
				= XMVectorSet((float)normal[0], (float)normal[1], (float)normal[2], 0.0f);
		}
	}
	// 頂点バッファ作成
	//（自分でやって）
	//頂点バッファ
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	//data_vertex.pSysMem = vertices;
	data_vertex.pSysMem = pVertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
	}


}

void Fbx::InitIndex(FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];

	//int* index = new int[polygonCount_ * 3];
	ppIndex_.resize(materialCount_);
	indexCount_ = std::vector<int>(materialCount_);
	//indexCount_.resize(materialCount_);


	for (int i = 0; i < materialCount_; i++)
	{

		//int count = 0;
		auto& indeces = ppIndex_[i];

		//全ポリゴン
		for (long poly = 0; poly < polygonCount_; poly++)
		{

			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				for (long vertex = 0; vertex < 3; vertex++)
				{
					indeces.push_back(mesh->GetPolygonVertex(poly, vertex));
					//count++;
				}
			}
		}

		//indexCount_[i] = count;
		indexCount_[i] = (int)indeces.size();//修正
		//自力でどうぞ
		//	（ここもデータサイズを指定するところだけ注意）
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.ByteWidth = sizeof(int) * indexCount_[i];
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		//InitData.pSysMem = index;
		InitData.pSysMem = indeces.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		}

	}

}

void Fbx::InitConstantBuffer()
{
	//Quadと一緒
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
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

void Fbx::InitMaterial(FbxNode* pNode)
{
	//materialCount_の数だけ配列を準備
	pMaterialList_.resize(materialCount_);
	for (int i = 0; i < materialCount_; i++)
	{
		//i番目のマテリアル情報を取得
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		//テクスチャ情報
	    FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		//テクスチャの数数
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//テクスチャあり
		if (fileTextureCount > 0)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			fs::path tPath(textureFilePath);
			if (fs::is_regular_file(tPath))
			{
				//ここでテクスチャの読み込み
				pMaterialList_[i].pTexture = new Texture;
				pMaterialList_[i].pTexture->Load(tPath.string());
			}
			else
			{
				pMaterialList_[i].pTexture = nullptr;
				//テクスチャファイルが無いときの処理(エラー）
			}
			FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
			FbxDouble diffuse = pMaterial->DiffuseFactor;
			FbxDouble3 diffuseColor = pMaterial->Diffuse;
			FbxDouble3 ambient = pMaterial->Ambient;
			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuseColor[0], (float)diffuseColor[1], (float)diffuseColor[2], 1.0f);
			pMaterialList_[i].factor = XMFLOAT4((float)diffuse, (float)diffuse,(float)diffuse,(float)diffuse);
			pMaterialList_[i].ambient = { (float)ambient[0],(float)ambient[1],(float)ambient[2],1.0f };
			if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxDouble3 specular = pMaterial->Specular;
				FbxDouble shininess = pMaterial->Shininess; //4つとも同じ値でセット
				//ここで、自分のpMaterialList[i]に値を設定
				pMaterialList_[i].specular = { (float)specular[0],(float)specular[1],(float)specular[2],1.0f };
				pMaterialList_[i].shiniess = shininess;
			}
		}  
			                                  
		//テクスチャ無し
		else
		{
			pMaterialList_[i].pTexture = nullptr;
			//マテリアルの色
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3 diffuse = pMaterial->Diffuse;
			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
			FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pNode->GetMaterial(i);
			FbxDouble factor = pPhong->DiffuseFactor;//拡散反射強度
			pMaterialList_[i].factor = XMFLOAT4((float)factor, (float)factor, (float)factor, 1.0f);
			
			FbxDouble3 ambient = pPhong->Ambient; //環境反射率;
			pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
			if (pPhong->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxDouble specular = pPhong->SpecularFactor;//鏡面反射率
				FbxDouble shininess = pPhong->Shininess;//光沢度
				pMaterialList_[i].specular = { (float)specular, (float)specular, (float)specular,1.0f };
				pMaterialList_[i].shiniess = shininess;
			}
			else
			{
				//フォンのパラメータを持っていない時のデフォルト値
				pMaterialList_[i].specular = { 0.0f,0.0f,0.0f,1.0f };
				pMaterialList_[i].shiniess = 10.0f;
			}
		}
    	
		//ここで、自分のマテリアル構造体に詰め込む
	}

}

void Fbx::RayCast(RayCastData& rayData)
{
	for (int material = 0; material < materialCount_; material++)
	{

		auto& indices = ppIndex_[material];
		//全ポリゴンに対して
		for (int i = 0; i < (int)indices.size(); i += 3)
		{
			VERTEX& V0 = pVertices_[ indices[i + 0] ];
			VERTEX& V1 = pVertices_[ indices[i + 1] ];
			VERTEX& V2 = pVertices_[ indices[i + 2] ];

			rayData.isHit = TriangleTests::Intersects(XMLoadFloat4(&rayData.start), XMLoadFloat4(&rayData.dir), V0.position, V1.position, V2.position, rayData.dist);
	
			
		}
		//rayData.isHit = InterSects(V0, V1, V2, レイキャストのデータ);
		if (rayData.isHit)
		{
			return;
		}
		////グループ語とに全ポリゴンに対して
	 //  // 頂点を3つ取ってくる
	 //  
		//XMVECTOR start = XMLoadFloat4(&rayData.start);
		//XMVECTOR dir = XMLoadFloat4(&rayData.dir);
		//XMVECTOR dirN = XMVector4Normalize(dir); //dirの単位ベクトル
		////rayData.isHit = InterSects();
	}
	rayData.isHit = false;
}

float Math::Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
{
	// 3x3 行列の行列式 
 /*| a.x a.y a.z | 
 | b.x b.y b.z | 
 | c.x c.y c.z |*/
	return (a.x * b.x * c.x) + (a.y * b.y * c.y) + (a.z * b.z * c.z) - (a.x * b.x * c.x) - (a.y * b.y * c.y) - (a.z * b.z * c.z);
}

bool Math::Intersect(XMFLOAT3 origin, XMFLOAT3 ray, XMFLOAT3 v0, XMFLOAT3  v1, XMFLOAT3 v2, float& dist)
{
	//Rayの始点・方向、三角形の頂点をXMVECOTRに変換
	//DirectXmath
	XMVECTOR vOrigin = XMLoadFloat3(&origin); //Rayの開始点
	XMVECTOR vRay = XMLoadFloat3(&ray);//Rayの方向ベクトル
	XMVECTOR vV0 = XMLoadFloat3(&v0); //三角形の頂点v0
	XMVECTOR vV1 = XMLoadFloat3(&v1); //三角形の頂点v1
	XMVECTOR vV2 = XMLoadFloat3(&v2); //三角形の頂点v2
	
	//三角形の2本の辺ベクトルを作る
	XMVECTOR vEdge1 = vV1 - vV0;
	XMVECTOR vEdge2 = vV2 - vV0;

	//行列式計算のため,XMFLOAT3に戻す
	XMFLOAT3 edge1;
	XMFLOAT3 edge2;
	XMStoreFloat3(&edge1, vEdge1);
	XMStoreFloat3(&edge2, vEdge2);

	//d = origin - v0
	//三角形の基準点v0から,Rayの開始点originへのベクトル
	//連立方程式
	// t*ray = (v0 - origin) + u*edge1 + v*edge2を作るための準備
	XMFLOAT3 d;
	XMStoreFloat3(&d,vEdge1);

	//rayを反転()
	//連立方程式を
	//u*edge1 + v*edge2 + t*(-ray) = d
    //の形に揃えるため、rayに-1を掛ける
	ray = {
		ray.x * -1.0f,
		ray.y * -1.0f,
		ray.z * -1.0f
	};

	//連立方程式(行列式)
	float denom = Det(edge1,edge2,ray);
	//平行(解なし)の判定
	if (denom <= 0.0f)
	{
		//Rayが三角形の平面と交差しない
		return false;
	}
	//クラメルの公式で、u,v,tを求める
   
	//u = det(d,edge2, -ray) /denom
	//交点がedge1 方向にどれだけ進んだか(重心座標 u)
	float u = Det(d,edge2, ray) / denom;

	// v = det(edge1, d, -ray) / denom
    // → 交点が edge2 方向にどれだけ進んだか（重心座標 v）
	float v = Det(edge1, d, ray) / denom;
	
	// t = det(edge1, edge2, d) / denom
   // → Ray の開始点から交点までの距離
	float t = Det(edge1, edge2, d) / denom;

	//--------------------------------------
    // 三角形内部 ＋ Ray の向き 判定
    //--------------------------------------
    // t >= 0  : Ray の前方向に交点がある
   // u >= 0  : v0 → v1 方向に外れていない
   // v >= 0  : v0 → v2 方向に外れていない
  // u + v <= 1 : 三角形の内部に入っている

	if (t >= 0 && u >= 0 && v >= 0 && u + v <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
