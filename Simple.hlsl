//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────

//  Sample3D.hlsl (頂点シェーダ)
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matWolrd; //ワールド行列
    float4x4 matNomal;
    float4 diffuseColor;
    float4 diffusefactor;
    float4 specular; //スペキュラ
    float4 shininess;
    float4 ambient;
    bool useTexture; //テクスチャを使うかどうか
    
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
                 //セマンティクス
    float4 wpos : Position; //ワールド座標
    float4 spos : SP_POSITION; //スクリーン座標
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色(明るさ)
    float4 normal : NORMAL; //法線ベクトル
    float4 eyev : POSITION1; //視線ポジション
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

    //ローカル座標に、ワールド・ビュー・プロジェクション行列を掛けて
    //スクリーン座標に変換し、ピクセルシェーダーへ
    outData.spos = mul(pos, matWVP);
	//ワールド座標も変換し、ピクセルシェーダーへ
    outData.wpos = mul(pos, matWolrd);
    outData.normal = mul(normal, matNomal);
    uv.w = 1;
    outData.uv = uv.xy;
    outData.eyev = eyePosition - outData.wpos;
    
    
    //normal = mul(normal, matNomal); //法線ベクトルをワールドビュープロジェクション行列で変換
    //normal = normalize(normal); //法線ベクトルの長さを正規化->1にする
    //normal.w = 0;
    
    //float4 light = float4(-1, 0.5, -0.7, 0);
    
    //light = normalize(light);
    //light.w = 0;
    
   // outData.color = clamp(dot(normal, light), 0, 1);
    
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
   // float4 light = float4(-1, 0.5, -0.7, 0);
    float4 color;
    float4 diffuse;
    float4 ambientColor = ambient;
    float4 ambentFactor = { 0.2, 0.2, 0.2, 1.0 };
    float3 dir = normalize(lightPosition.xyz);//ピクセル位置のポリゴン3次元座標 = wpos
                                        
    diffuse = diffuseColor * diffusefactor * clamp(dot(inData.normal.xyz, dir), 0, 1);
    
    if (useTexture == 1)
    {
        //テクスチャから色を取得
        color = g_texture.Sample(g_sampler, inData.uv) + ambientColor * ambentFactor;
    }
    else
    {
        color = diffuse + diffuseColor * ambentFactor;
    }
    return color;
}