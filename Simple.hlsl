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
    row_major float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    row_major float4x4 matWolrd; //ワールド行列
    row_major float4x4 matNomal;//法線変換行列
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
    float4 wpos  : Position0; //ワールド座標
    float4 spos  : SP_POSITION; //スクリーン座標
    float2 uv    : TEXCOORD; //UV座標
    float4 color : COLOR; //色(明るさ)
    float4 normal: NORMAL; //法線ベクトル
    float4 eyev  : POSITION1; //視線ポジション
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
    float4 n = normal;
    n.w = 0;
    outData.normal = mul(normal, matNomal);
    outData.normal.xyz = normalize(mul(n, matNomal).xyz);
    
    uv.w = 0;
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
    float4 diffuse;
    float4 ambientColor = ambient;
    float4 ambentFactor = { 0.1, 0.1, 0.1, 1.0 };
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz);//ピクセル位置のポリゴン3次元座標 = wpos
    
    float3 k = { 0.2f, 0.2f, 1.0f };
    float len = length(lightPosition.xyz - inData.wpos.xyz);
    float dTerm = 1.0 / (k.x + k.y * len + k.z * len * len);//距離減衰計算
   // float dTerm = 1.0;
    float3 N = normalize(inData.normal.xyz);
    diffuse = diffuseColor * diffusefactor * clamp(dot(N, dir), 0, 1) * dTerm;
    
    float3 L = normalize(lightPosition.xyz - inData.wpos.xyz);
    float ndotl = saturate(dot(N, L));
    float spec = 0.0;
    
    if(ndotl > 0.0)
    {
        float3 R = reflect(L, N);
        float3 V = normalize(inData.eyev.xyz);
        spec = pow(saturate(dot(R, V)), 32.0) * ndotl;

    }
    float4 specularCol = specular * spec;
    
    float4 diffuseTerm;
    float4 specularTerm = specularCol * dTerm;
    
    
    float4 ambientTerm;
    float4 color;
    diffuse = diffuseColor * diffusefactor * clamp(dot(inData.normal.xyz, dir), 0, 1);
    
    if (useTexture == 1)
    {
        //テクスチャから色を取得
        diffuseTerm = diffuse * g_texture.Sample(g_sampler, inData.uv);
        ambientTerm = ambentFactor * g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
        diffuseTerm = diffuse * dTerm;
        ambientTerm = ambentFactor * diffuseColor;
    }
    color = diffuseTerm + specularTerm + ambientTerm;
    return color;
}